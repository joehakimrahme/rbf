#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MEM_SIZE 30000
#define MAX_LINE_LENGTH 256

#define ERRNOMATCH -1

int matchparenup (char* input, int i)
{
    int nr = 0;
    while (input[i++] != '\0'){
        switch (input[i]){
            case '[':
                nr++;
                break;
            case ']':
                nr--;
                if (nr <0)
                    return i;
                break;
            default:
                break;
        }
    }

    return ERRNOMATCH;
}

int matchparendown (char* input, int i)
{
    int nr = 0;
    while (i-- != 0){
        switch (input[i]){
            case ']':
                nr++;
                break;
            case '[':
                nr--;
                if (nr <0)
                    return i;
                break;
            default:
                break;
        }
    }

    return ERRNOMATCH;
}

char *get_file_tostr (FILE* file){
    char *file_contents = NULL;
    long long input_file_size = 0;
    FILE *input_file = file;

    fseek(input_file, 0, SEEK_END);
    input_file_size = ftell(input_file);
    rewind(input_file);

    file_contents = (char *)malloc(input_file_size * (sizeof (char)) + 1 );
    fread(file_contents, sizeof (char), input_file_size, input_file);
    fclose(input_file);
    file_contents[input_file_size]='\0';
    return file_contents;
}

int main (int argc, char **argv)
{
    char *input = (char *)malloc (MAX_LINE_LENGTH);
    char bytes [MEM_SIZE] = {0};

    int pos=0;
    int i=0;

    FILE* bf_file = NULL;


    if (argc < 2){
        fprintf (stderr, "No script provided. Exiting.\n");
        return EXIT_FAILURE;
    } else
        bf_file = fopen (argv[1], "rb"); // Check for errors?


    input = get_file_tostr (bf_file);

    while (input[i] != '\0'){

        switch (input[i]){
            case '+':
                bytes[pos]++;
                break;
            case '-':
                bytes[pos]--;
                break;
            case '>':
                pos = (pos+1) % MEM_SIZE;
                break;
            case '<':
                pos = (pos-1) % MEM_SIZE;
                break;
            case '.':
                printf ("%c", bytes[pos]);
                break;
            case ',':
                bytes[pos] = fgetc(stdin);
                while( (bytes[pos] == '\n') || (bytes[pos] == ' ') && (bytes[pos] != EOF) )
                    bytes[pos] = fgetc(stdin);
                break;
            case '[':
                if (bytes[pos] == 0)
                    i = matchparenup(input, i);
                break;
            case ']':
                if (bytes[pos] != 0)
                    i = matchparendown(input, i);
                    break;
            default:
                break;
        }

        if (i != ERRNOMATCH)
            i++;
        else
        {
            fprintf (stderr, "ERROR: Umatched '['\n");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
