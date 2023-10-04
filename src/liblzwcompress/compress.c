#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
//#include <stdint.h>
#include "trie.c"
#include "constants.h"

static inline void write_code_to_file(FILE *file, uint32_t code, size_t bytes_per_code)
{
    fwrite(&code, bytes_per_code, 1, file);
}

int compress(FILE* input_file,FILE* output_file)
{


    // Резервирую место для хранения позиции в файле, где будет храниться код, использующий 24 бита (3 байта)
    fseek(output_file, sizeof(long int), SEEK_SET);

    // Резервирую место для хранения позиции в файле, где будет храниться код, использующий 32 бита (4 байта)
    fseek(output_file, sizeof(long int), SEEK_CUR);

    // Резервирую место для хранения значения максимального сгенерированного кода
    fseek(output_file, sizeof(uint32_t), SEEK_CUR);

    // Беру первый байт файла
    unsigned char c = fgetc(input_file);
    if (feof(input_file))
    {
        printf("Файл пуст.\n");
        return 1;
    }

    // Переменны, которые хранят позицию кода с 24 бит и 32 соответственно
    long int file_offset_24bit_codes = 0, file_offset_32bit_codes = 0;

    // Создаем словарь (на основе структуры Trie tree)
    TrieNode *tree = make_trie_node(); // Root node

    // Инициализируем код для словаря
    uint32_t current_code = 0;

    // Псевдо код:
    
    /*
      1     Initialize table with single character strings
      2     P = first input character
      3     WHILE not end of input stream
      4          C = next input character
      5          IF P + C is in the string table
      6            P = P + C
      7          ELSE
      8            add P + C to the string table
      9            output the code for P
      10           P = C
      11    END WHILE
      12    output code for P
    */

    // Инициализирую словарь символами ASCII
    for (current_code = 0; current_code < 256; current_code++)
    {
        insert_char(tree, (unsigned char)current_code, current_code);
    }

    TrieNode *p;
    bool char_found = search_char(tree, c, &p);
    assert(char_found);
    size_t bytes_per_code = 2; // Начинаем с 16 бит

    while (!feof(input_file))
    {
        c = fgetc(input_file); // C = next input character

        TrieNode *p_plus_c;
        if (search_char(p, c, &p_plus_c) == true) // IF P + C is in the string table
        {
            p = p_plus_c; // P = P + C
        }
        else
        {
            insert_char(p, c, current_code++);                         // add P + C to the string table
            write_code_to_file(output_file, p->value, bytes_per_code); // output the code for P
            bool char_found = search_char(tree, c, &p);                // P = C
            assert(char_found);

            if (current_code == 0xFFFF) // Находим, где заканчивается место под 2 байта. 2^16 - 1 (65536)
            {
                bytes_per_code = 3; // Выставляем новое количество байт для нашего кода. 3 байта (24 бит)
                file_offset_24bit_codes = ftell(output_file);
            }
            else if (current_code == 0xFFFFFF) // Находим, где заканчивается место под 3 байта. 2^24 - 1 (16 777 215)
            {
                bytes_per_code = 4; // 4 байта (32 бита) (4 294 967 295)
                file_offset_32bit_codes = ftell(output_file);
            }
        }
    }

    write_code_to_file(output_file, p->value, bytes_per_code); // output the code for P

    // Вписываем в выходной файл места, где переходим на другое количество байт
    fseek(output_file, 0, SEEK_SET); // Ставим курсор в начало файла
    fwrite(&file_offset_24bit_codes, sizeof(long int), 1, output_file);
    fwrite(&file_offset_32bit_codes, sizeof(long int), 1, output_file);
    fwrite(&current_code, sizeof(uint32_t), 1, output_file); // Записываем в файл общее количество кодов

    fclose(output_file);
    fclose(input_file);
    free_trie_node(tree);


    return 0;
}