#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct forBayes
{
    float po;
    int count;
};

int main(void)
{
    struct forBayes negativeword1[10000];
    for (int size = 0; size < 10000; size++)
    {
        memset(&negativeword1[size], 0, sizeof(struct forBayes));
    }

    struct forBayes nonnegativeword1[10000];
    for (int size = 0; size < 10000; size++)
    {
        memset(&nonnegativeword1[size], 0, sizeof(struct forBayes));
    }

    char negativeword[10000][30];
    char nonnegativeword[10000][30];
    int count = 0;
    int allcount = 0;
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
                    negativeword1[index].count++;
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
            allcount++;
            ptr = strtok(NULL, " ");
        }
    }
    fclose(fp1);
    fclose(fp2);

    int noncount = 0;
    int allnoncount = 0;
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
                    nonnegativeword1[index].count++;
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
            allnoncount++;
            ptr = strtok(NULL, " ");
        }
    }

    fclose(fp3);
    fclose(fp4);
    for (int i = 0; i < count; i++)
    {
        negativeword1[i].po = log((float)(negativeword1[i].count + 1) / (allnoncount + allcount));
    }
    for (int i = 0; i < noncount; i++)
    {
        nonnegativeword1[i].po = log((float)(nonnegativeword1[i].count + 1) / (allcount + allnoncount));
    }

    FILE *testnegative = fopen("../data/test.negative.csv", "r");
    FILE *confirmnegative = fopen("./result/confirmnegative.txt", "a");
    char testbuffer[300] = "";
    if (testnegative == NULL)
    {
        printf("Can not read test.negative.csv\n");
    }
    else
    {
        printf("Read test.negative.csv\n");
    }
    while (fgets(testbuffer, sizeof(testbuffer), testnegative) != NULL)
    {
        float negativepro = 0;
        float nonnegativepro = 0;
        char temp[200] = "";
        strcpy(temp, testbuffer);
        char *ptr = strtok(testbuffer, " ");
        while (ptr != NULL)
        {
            int confirm = 0;
            for (index = 0; index < count; index++)
            {
                char * tempptr = negativeword[index];

                if (strstr(ptr, negativeword[index]) != NULL)
                {
                    negativepro = negativepro + negativeword1[index].po;
                    break;
                }
            }
            for (index = 0; index < noncount; index++)
            {

                if (strstr(ptr, nonnegativeword[index]) != NULL)
                {
                    nonnegativepro = nonnegativepro + nonnegativeword1[index].po;
                    break;
                }
            }
            ptr = strtok(NULL, " ");
        }
        if (negativepro > nonnegativepro)
        {
            fputs(temp, confirmnegative);
        }
    }

    fclose(testnegative);
    fclose(confirmnegative);

    FILE *testnonnegative = fopen("../data/test.non-negative.csv", "r");
    FILE *confirmnonnegative = fopen("./result/confirmnonnegative.txt", "a");
    char nontestbuffer[300] = "";
    if (testnonnegative == NULL)
    {
        printf("Can not read test.non-negative.csv\n");
    }
    else
    {
        printf("Read test.non-negative.csv\n");
    }
    while (fgets(nontestbuffer, sizeof(nontestbuffer), testnonnegative) != NULL)
    {
        float negativepro = 0;
        float nonnegativepro = 0;
        char temp[200] = "";
        strcpy(temp, nontestbuffer);
        char *ptr = strtok(nontestbuffer, " ");
        while (ptr != NULL)
        {
            int confirm = 0;
            for (index = 0; index < count; index++)
            {
                if (strstr(ptr, negativeword[index]) != NULL)
                {
                    negativepro = negativepro + negativeword1[index].po;
                    break;
                }
            }
            for (index = 0; index < noncount; index++)
            {
                if (strstr(ptr, nonnegativeword[index]) != NULL)
                {
                    nonnegativepro = nonnegativepro + nonnegativeword1[index].po;
                    break;
                }
            }
            ptr = strtok(NULL, " ");
        }
        if (negativepro < nonnegativepro)
        {
            fputs(temp, confirmnonnegative);
        }
    }

    fclose(testnonnegative);
    fclose(confirmnonnegative);
    return 0;
}
