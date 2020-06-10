/*
**  @(#)options.h
**
**  convert - command line options
**  ------------------------------
**
**  copyright (c) 1996-2020 Code Construct Systems (CCS)
*/
#ifndef __OPTIONS_H
#define __OPTIONS_H

/*
** Temporary file name prefix
*/
#define TEMP_FNAME_PREF "convert"

/*
** Command line options structure and type
*/
typedef struct options_s {
        char input_name[_MAX_FILE_NAME_SIZE + 1];
        char output_name[_MAX_FILE_NAME_SIZE + 1];
        bool_c_t overwrite_input_file;
        bool_c_t statistics;
} options_t;

/*
** Function prototypes
*/
extern void GetOptions(int, char **, options_t *);

#endif /* __OPTIONS_H */
