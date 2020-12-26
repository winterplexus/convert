/*
**  @(#)options.c
**
**  convert - command line options
**  ------------------------------
**
**  copyright (c) 1996-2021 Code Construct Systems (CCS)
*/
#include "modules.h"

/*
** Local function prototypes
*/
static void GetOptionValues(int, char **, options_t *);
static void SetDefaultOptions(options_t *);
static void StoreOptionArgument(int, string_c_t [], int, string_c_t, size_t);
static void DisplayVersion(int);
static void DisplayUsage(void);

/*
** Get command line options
*/
void GetOptions(int argc, char **argv, options_t *opts) {
    /*
    ** Set default options
    */
    SetDefaultOptions(opts);

    /*
    ** Display usage if no argument; otherwise get option values
    */
    if (argc == 1) {
        DisplayUsage();
    }
    else {
        GetOptionValues(argc, argv, opts);
    }
}

/*
** Get option values
*/
static void GetOptionValues(int argc, char **argv, options_t *opts) {
    int i;
    time_t msec = time(NULL) * 1000;

    /*
    ** Process each command line argument
    */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            StoreOptionArgument(argc, argv, ++i, opts->input_name, sizeof(opts->input_name) - 1);
        }
        else if (strcmp(argv[i], "-o") == 0) {
            StoreOptionArgument(argc, argv, ++i, opts->output_name, sizeof(opts->output_name) - 1);
        }
        else if (strcmp(argv[i], "-s") == 0) {
            opts->statistics = TRUE;
        }
        else if (strcmp(argv[i], "-v") == 0) {
            DisplayVersion(argc);
        }
        else if (strcmp(argv[i], "-w") == 0) {
            opts->overwrite_input_file = TRUE;
        }
        else {
            EXIT_APPLICATION(EXIT_FAILURE);
        }
    }

    /*
    ** Set overwrite file name (if required)
    */
    if (opts->overwrite_input_file) {
        strfmt_p(opts->output_name, sizeof(opts->output_name), (string_c_t)".%s.%d", TEMP_FNAME_PREF, msec);
    }
}

/*
** Set default options
*/
static void SetDefaultOptions(options_t *opts) {
    memset(opts->input_name, 0, _MAX_FILE_NAME_SIZE + 1);
    memset(opts->output_name, 0, _MAX_FILE_NAME_SIZE + 1);
    opts->statistics = FALSE;
    opts->overwrite_input_file = FALSE;
}

/*
** Store option argument
*/
static void StoreOptionArgument(int argc, string_c_t argv[], int i, string_c_t argument, size_t size) {
    if (argc < i || !argv[i]) {
        DisplayUsage();
    }
    assert(argv[i]);
    strcpy_p(argument, size, argv[i], size);
}

/*
** Display version
*/
static void DisplayVersion(int argc) {
    printf("%s - convert text file utility\n", _VERSION_PRODUCT);
    printf("%s\n\n", _VERSION_RELEASE);

    /*
    ** Exit application (if no other command line arguments)
    */
    if (argc == 2) {
        EXIT_APPLICATION(EXIT_SUCCESS);
    }
}

/*
** Display usage
*/
static void DisplayUsage(void) {
    printf("usage: %s (options)\n\n", _VERSION_PRODUCT);
    printf("where (options) include:\n");
    printf("  -i  [input file name]\n");
    printf("  -o  [output file name]\n");
    printf("  -w  enable write over input file as output mode\n");
    printf("  -v  display version\n");

    /*
    ** Exit application
    */
    EXIT_APPLICATION(EXIT_SUCCESS);
}
