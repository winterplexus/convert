/*
**  @(#)convert.c
**
**  convert - text file converter utility
**  -------------------------------------
**
**  copyright (c) 1996-2021 Code Construct Systems (CCS)
*/
#include "modules.h"

/*
** File stream information structure and type
*/
typedef struct file_stream_s {
        string_c_t name;
        FILE *fp;
        long counter;
        long line_counter;
} file_stream_t;

/*
** Unwind buffer stack pointer
*/
jmp_buf unwind_buffer_sp;

/*
** Local function prototypes
*/
static void SetSystemSignals(void);
static void InterruptHandler(int);
static void ProcessFiles(options_t *, file_stream_t *, file_stream_t *);
static void DisplayStatistics(file_stream_t *, file_stream_t *);
static void OverWriteInputFile(file_stream_t *, file_stream_t *);
static void OpenFiles(options_t *, file_stream_t *, file_stream_t *);
static void CloseFiles(options_t *, file_stream_t *, file_stream_t *);
static bool_c_t WriteEndOfLine(int *, file_stream_t *, file_stream_t *);
static bool_c_t ReadByte(file_stream_t *, int *);
static bool_c_t WriteByte(file_stream_t *, int);

/*
** Text file converter utility driver
*/
int main(int argc, string_c_t argv[]) {
    options_t opts;
    file_stream_t input_file = { 0, stdin, 0, 0 };
    file_stream_t output_file = { 0, stdout, 0, 0 };

    /*
    ** Set system signals and if stack was unwound, close files and exit
    */
    SetSystemSignals();
    if (setjmp(unwind_buffer_sp) != 0) {
        CloseFiles(&opts, &input_file, &output_file);
        return (EXIT_FAILURE);
    }

    /*
    ** Get options
    */
    GetOptions(argc, argv, &opts);

    /*
    ** Process input and output files
    */
    ProcessFiles(&opts, &input_file, &output_file);

    /*
    ** Overwrite input file with output file (if required)
    */
    if (opts.overwrite_input_file) {
        OverWriteInputFile(&input_file, &output_file);
    }

    /*
    ** Display statistics (if required)
    */
    if (opts.statistics) {
        DisplayStatistics(&input_file, &output_file);
    }

    /*
    ** Return success return code
    */
    return (EXIT_SUCCESS);
}

/*
** Set system signals
*/
static void SetSystemSignals(void) {
    signal(SIGINT,  InterruptHandler);
    signal(SIGTERM, InterruptHandler);
}

/*
** Interrupt handler
*/
static void InterruptHandler(int signal_number) {
    printf("signal detected (%d)!\n", signal_number);
    EXIT_APPLICATION(EXIT_SUCCESS);
}

/*
** Process files
*/
static void ProcessFiles(options_t *opts, file_stream_t *input_file, file_stream_t *output_file) {
    int file_byte;

    /*
    ** Open files
    */
    OpenFiles(opts, input_file, output_file);

    /*
    ** Process each byte from input file until either I/O error or when at end of file (EOF)
    */
    for (;;) {
        /*
        ** Read a byte from input file
        */
        if (!ReadByte(input_file, &file_byte)) {
            break;
        }

        /*
        ** Write end of line character sequence
        */
        if (!WriteEndOfLine(&file_byte, input_file, output_file)) {
            break;
        }

        /*
        ** Write byte to output file
        */
        if (!WriteByte(output_file, file_byte)) {
            break;
        }
    }

    /*
    ** Close files
    */
    CloseFiles(opts, input_file, output_file);
}

/*
** Overwrite input file with output file
*/
static void OverWriteInputFile(file_stream_t *input_file, file_stream_t *output_file) {
    /*
    ** Remove input file first
    */
    if (remove(input_file->name) != EXIT_SUCCESS) {
        printf("error-> unable to remove input file: %s (%d)\n", input_file->name, errno);
        return;
    }

    /*
    ** Rename output file as input file
    */
    if (rename(output_file->name, input_file->name) != EXIT_SUCCESS) {
        printf("error-> unable to rename output file as input file: %s (%d)\n", output_file->name, errno);
    }
}

