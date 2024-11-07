#ifndef edit_H
#define edit_H

#include "types.h"

typedef struct 
{
    char *edit_content;
    char *fname;
    FILE *fptr_edit;
    FILE *fptr_new;
}edit;


Status validate(char *argv[], edit * edit_fileinfo);

Status opening_file(edit * edit_fileinfo);

Status skip_data(FILE *src, FILE *dest);

Status edit_option(char *argv[], edit *edit_fileinfo);

Status edit_data(FILE *src, FILE *dest, char *argv[]);

Status copy_remaining(FILE * src, FILE* dest);


#endif