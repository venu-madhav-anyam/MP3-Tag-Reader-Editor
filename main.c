#include<stdio.h>
#include "view.h"
#include "edit.h"

int main(int argc, char *argv[])
{

    view view_fileinfo;
    edit edit_fileinfo;

    {   //Arguments checking
        if(!(argc >= 2))
        {
            printf("Error: Invalid Arguments\n");
        printf("USAGE:\n"
            "To view please pass like: ./a.out -v <mp3_filename>\n"
            "To edit please pass like: ./a.out -t/-a/-A/-m/-y/-c <changing text> <mp3_filename>\n"
            "To get help pass like: ./a.out --help\n");
            return 1;
        }

        if ((argc != 3 && check_operation_type(argv) == e_view) || (argc != 5 && check_operation_type(argv) == e_edit))
        {
            printf("Error: Invalid arguments for Selected Operation\n");
            printf("USAGE:\n"
            "To view please pass like: ./a.out -v <mp3_filename>\n"
            "To edit please pass like: ./a.out -t/-a/-A/-m/-y/-c <changing text> <mp3_filename>\n"
            "To get help pass like: ./a.out --help\n");
            return 1;
        }
    }
    
    

    if(check_operation_type(argv) == e_view)
    {
        { //Checking for .mp3 extenstion file
            if(validate_extn(argv, &view_fileinfo) == e_failure)
            {
                printf("\nError: Invalid File (Only .mp3 Files are Allowed)\n");
                return 1;
            }
        }

        {
            if(open_file(&view_fileinfo) == e_failure)
            {
                printf("\nError: File opening failed\n");
                return 1;
            }
        }

        {
            if(display_data(&view_fileinfo) == e_success);
            else
            {
                printf("Error: Display file info failed");
            }
        }


    }



    if(check_operation_type(argv) == e_edit)
    {
        {//validatiang file extension
            if(validate(argv, &edit_fileinfo) == e_failure)
            {
                printf("\nError: Invalid File (Only .mp3 Files are Allowed)\n");
                return 1;
            }

            {//opening file
                if(opening_file(&edit_fileinfo) == e_failure)
                {
                    printf("\nError: File opening failed\n");
                    return 1;
                }
            }

            {//editing content
                if(edit_option(argv, &edit_fileinfo) == e_success)
                {
                    if(remove(argv[4]) != 0)
                    {
                        printf("\nError: Failed to remove file\n");
                        return 1;
                    }

                    if(rename("edited_song.mp3",argv[4]) != 0)
                    {
                        printf("\nError: Failed to rename file\n");
                        return 1;
                    }
                }
            }
        }
    }

    if(check_operation_type(argv) == e_help)
    {
        printf("1. -v -> view mp3 file contents\n"
               "2. -e -> to edit mp3 file contents\n"
                "         2.1. -t -> to edit song title\n"
                "         2.2. -a -> to edit artist name\n"
                "         2.3. -A -> to edit album name\n"
                "         2.4. -y -> to edit year\n"
                "         2.5. -m -> to edit content\n"
                "         2.6. -c -> to edit comment\n");
    }

    if(check_operation_type(argv) == e_unsupported)
    {
        printf("\nInvalid operation\n");
        printf("USAGE:\n"
            "To view please pass like: ./a.out -v <mp3_filename>\n"
            "To edit please pass like: ./a.out -t/-a/-A/-m/-y/-c <changing text> <mp3_filename>\n"
            "To get help pass like: ./a.out --help\n");
        return 1;
    }

    
}