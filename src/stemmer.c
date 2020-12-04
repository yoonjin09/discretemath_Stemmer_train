#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/libstemmer.h"

int 
main () 
{
	FILE * fp = fopen("../data/train.negative.csv","r");
  	printf("hellow world\n");
 	char negativecsv[9090][200];
 	char tokenNegativeCsv[9090][200];
 	int countNegativeline=0;
	if(fp == NULL){
   		printf("Wrong");
 	}
 	else{
   		printf("it is ok\n");
 	}
	//Read train.negative.csv

 	while(fgets(negativecsv[countNegativeline],sizeof(negativecsv[countNegativeline]), fp)!= NULL){
    		countNegativeline++;
  	}

  	for(int index=0; index< countNegativeline; index++){
    		char *ptr = strtok(negativecsv[index], " \"+/_-?!,.;$:()*");
    		while(ptr != NULL){
      			char space[2] = " ";
      			strcat(tokenNegativeCsv[index], ptr);
      			strcat(tokenNegativeCsv[index], space);
      			ptr = strtok(NULL, " \"+/_-?!,.;$:()*");
    		}
  	}
	//Tokenization train.negative.csv and store tokenNegativeCsv array

	//Start Normalization train.negative.csv
	struct sb_stemmer * stemmer ;
	char buf[1024] ="connection ";

 	char afterStemNegative[9090][200];
	stemmer = sb_stemmer_new("english", 0x0) ;
	for(int index = 0 ; index < countNegativeline; index++){
		char *ptr = strtok(tokenNegativeCsv[index], " ");
		while(ptr != NULL){
			const char * s ;
			s = sb_stemmer_stem(stemmer, ptr, strlen(ptr)) ;
      			char space[2] = " ";
      			strcat(afterStemNegative[index], s);
      			strcat(afterStemNegative[index], space);
			ptr = strtok(NULL, " ");
		}
	}	
	sb_stemmer_delete(stemmer) ;
	//Finish Stemmer train.negative.csv

	//Store afterstemNegative array to afterStemmerNegative.txt	
	FILE* fpAfterStemNe = fopen("afterStemmerNegative.txt","a");
	for(int index = 0; index < countNegativeline; index++){
		fputs(afterStemNegative[index], fpAfterStemNe);
	}
	fclose(fpAfterStemNe); 	
}
