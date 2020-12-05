#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char negativeword[5000][30];
    char nonnegativeword[3500][30];
    int count = 0;
    int index = 0;

    FILE *fp1 = fopen("./alltext/RemoveNegative.txt", "r");
    FILE *fp2 = fopen("./alltext/negativewords.txt", "a");
    char buffer[200] = "";
    while (fgets(buffer, sizeof(buffer), fp1) != NULL)
    {
        char *ptr = strtok(buffer, " ");
        while (ptr != NULL)
        {

            int confirm = 0;
            for (index = 0; index < count; index++)
            {
               
                if (strcmp(ptr, negativeword[index]) == 0)
                {
                    confirm = 1;
                    break;
                }
            }
            if (confirm == 0)
            {
                fputs(ptr, fp2);
                strcpy(negativeword[count], ptr);
                if (strcmp(&ptr[strlen(ptr) - 1], "\n") != 0)
                {
                    fputs("\n", fp2);
                }
                count++;
            }

            ptr = strtok(NULL, " ");
        }
    }
    printf("count :%d\n",count);
    fclose(fp1);
    fclose(fp2);
    
    int noncount =0;

    FILE *fp3 = fopen("./alltext/RemoveNonNegative.txt", "r");
    FILE *fp4 = fopen("./alltext/nonnegativewords.txt", "a");
    while (fgets(buffer, sizeof(buffer), fp3) != NULL)
    {
        char *ptr = strtok(buffer, " ");
        while (ptr != NULL)
        {

            int confirm = 0;
            for (index = 0; index < noncount; index++)
            {
               
                if (strcmp(ptr, nonnegativeword[index]) == 0)
                {
                    confirm = 1;
                    break;
                }
            }
            if (confirm == 0)
            {
                fputs(ptr, fp4);
                strcpy(nonnegativeword[noncount], ptr);
                if (strcmp(&ptr[strlen(ptr) - 1], "\n") != 0)
                {
                    fputs("\n", fp4);
                }
                noncount++;
            }

            ptr = strtok(NULL, " ");
        }
    }
    printf("count :%d\n",noncount);
    fclose(fp3);
    fclose(fp4);
    return 0;
}