/*
** Display statistics
*/
static void DisplayStatistics(file_stream_t *input_file, file_stream_t *output_file) {
    printf("statistics\n");
    printf("\ninput file\n");
    printf("--------------------------------------------------------------------------------\n");
    printf("total lines : %ld\n", input_file->line_counter);
    printf("total size  : %ld bytes\n", input_file->counter);
    printf("\noutput file\n");
    printf("--------------------------------------------------------------------------------\n");
    printf("total lines : %ld\n", output_file->line_counter);
    printf("total size  : %ld bytes\n", output_file->counter);
}

/*
** Open files
*/
static void OpenFiles(options_t *opts, file_stream_t *input_file, file_stream_t *output_file) {
    /*
    ** Open input file (read-only binary mode)
    */
    if (opts->input_name && strlen(opts->input_name) > 0) {
        input_file->name = opts->input_name;
        fopen_p(&input_file->fp, input_file->name, (string_c_t)_F_RO_BIN);
        if (input_file->fp == NULL) {
            printf("error-> unable to open input file: %s (%d)\n", input_file->name, errno);
            EXIT_APPLICATION(EXIT_FAILURE);
        }
    }

    /*
    ** Open output file (read/write binary mode)
    */
    if (opts->output_name && strlen(opts->output_name) > 0) {
        output_file->name = opts->output_name;
        fopen_p(&output_file->fp, output_file->name, (string_c_t)_F_RW_BIN);
        if (output_file->fp == NULL) {
            printf("error-> unable to open output file: %s (%d)\n", output_file->name, errno);
            EXIT_APPLICATION(EXIT_FAILURE);
        }
    }
}

/*
** Close files
*/
static void CloseFiles(options_t *opts, file_stream_t *input_file, file_stream_t *output_file) {
    /*
    ** Flush output buffer
    */
    if (output_file->fp) {
        fflush(output_file->fp);
    }

    /*
    ** Close input and output files
    */
    fclose_p(input_file->fp);
    fclose_p(output_file->fp);
}

/*
** Write end of line character sequence
*/
static bool_c_t WriteEndOfLine(int *file_byte, file_stream_t *input_file, file_stream_t *output_file) {
#ifdef _LTOD
    /*
    ** Write carriage-return character to output file if input byte is a line feed character
    */
    if (*file_byte == '\n') {
        if (!WriteByte(output_file, (int)'\r')) {
            return (FALSE);
        }
        input_file->line_counter++;
        output_file->line_counter = input_file->line_counter;
    }
#else
    /*
    ** Read next byte from input file if input byte is a carriage-return character
    */
    if (*file_byte == '\r') {
        if (!ReadByte(input_file, file_byte)) {
            return (FALSE);
        }
        input_file->line_counter++;
        output_file->line_counter = input_file->line_counter;
    }
#endif
    return (TRUE);
}

/*
** Read byte
*/
static bool_c_t ReadByte(file_stream_t *input_file, int *file_byte) {
    /*
    ** Get next byte from input file
    */
    *file_byte = fgetc(input_file->fp);

    /*
    ** Check for file error and end-of-file condition
    */
    if (ferror(input_file->fp)) {
        printf("error-> unable to read from input file: %s (%d)\n", input_file->name, errno);
        return (FALSE);
    }
    if (feof(input_file->fp)) {
        return (FALSE);
    }

    /*
    ** Increment input file byte counter
    */
    input_file->counter++;
    return (TRUE);
}

/*
** Write byte
*/
static bool_c_t WriteByte(file_stream_t *output_file, int file_byte) {
    /*
    ** Write byte to output file
    */
    fputc(file_byte, output_file->fp);

    /*
    ** Check for file error
    */
    if (ferror(output_file->fp)) {
        printf("error-> unable to write to output file: %s (%d)\n", output_file->name, errno);
        return (FALSE);
    }

    /*
    ** Increment output file file byte counter
    */
    output_file->counter++;
    return (TRUE);
}