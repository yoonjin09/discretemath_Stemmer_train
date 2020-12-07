#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/libstemmer.h"

int main()
{
	FILE *fp = fopen("../data/train.negative.csv", "r");
	char negativecsv[9090][200];
	char tokenNegativeCsv[9090][200];
	int countNegativeline = 0;
	int endline =0;
	char endl[2] = "\n";
	char space[2] = " ";
	if (fp == NULL)
	{
		printf("Can not read train.negative.csv");
	}
	else
	{
		printf("Read train.negative.csv\n");
	}
	//Read train.negative.csv

	while (fgets(negativecsv[countNegativeline], sizeof(negativecsv[countNegativeline]), fp) != NULL)
	{
		countNegativeline++;
	}

	for (int index = 0; index < countNegativeline; index++)
	{
		char *ptr = strtok(negativecsv[index], " +_“”-?'.!,;$()*");
		while (ptr != NULL)
		{
			strcat(tokenNegativeCsv[index], ptr);
			strcat(tokenNegativeCsv[index], space);
			ptr = strtok(NULL, " +_“”-?'.!,;$()*");
		}
	}
	//Tokenization train.negative.csv and store tokenNegativeCsv array

	//Start Normalization train.negative.csv
	struct sb_stemmer *stemmer;

	char afterStemNegative[9090][150];
	stemmer = sb_stemmer_new("english", 0x0);
	for (int index = 0; index < countNegativeline; index++)
	{
		char *ptr = strtok(tokenNegativeCsv[index], " ");
		while (ptr != NULL)
		{
			const char *s;
			s = sb_stemmer_stem(stemmer, ptr, strlen(ptr));
			
			strcat(afterStemNegative[index], s);
			strcat(afterStemNegative[index], space);
			ptr = strtok(NULL, " ");
		}
	}
	sb_stemmer_delete(stemmer);
	//Finish Stemmer train.negative.csv

	//Store afterstemNegative array to afterStemmerNegative.txt
	FILE *fpAfterStemNe = fopen("./alltext/afterStemmerNegative.txt", "a");
	for (int index = 0; index < countNegativeline; index++)
	{
		fputs(afterStemNegative[index], fpAfterStemNe);
	}
	fclose(fpAfterStemNe);
	fclose(fp);

	//Start none-negative.csv tokenization
	FILE *fpnone = fopen("../data/train.non-negative.csv", "r");
	char nonnegativecsv[5570][200];
	char tokenNonNegativeCsv[5570][200];
	int countNonNegativeline = 0;
	if (fpnone == NULL)
	{
		printf("Can not read train.non-negative.csv");
	}
	else
	{
		printf("Read train.non-negative.csv\n");
	}
	while (fgets(nonnegativecsv[countNonNegativeline], sizeof(nonnegativecsv[countNonNegativeline]), fpnone) != NULL)
	{
		countNonNegativeline++;
	}

	for (int index = 0; index < countNonNegativeline; index++)
	{
		char *ptr = strtok(nonnegativecsv[index], " “”.+'_-?!,;$()*");
		while (ptr != NULL)
		{
			
			strcat(tokenNonNegativeCsv[index], ptr);
			strcat(tokenNonNegativeCsv[index], space);
			ptr = strtok(NULL, " “”.+'_-?!,;$()*");
		}
	}
	fclose(fpnone);

	for (int i = 0; i < countNonNegativeline; i++)
	{
		nonnegativecsv[i][0] = '\0';
	}

	//Start Normalization train.negative.csv
	struct sb_stemmer *stemmer2;

	// char afterStemNonNegative[2000][150]; //처음 nonnegativecsv를 다시 사용한다.
	stemmer2 = sb_stemmer_new("english", 0x0);
	for (int index = 0; index < countNonNegativeline; index++)
	{
		char *ptr = strtok(tokenNonNegativeCsv[index], " ");
		while (ptr != NULL)
		{
			
			const char *s;
			s = sb_stemmer_stem(stemmer2, ptr, strlen(ptr));
			strcat(nonnegativecsv[index], s);
			strcat(nonnegativecsv[index], space);
			ptr = strtok(NULL, " ");
		}
	}
	sb_stemmer_delete(stemmer2);
	//Finish Stemmer train.negative.csv

	//Store afterstemNegative array to afterStemmerNegative.txt
	FILE *fpAfterStemNonNe = fopen("./alltext/afterStemmerNonNegative.txt", "a");
	for (int index = 0; index < countNonNegativeline; index++)
	{
		fputs(nonnegativecsv[index], fpAfterStemNonNe);
	}
	fclose(fpAfterStemNonNe);

	/*
	*	Stopword removal and Vocabulary reduction
	*/
	/*
	*	read all words
	*/

	FILE *fpAllwords = fopen("allwords", "r");
	char allword[850][30];
	int countword = 0;
	if (fpAllwords == NULL)
	{
		printf("Can not read allwords");
	}
	else
	{
		printf("Read allwords file\n");
	}
	while (fgets(allword[countword], sizeof(allword[countword]), fp) != NULL)
	{
		allword[countword][strlen(allword[countword]) - 1] = '\0';
		countword++;
	}
	fclose(fpAllwords);
	/*
	*	remove Stopword on negative.csv
	*/
	FILE *fpNegativeRemove = fopen("./alltext/RemoveNegative.txt", "a");
	FILE *removeNegative = fopen("./alltext/afterStemmerNegative.txt", "r");
	char line[200];
	while (fgets(line, sizeof(line), removeNegative) != NULL)
	{
		char *ptr = strtok(line, " ");
		int confirm = 0;
		char temp[200] = "";
		while (ptr != NULL)
		{	
			if (strcmp(&ptr[strlen(ptr) - 1], "\n") == 0){
				ptr[strlen(ptr) - 1] = '\0';
				endline = 1;
			}
				
			for (int i = 0; i < countword; i++)
			{
				if (strcmp(ptr, allword[i]) == 0)
				{
					confirm = 1;
					break;
				}
			}
			if (strchr(ptr, '^') || strchr(ptr, '\"') || strchr(ptr, '@') || strchr(ptr, ':') || strchr(ptr, '/') || strchr(ptr, '#') || strchr(ptr, '\'') || strchr(ptr, '&') || strchr(ptr, '1') || strchr(ptr, '2') || strchr(ptr, '3') || strchr(ptr, '4') || strchr(ptr, '5') || strchr(ptr, '6') || strchr(ptr, '7') || strchr(ptr, '8') || strchr(ptr, '9') || strchr(ptr, '0') != NULL)
			{
				confirm = 1;
			}
			if (isupper(ptr[0]) ==1 && isupper(ptr[1]) != 1)
			{
				confirm = 1;
			}
			if (confirm == 0)
			{
				strcat(temp, ptr);
				strcat(temp, space);
			}
			else
			{
				if (endline == 1)
					strcat(temp, endl);
			}
			endline = 0;
			confirm = 0;
			ptr = strtok(NULL, " ");
		}
		fputs(temp, fpNegativeRemove);
	}
	fclose(removeNegative);
	fclose(fpNegativeRemove);
	/*
	*	remove Stopword on non-negative.csv
	*/
	FILE *fpNonNegativeRemove = fopen("./alltext/RemoveNonNegative.txt", "a");
	FILE *removeNonNegative = fopen("./alltext/afterStemmerNonNegative.txt", "r");
	char nonline[200];
	int confirmcount = 0;

	while (fgets(nonline, sizeof(nonline), removeNonNegative) != NULL)
	{
		char *ptr = strtok(nonline, " ");
		int confirm = 0;
		char temp[200] = "";

		while (ptr != NULL)
		{
			if (strcmp(&ptr[strlen(ptr) - 1], "\n") == 0){
				ptr[strlen(ptr) - 1] = '\0';
				endline = 1;
			}
			for (int i = 0; i < countword; i++)
			{
				if (strcmp(ptr, allword[i]) == 0)
				{
					confirm = 1;
					break;
				}
			}
			if (strchr(ptr, '^') || strchr(ptr, '\"') || strchr(ptr, '@') || strchr(ptr, ':') || strchr(ptr, '/') || strchr(ptr, '#') || strchr(ptr, '\'') || strchr(ptr, '&') || strchr(ptr, '1') || strchr(ptr, '2') || strchr(ptr, '3') || strchr(ptr, '4') || strchr(ptr, '5') || strchr(ptr, '6') || strchr(ptr, '7') || strchr(ptr, '8') || strchr(ptr, '9') || strchr(ptr, '0')  != NULL)
			{
				confirm = 1;
				if (strcmp(&ptr[strlen(ptr) - 1], "\n") == 0)
					strcat(temp, "\n");
			}
			if (isupper(ptr[0]) ==1 && isupper(ptr[1]) != 1)
			{
				confirm = 1;
			}
			if (confirm == 0)
			{
				strcat(temp, ptr);
				strcat(temp, space);
			}
			else
			{
				if (endline == 1)
					strcat(temp, "\n");
			}
			endline=0;
			confirm = 0;
			ptr = strtok(NULL, " ");
		}
		fputs(temp, fpNonNegativeRemove);
	}

	fclose(removeNonNegative);

	fclose(fpNonNegativeRemove);

	printf("FINISH\n");
	return 0;
}
