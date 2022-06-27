#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Cipher.h"

/* Construieste un multi-dictionar pe baza unui fisier text
 * Cheia (elem) unui nod va fi reprezentata de un cuvant din text
 * iar valoarea (info) va fi indexul de inceput al acelui cuvant
 * ignorand caracterele separatoare (",.? \n\r") - i.e. numarul
 * de caractere A-Z precedente
 *
 * E.g: THIS IS AN EXAMPLE
 *
 * 			IS (4)
 *		    / \
 * 	  (6) AN  THIS (0)
 *         \
 *	 	 EXAMPLE (8)
 * */
void buildTreeFromFile(char *fileName, TTree *tree)
{
	// Verificarea argumentelor
	if (fileName == NULL || tree == NULL)
	{
		return;
	}

	char comanda[300];
	char parametrii[100][100];
	int index = 0;
	FILE *f = fopen(fileName, "r");

	while (fgets(comanda, sizeof(comanda), f))
	{
		char *token = strtok(comanda, " .,?!\n\r"); // luam fiecare parametru
		int m = 0;
		while (token != NULL)
		{
			int lun = strlen(token);
			memcpy(parametrii[m], token, lun + 1);
			// trunchiam la 5 litere
			if (lun >= 5)
			{
				parametrii[m][5] = '\0';
			}
			else
			{
				parametrii[m][lun] = '\0';
			}
			insert(tree, parametrii[m], &index);
			// aflam indexul
			index = index + lun;
			token = strtok(NULL, " .,?!\n\r");
			m++;
		}
	}
	fclose(f);
}

/* Functie pentru afisarea unei chei de criptare
 * O cheie este reprezentata print-o serie de offset-uri
 *
 * E.g: key = [1, 2, 3, 4]
 * input text  = A    A    A    A
 *			   +1|  +2|  +3|  +4|
 * 				 V    V    V    V
 * output text = B    C    D    E
 *
 */
void printKey(char *fileName, Range *key)
{

	FILE *f = fopen(fileName, "w");

	if (key == NULL)
	{
		fprintf(f, "No key provided!\n");
		fclose(f);
		return;
	}

	fprintf(f, "Decryption key of length %d is:\n", key->size);

	// Afiseaza fiecare offset din cheie % 26 (lungimea
	// alfabetului A-Z)
	for (int i = 0; i < key->size; i++)
	{
		fprintf(f, "%d ", key->index[i] % 26);

		// Rand nou pentru lizibilitate
		if ((i + 1) % 10 == 0)
			fprintf(f, "\n");
	}

	fclose(f);
}

/* Returneaza cheia obtinuta de parcurgerea in ordine
 * crescatoare a nodurilor arborelui (parcurgand si listele
 * de duplicate)
 */
Range *inorderKeyQuery(TTree *tree)
{
	Range *aux = malloc(sizeof(Range));
	aux->index = malloc(100 * sizeof(int));
	aux->capacity = 100;
	aux->size = 0;
	// aflam minimul arborelui pentru a parcurge lista si adaugam info in range
	TreeNode *rad = minimum(tree->root);
	while (rad != NULL)
	{
		aux->index[aux->size] = *((int *)rad->info);
		aux->size++;
		rad = rad->next;
	}
	return aux;
}

/* Functie pentru extragerea cheii formate din valorile
 * nodurilor de la nivelul ce contine cel mai frecvent cuvant
 * (in cazul in care exista mai multe cuvinte cu numar maxim
 * de aparitii atunci se va considera primul nod dintre acestea conform
 * parcurgerii in inordine a arborelui)
 */
Range *levelKeyQuery(TTree *tree)
{
	return NULL;
}

/* Extragerea cheii din nodurile aflate intr-un anumit
 * domeniu de valori specificat
 */
Range *rangeKeyQuery(TTree *tree, char *q, char *p)
{
	Range *aux = malloc(sizeof(Range));
	aux->index = malloc(100 * sizeof(int));
	aux->capacity = 100;
	aux->size = 0;
	TreeNode *rad = minimum(tree->root);
	while (rad != NULL)
	{
		// procedam la fel ca la inorder, insa conditionam ca elementul sa se afle intre p si q
		if (tree->compare(rad->elem, q) == 1 && tree->compare(rad->elem, p) == -1)
		{
			aux->index[aux->size] = *((int *)rad->info);
			aux->size++;
		}

		rad = rad->next;
	}

	return aux;
}

void encrypt(char *inputFile, char *outputFile, Range *key)
{

	FILE *f_in = fopen(inputFile, "r");
	FILE *f_out = fopen(outputFile, "w");

	if (f_in == NULL)
		return;

	char *buff = (char *)malloc(BUFLEN + 1);
	char c;

	int idx = 0;

	while (fgets(buff, BUFLEN, f_in) != NULL)
	{

		for (int i = 0; i < strlen(buff); i++)
		{
			if (buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\r')
			{
				c = ((toupper(buff[i]) - 'A') + key->index[idx] % 26) % 26 + 'A';
				idx += 1;
			}
			else
				c = buff[i];

			fprintf(f_out, "%c", c);

			if (idx == key->size)
				idx = 0;
		}
	}

	free(buff);
	fclose(f_in);
	fclose(f_out);
}

void decrypt(char *inputFile, char *outputFile, Range *key)
{

	FILE *f_in = fopen(inputFile, "r");
	FILE *f_out = fopen(outputFile, "w");

	if (f_in == NULL)
		return;

	char *buff = (char *)malloc(BUFLEN + 1);
	char c;

	int idx = 0;

	while (fgets(buff, BUFLEN, f_in) != NULL)
	{

		for (int i = 0; i < strlen(buff); i++)
		{
			if (buff[i] != ' ' && buff[i] != '\n' && buff[i] != '\r')
			{
				c = ((toupper(buff[i]) - 'A') - (key->index[idx] % 26) + 26) % 26 + 'A';
				idx += 1;
			}
			else
				c = buff[i];

			fprintf(f_out, "%c", c);

			if (idx == key->size)
				idx = 0;
		}
	}

	free(buff);
	fclose(f_in);
	fclose(f_out);
}