#include<string.h>
#include<stdio.h>
#include "edit.h"
#include "types.h"



int count=0,flag=1;
char ch;

Status validate(char *argv[], edit * edit_fileinfo)
{
    char *ext = strstr(argv[4],".");

    if(ext != NULL && strcmp(ext,".mp3") == 0)
    {
        edit_fileinfo->fname = argv[4];
        edit_fileinfo->edit_content = argv[3];
        return e_success;
    }
    else
        return e_failure;
}


Status opening_file(edit * edit_fileinfo)
{
    edit_fileinfo->fptr_edit = fopen(edit_fileinfo->fname, "r+");

    if(edit_fileinfo->fptr_edit == NULL)
        return e_failure;

    edit_fileinfo->fptr_new = fopen("edited_song.mp3", "w+");

    if(edit_fileinfo->fptr_new != NULL)
        return e_success;
    else
        return e_failure;
}



Status skip_data(FILE *src, FILE *dest)
{
    long cur = ftell(src);
    int tag_size=0,count=0;
    char tag[50],tit2_id[4];

    unsigned char size[4];

    if(fread(tit2_id,1,4,src)!=4)
        return e_failure;

    if(fread(size,1,4,src)!=4)
        return e_failure;

    tag_size = (size[0] << 21) | (size[1] << 14) | (size[2] << 7) | size[3];

    if(fseek(src,3,SEEK_CUR)!=0)
        return e_failure;

    if (tag_size - 1 > sizeof(tag))
        return e_failure;

    if(fread(tag,1,tag_size-1,src)!=tag_size-1)
        return e_failure;

    long end = ftell(src);

    if(fseek(src,cur,SEEK_SET)!=0)
            return e_failure;

    long bytes = (end-cur);

    while (bytes-- > 0)
    {
        fread(&ch, 1, 1,src );
        fwrite(&ch, 1, 1, dest);
    }
    
    return e_success;
}


Status edit_data(FILE *src, FILE *dest, char *argv[])
{
    char tit2_id[4];
    char tsize[4];
    int size = strlen(argv[3])+1;

    if(fread(tit2_id,1,4,src)!=4)
        return e_failure;

    if(fwrite(tit2_id,1,4,dest)!=4)
        return e_failure;

    if(fread(tsize,1,4,src)!=4)
        return e_failure;

    int tag_size = (tsize[0] << 21) | (tsize[1] << 14) | (tsize[2] << 7) | tsize[3];

    fseek(src,-4,SEEK_SET);

    {
        char frame_size[4];

        frame_size[0] = (size >> 24) & 0xFF;
        frame_size[1] = (size >> 16) & 0xFF;
        frame_size[2] = (size >> 8) & 0xFF;
        frame_size[3] =  size & 0xFF;

        if(fwrite(frame_size,1,4,dest)!=4)
            return e_failure;

        // if(fseek(src,3,SEEK_CUR)!=0)
        //     return e_failure;

        while (count++ < 3)
        {
            fread(&ch, 1, 1, src);
            fwrite(&ch, 1, 1, dest);
        }

    if(fwrite(argv[3],1,strlen(argv[3]),dest)!=strlen(argv[3]))
        return e_failure;
    }
    
    if(fseek(src,tag_size-1,SEEK_CUR)!=0)
        return e_failure;

    printf("----------------------------------------------------------------------\n");
    printf("%51s\n","MP3 Tag Data Edited Successfully");
    printf("----------------------------------------------------------------------\n");

    return e_success;
}



Status edit_option(char *argv[], edit *edit_fileinfo)
{
    {
        while (ftell(edit_fileinfo->fptr_edit)!=10)
        {
            fread(&ch, 1, 1, edit_fileinfo->fptr_edit);
            fwrite(&ch, 1, 1, edit_fileinfo->fptr_new);
        }
    }


    if(strcmp(argv[2],"-t") == 0)
    {

        if (edit_data(edit_fileinfo->fptr_edit, edit_fileinfo->fptr_new, argv) == e_failure)
            return e_failure;
        printf("Title     :  %s\n",argv[3]);
        flag=0;
    }
    else
    {
        if(skip_data(edit_fileinfo->fptr_edit,edit_fileinfo->fptr_new) == e_failure)
            return e_failure;
    }
    
    
    if(strcmp(argv[2],"-a") == 0)
    {
        if (edit_data(edit_fileinfo->fptr_edit, edit_fileinfo->fptr_new, argv) == e_failure)
            return e_failure;
        printf("Artist     :  %s\n",argv[3]);
        flag=0;
    }
    else
    {
        if(skip_data(edit_fileinfo->fptr_edit,edit_fileinfo->fptr_new) == e_failure)
            return e_failure;
    }
    
    if(strcmp(argv[2],"-A") == 0)
    {
        if (edit_data(edit_fileinfo->fptr_edit, edit_fileinfo->fptr_new, argv) == e_failure)
            return e_failure;
        printf("Album     :  %s\n",argv[3]);
        flag=0;
    }
    else
    {
        if(skip_data(edit_fileinfo->fptr_edit,edit_fileinfo->fptr_new) == e_failure)
            return e_failure;
    }
    
    if(strcmp(argv[2],"-y") == 0)
    {
        if (edit_data(edit_fileinfo->fptr_edit, edit_fileinfo->fptr_new, argv) == e_failure)
            return e_failure;
        printf("Year     :  %s\n",argv[3]);
        flag=0;
    }
    else
    {
        if(skip_data(edit_fileinfo->fptr_edit,edit_fileinfo->fptr_new) == e_failure)
            return e_failure;
    }
    
    
    if(strcmp(argv[2],"-m") == 0)
    {
        if (edit_data(edit_fileinfo->fptr_edit, edit_fileinfo->fptr_new, argv) == e_failure)
            return e_failure;
        printf("Genre     :  %s\n",argv[3]);
        flag=0;
    }
    else
    {
        if(skip_data(edit_fileinfo->fptr_edit,edit_fileinfo->fptr_new) == e_failure)
            return e_failure;
    }
    
    
    if(strcmp(argv[2],"-c") == 0)
    {
        if (edit_data(edit_fileinfo->fptr_edit, edit_fileinfo->fptr_new, argv) == e_failure)
            return e_failure;
        printf("Comments     :  %s\n",argv[3]);
        flag=0;
    }
    
    if(flag)
    {
        printf("\nError: Enter a valid edit option\n");
        flag=1;
        printf("USAGE:\n"
            "To edit please pass like: ./a.out -t/-a/-A/-m/-y/-c <changing text> <mp3_filename>\n");
        return e_failure;
    }

    
    if(copy_remaining(edit_fileinfo->fptr_edit, edit_fileinfo->fptr_new) == e_failure)
        return e_failure;
    flag=1;
    
    return e_success;
}




Status copy_remaining(FILE * src, FILE* dest)
{
    while (fread(&ch, 1, 1,src ))
        fwrite(&ch, 1, 1, dest);

    printf("----------------------------------------------------------------------\n");

    return e_success;
}