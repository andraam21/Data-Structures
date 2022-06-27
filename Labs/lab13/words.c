#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char *Key;
typedef int Value;
typedef long (*HashFunction)(Key, long);

typedef struct Element
{
  Key key;
  Value value;
  struct Element *next;
} Element;

typedef struct HashTable
{
  Element **elements;
  long size;
  HashFunction hashFunction;
} HashTable;

void initHashTable(HashTable **h, long size, HashFunction f)
{
  *h = malloc(sizeof(HashTable));
  (*h)->size = size;
  (*h)->hashFunction = f;
  (*h)->elements = malloc((*h)->size * sizeof(Element));
}

int exists(HashTable *hashTable, Key key)
{
  for (int i = 0; i < hashTable->size; i++)
  {
    long hash = hashTable->hashFunction(key, hashTable->size);
    if (strcmp(hashTable->elements[hash]->key, key) == 0)
    {
      return 1;
    }
  }

  return 0;
}

Value get(HashTable *hashTable, Key key)
{
  for (int i = 0; i < hashTable->size; i++)
  {
    long hash = hashTable->hashFunction(key, hashTable->size);
    if (strcmp(hashTable->elements[hash]->key, key) == 0)
    {
      return hashTable->elements[hash]->value;
    }
  }

  return (Value)0;
}

void put(HashTable *hashTable, Key key, Value value)
{
  if (exists(hashTable, key))
  {
    long hash = hashTable->hashFunction(key, hashTable->size); // numarul bucketului e dat de cheia hash-uita
    for (int i = 0; i < hashTable->size; i++)
    {
      if (strcmp(hashTable->elements[hash]->key, key) == 0)
      {
        hashTable->elements[hash]->value = value;
      }
    }
  }
  else
  {
    long hash = hashTable->hashFunction(key, hashTable->size);
    //hashTable = realloc(hashTable, (hashTable->size + 1) * sizeof(Element));
    strcpy(hashTable->elements[hashTable->size]->key, key);
    hashTable->elements[hashTable->size]->value = value;
    hashTable->size++;
  }
}

void deleteKey(HashTable *hashTable, Key key)
{
  for (int i = 0; i < hashTable->size; i++)
  {
    Element *aux = hashTable->elements[i];
    if (strcmp(aux->next->key, key) == 0)
    {
      Element *prev = aux;
      aux = aux->next;
      prev->next = aux->next;
      free(aux);
      return;
    }
    else
    {
      aux = aux->next;
    }
  }
}

void print(HashTable *hashTable)
{
  for (int i = 0; i < hashTable->size; i++)
  {
    printf("Nod %d", i);
    printf("Cheie %s", hashTable->elements[i]->key);
    Element *aux = hashTable->elements[i];
    while (aux != NULL)
    {
      printf("Value %d", aux->value);
      aux = aux->next;
    }
  }
}

void freeHashTable(HashTable *hashTable)
{
  for (int i = 0; i < hashTable->size; i++)
  {
    free(hashTable->elements[i]);
  }
  free(hashTable->elements);
  free(hashTable);
}

long hash1(Key word, long size)
{
  long hash;
  for (int i = 0; i < strlen(word); i++)
  {
    hash = hash * 17 + size;
  }
  return hash;
}

int main(int argc, char *argv[])
{
  HashTable *hashTable;
  FILE *f1, *f2;
  char word[256];
  long hashSize, common;

  hashSize = atoi(argv[1]);
  f1 = fopen(argv[2], "r");
  f2 = fopen(argv[3], "r");

  initHashTable(&hashTable, hashSize, &hash1);

  // Cerinta 3

  // ...

  // Cerinta 4

  // ...

  printf("Common words: %ld\n", common);

  fclose(f1);
  fclose(f2);
  return 0;
}
