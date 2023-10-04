#include <stdio.h>
#include <unistd.h>
#include "liblzwcompress/compress.c"
#include "liblzwcompress/decompress.c"

int file_check(FILE* file)
{
    rewind(file);
    getc(file);
    if (feof(file))
        {
        rewind(file);
        printf("Файл пуст.\n");
        return EOF;
        }      
    rewind(file);
    return 1;
}

int main(int argc,char* argv[])
{

    int swt = 0;
    int option = 0;
    FILE* output_file = NULL;
    FILE* input_file = NULL;
    int argcount= argc-1;    

    if (argc<3){
        printf("Недостаточно аргументов\n");
        return 1;
    }
    if (argc>5){
        printf("Слишком много аргументов\n");
        return 1;
    }
   
    // Прохожу по аргументам. Дальше в зависимости от option будем выполнять нужное действие:
    // option = 0 -> нет аргумента | option = 1 -> декомпрессия | option = 2 -> сжатие | option = 3 -> оба аргумента 

    while ((swt = getopt(argc, argv, "cdo:")) != -1){
        switch(swt){
            case 'c': option = 2; break;
            case 'd': option++; break;
            case 'o': 
                if (argc!=5){
                    printf("Недостаточно аргументов\n");
                    return 1;
                }
                input_file = fopen(argv[argcount], "r");
                    if (!input_file)
                        {
                        printf("Неудалось открыть входной файл\n");
                        return 1;
                        }
                if(optarg==NULL){
                    printf("Необходимо указать имя выходного файла\n");
                    return 1;
                }   
                if (file_check(input_file)==EOF)
                    return 1;       
                output_file = fopen(optarg,"wb"); 
                break;
        }
    }

    input_file = fopen(argv[argcount], "r");

    if (!input_file)
    {
        printf("Неудалось открыть входной файл\n");
        return 1;
    }
    if (file_check(input_file)==EOF)
        return 1;
    
    // Если нет ключа "-o" то создается шаблонный файл

    if ((output_file==NULL) && (option == 2)){
        output_file = fopen("compressed.lzw","wb");
    } else if ((output_file==NULL) && (option == 1))
    {
       output_file = fopen("uncompressed.txt","wb"); 
    }




    if (!output_file)
    {
        printf("Неудалось открыть выходной файл\n");
        return 1;
    }        

    switch(option){
        case 0: printf("Недостаточно аргументов.\n"); return 1; break;
        case 1: decompress(input_file,output_file); break;
        case 2: compress(input_file,output_file); break;
        case 3: printf("Слишком много аргументов\n"); return 1; break;
    }

    return 0;
}