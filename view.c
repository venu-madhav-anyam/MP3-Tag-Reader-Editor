#include<string.h>
#include<stdlib.h>
#include "view.h"
#include "types.h"




OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-v") == 0)
        return e_view;
    else if (strcmp(argv[1],"-e") == 0)
        return e_edit;
    else if(strcmp(argv[1],"--help") == 0)
        return e_help;
    else
        return e_unsupported;

}

Status validate_extn(char *argv[], view * view_fileinfo)
{
    char *ext = strstr(argv[2],".");

    if(ext != NULL && strcmp(ext,".mp3") == 0)
    {
        view_fileinfo->fname = argv[2];
        return e_success;
    }
    else
        return e_failure;
}

Status open_file(view * view_fileinfo)
{
    view_fileinfo->fptr_mp3 = fopen(view_fileinfo->fname, "r+");

    if(view_fileinfo->fptr_mp3 != NULL)
        return e_success;
    else
        return e_failure;
}



Status extract_data(FILE *fptr, char *data)
{
    int tag_size=0;
    char tag[50];

    unsigned char size[4];

    if(fread(size,1,4,fptr)!=4)
        return e_failure;

    tag_size = (size[0] << 21) | (size[1] << 14) | (size[2] << 7) | size[3];

    if(fseek(fptr,3,SEEK_CUR)!=0)
        return e_failure;

    if(fread(tag,1,tag_size-1,fptr)!=tag_size-1)
        return e_failure;

    tag[tag_size-1]='\0';

    strcpy(data,tag);

    return e_success;
}



Status display_data(view * view_fileinfo)
{
    printf("----------------------------------------------------------------------\n");
    printf("%40s\n","MP3 Tag Data");
    printf("----------------------------------------------------------------------\n");

    { //Tag id
        char buffer[4];
        if(fread(buffer,1,3,view_fileinfo->fptr_mp3)!=3)
            return e_failure;
        
        buffer[3]='\0';
        printf("%s ",buffer);

    }

    { //version
        unsigned char version[3];
        
        if(fread(version,1,2,view_fileinfo->fptr_mp3)!=2)
            return e_failure;
        
        printf("V2.%d.%d\n",version[0],version[1]);
    }


    { //tit2 data
        char tit2_id[5];

        if(fseek(view_fileinfo->fptr_mp3,5,SEEK_CUR)!=0)
            return e_failure;
        
        if(fread(tit2_id,1,4,view_fileinfo->fptr_mp3)!=4)
            return e_failure;
        
        tit2_id[4]='\0';

        if(extract_data(view_fileinfo->fptr_mp3, view_fileinfo->title) == e_success) 
            printf("Title     :  %s\n",view_fileinfo->title);
        else
            return e_failure;
    }

    { //tpe1 data
        char artist_id[5];
        
        if(fread(artist_id,1,4,view_fileinfo->fptr_mp3)!=4)
            return e_failure;

        artist_id[4]='\0';

        if(extract_data(view_fileinfo->fptr_mp3, view_fileinfo->artist) == e_success) 
            printf("Artist    :  %s\n",view_fileinfo->artist);
        else
            return e_failure;
    }


    
    { //TALB data
        char album_id[5];
        
        if(fread(album_id,1,4,view_fileinfo->fptr_mp3)!=4)
            return e_failure;

        album_id[4]='\0';

        if(extract_data(view_fileinfo->fptr_mp3, view_fileinfo->album) == e_success) 
            printf("Album     :  %s\n",view_fileinfo->album);
        else
            return e_failure;
    }


    { //TYER data
        char tyer_id[5];
        
        if(fread(tyer_id,1,4,view_fileinfo->fptr_mp3)!=4)
            return e_failure;

        tyer_id[4]='\0';

        if(extract_data(view_fileinfo->fptr_mp3, view_fileinfo->year) == e_success) 
            printf("Year      :  %s\n",view_fileinfo->year);
        else
            return e_failure;
    }

    {
        char tcon_id[5];
        
        if(fread(tcon_id,1,4,view_fileinfo->fptr_mp3)!=4)
            return e_failure;

        tcon_id[4]='\0';

        if(extract_data(view_fileinfo->fptr_mp3, view_fileinfo->genre) == e_success) 
            printf("Genre     :  %s\n",view_fileinfo->genre);
        else
            return e_failure;
        
    }


    {
        char comm_id[5];
        
        if(fread(comm_id,1,4,view_fileinfo->fptr_mp3)!=4)
            return e_failure;

        comm_id[4]='\0';

        if(extract_data(view_fileinfo->fptr_mp3, view_fileinfo->comm) == e_success) 
            printf("Comments  :  %s\n",view_fileinfo->comm);
        else
            return e_failure;
    }

    printf("----------------------------------------------------------------------\n");
    
    return e_success;
}