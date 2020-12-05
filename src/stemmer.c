#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/libstemmer.h"

int main()
{
	FILE *fp = fopen("../data/train.negative.csv", "r");
	printf("hellow world\n");
	char negativecsv[9090][200];
	char tokenNegativeCsv[9090][200];
	int countNegativeline = 0;
	if (fp == NULL)
	{
		printf("Wrong");
	}
	else
	{
		printf("it is ok\n");
	}
	//Read train.negative.csv

	while (fgets(negativecsv[countNegativeline], sizeof(negativecsv[countNegativeline]), fp) != NULL)
	{
		countNegativeline++;
	}

	for (int index = 0; index < countNegativeline; index++)
	{
		char *ptr = strtok(negativecsv[index], " \"+/_-?!,.;$:()*");
		while (ptr != NULL)
		{
			char space[2] = " ";
			strcat(tokenNegativeCsv[index], ptr);
			strcat(tokenNegativeCsv[index], space);
			ptr = strtok(NULL, " \"+/_-?!,.;$:()*");
		}
	}
	//Tokenization train.negative.csv and store tokenNegativeCsv array

	//Start Normalization train.negative.csv
	struct sb_stemmer *stemmer;
	char buf[1024] = "connection ";

	char afterStemNegative[9090][150];
	stemmer = sb_stemmer_new("english", 0x0);
	for (int index = 0; index < countNegativeline; index++)
	{
		char *ptr = strtok(tokenNegativeCsv[index], " ");
		while (ptr != NULL)
		{
			const char *s;
			s = sb_stemmer_stem(stemmer, ptr, strlen(ptr));
			char space[2] = " ";
			strcat(afterStemNegative[index], s);
			strcat(afterStemNegative[index], space);
			ptr = strtok(NULL, " ");
		}
	}
	sb_stemmer_delete(stemmer);
	//Finish Stemmer train.negative.csv

	//Store afterstemNegative array to afterStemmerNegative.txt
	FILE *fpAfterStemNe = fopen("afterStemmerNegative.txt", "a");
	for (int index = 0; index < countNegativeline; index++)
	{
		fputs(afterStemNegative[index], fpAfterStemNe);
	}
	fclose(fpAfterStemNe);
	fclose(fp);
	printf("난 행복해\n");

	//Start none-negative.csv tokenization
	FILE *fpnone = fopen("../data/train.non-negative.csv", "r");
	char nonnegativecsv[5570][200];
	char tokenNonNegativeCsv[5570][200];
	int countNonNegativeline = 0;
	if (fpnone == NULL)
	{
		printf("Wrong");
	}
	else
	{
		printf("it is ok\n");
	}
	while (fgets(nonnegativecsv[countNonNegativeline], sizeof(nonnegativecsv[countNonNegativeline]), fpnone) != NULL)
	{
		countNonNegativeline++;
	}

	for (int index = 0; index < countNonNegativeline; index++)
	{
		char *ptr = strtok(nonnegativecsv[index], " \"“”+/_-?!,.;$:()*");
		while (ptr != NULL)
		{
			char space[2] = " ";
			// printf("%s\n",ptr);
			strcat(tokenNonNegativeCsv[index], ptr);
			strcat(tokenNonNegativeCsv[index], space);
			ptr = strtok(NULL, " \"“”+/_-?!,.;$:()*");
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
			char space[2] = " ";
			strcat(nonnegativecsv[index], s);
			strcat(nonnegativecsv[index], space);
			ptr = strtok(NULL, " ");
		}
	}
	sb_stemmer_delete(stemmer2);
	//Finish Stemmer train.negative.csv

	//Store afterstemNegative array to afterStemmerNegative.txt
	FILE *fpAfterStemNonNe = fopen("afterStemmerNonNegative.txt", "a");
	for (int index = 0; index < countNonNegativeline; index++)
	{
		fputs(nonnegativecsv[index], fpAfterStemNonNe);
	}
	fclose(fpAfterStemNonNe);

	//Stopword removal and Vocabulary reduction

	FILE *fpAllwords = fopen("allwords", "r");
	char allword[850][30];
	int countword = 0;
	if (fpAllwords == NULL)
	{
		printf("Wrong");
	}
	else
	{
		printf("it is ok\n");
	}
	while (fgets(allword[countword], sizeof(allword[countword]), fp) != NULL)
	{
		allword[countword][strlen(allword[countword]) - 1] = '\0';
		countword++;
	}
	printf("allword :%s\n", allword[10]);
	FILE *fpNegativeRemove = fopen("RemoveNegative.txt", "a");
	printf("countNegativeline :%d\n", countNegativeline);

	FILE * removeNegative = fopen("afterStemmerNegative.txt", "r");
	char line[200] ;
	while(fgets(line, sizeof(line), removeNegative) != NULL)
	{
		
		char *ptr = strtok(line, " ");
		char space[2] = " ";
		int confirm = 0;
		char temp[200] = "";
		while (ptr != NULL)
		{

			// printf("ptr :%s!\n", ptr);
			for (int i = 0; i < countword; i++)
			{
				if (strcmp(ptr, allword[i]) == 0)
				{
					// printf("이거됨\n");
					// printf("allword :%s\n", allword[i]);
					confirm = 1;
					break;
				}
			}
			// printf("confirm :%d\n", confirm);
			if (confirm == 0)
			{
				strcat(temp, ptr);
				strcat(temp, space);
			}
			confirm = 0;
			ptr = strtok(NULL, " ");
			// printf("\n");
		}
		fputs(temp, fpNegativeRemove);
	}
	
	fclose(removeNegative);
	fclose(fpNegativeRemove);
	fclose(fpAllwords);
	return 0;
}
