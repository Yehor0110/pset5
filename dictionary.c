/*
* check50--> https://sandbox.cs50.net/checks/e0bde36267104811a6bc2725e36ec9fa
* 
* Valentiukevych Yehor
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <stdbool.h>

#include "dictionary.h"

#define ALPHABET 27

typedef struct node
{
    bool word;
    struct node* child[ALPHABET];
}
node;

void unloader(node* current);

node* root;

// Размер инициализации словаря, чтобы сделать подсчет более удобным
unsigned int dictionarySize = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    node* cursor = root;
    
    /*
    пройти через каждую букву в слове, убедиться, что она в нижнем регистре,и убедиться,
    что это часть структуры и проверить, если конец слова
    */
    
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        //подобные нагрузки выяснить, где мы должны индексировать
        int index = (word[i] == '\'') ? ALPHABET - 1 : tolower(word[i]) - 'a';
        
        if (cursor->child[index] == NULL)
            return false;
        
        else
            cursor = cursor->child[index];
    }
    return cursor->word;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    FILE* file = fopen(dictionary, "r");
    
    if (file == NULL)
        return false;
        
    root = calloc(1, sizeof(node));
    if (root == NULL)
    {
        fclose(file);
        return false;
    }
    
    node* cursor = root;
    
    for (int c = fgetc(file); c != EOF; c = fgetc(file))
    {
        if (c != '\n')
        {
            // изменение буквы в соответствующие цифры
            int index = (c == '\'') ? ALPHABET - 1 : c - 'a';
            
            if (cursor->child[index] == NULL)
            {
                cursor->child[index] = calloc(1, sizeof(node));
                if (cursor->child[index] == NULL)
                {
                    unload();
                    fclose(file);
                    return false;
                }
            }
            
            cursor = cursor->child[index];
        }
        else
        {
            cursor->word = true;
            
            cursor = root;
            
            dictionarySize++;
        }
    }
    
    if (ferror(file))
    {
        unload();
        fclose(file);
        return false;
    }
    
    fclose(file);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    
    return dictionarySize;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    unloader(root);
    
    return true;
}

void unloader(node* current)
{
    //перебрать всех child, чтобы увидеть, если они указывают на что-нибудь и пойти туда, если они указывают
    for (int i = 0; i < ALPHABET; i++)
    {
        if (current->child[i] != NULL)
        {
            unloader(current->child[i]);
        }
    }
    
    free(current);
}
