//Author: Sneha Sirnam
//NetId: s_s1078
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
/* Global Variables shared among threads*/
static FILE* fp;
static int seed = 12;
static int dealerscore;
static int scoreofallplayers[8] = { 0,0,0,0,0,0,0,0 };
static int sumA=0,sumB=0,sumC=0,sumD=0;
static bool dealerFlag=false;
/* mutex and semaphore to sync threads*/
static pthread_mutex_t lock;
static pthread_cond_t ok = PTHREAD_COND_INITIALIZER;
/* Throw dice function throws two dice and save their results in scoreofallplayer with respect to player positions*/
void* throwdice(void* n)
{
    int diceroll1, diceroll2;
    char playername;
    pthread_mutex_lock(&lock);
    int player = (int)n;
    time_t t;
    if (player == 0)
    {
        playername = 'A';
    }
    else if (player == 1)
    {
        playername = 'B';
    }
    else if (player == 2)
    {
        playername = 'C';
    }
    else if (player == 3)
    {
        playername = 'D';
    }
    else
    {
        playername = 'K';
    }


    if (playername != 'K')
    {
        int temp=0;
        diceroll1 = (rand() % 6)+1;
        diceroll2 = (rand() % 6)+1;
        scoreofallplayers[(player * 2)] = diceroll1;
        scoreofallplayers[(player * 2) + 1] = diceroll2;
        temp=diceroll1+diceroll2;
        if(playername == 'A')
            sumA=temp;
        if(playername == 'B')
            sumB=temp;
        if(playername == 'C')
            sumC=temp;
        if(playername == 'D')
            sumD=temp;

        // printf("Player %c gets %d and %d with a sum %d \n", playername, diceroll1, diceroll2, diceroll1 + diceroll2);

        /* write text into the file stream*/
        fputs("Player ", fp);
        fputc(playername, fp);
        fputs(" gets ", fp);
        char str[50];
        sprintf(str, "%d", diceroll1);
        fputs(str, fp);

        fputs(" and ", fp);
        sprintf(str, "%d", diceroll2);
        fputs(str, fp);
        fputs(" with a sum ", fp);
        sprintf(str, "%d", diceroll1 + diceroll2);
        fputs(str, fp);
        fputs("\n", fp);


    }
    else
    {
        dealerFlag=(sumA!=sumC && sumB!=sumD);
        if(!dealerFlag)
        {
            if(sumA==sumC)
            {
                printf("DEALER: The winning team is A and C");
                fputs(" DEALER: The winning team is A and C ", fp);

            }
            else if(sumB==sumD)
            {
                printf("DEALER: The winning team is B and D");
                fputs(" DEALER: The winning team is B and D ", fp);

            }

        }

    }
    pthread_cond_signal(&ok);
    pthread_mutex_unlock(&lock);

}
/* Driver Program*/
int main(int argc, char* argv[])
{
    pthread_t callThd[4];
    pthread_mutex_init(&lock, NULL);
    time_t t;
    /* Intializes random number generator */
    srand((unsigned)time(&t));

    printf("Program Name Is: %s", argv[0]);
    if (argc == 1)
    {
        printf("\nNo Seed Paramter is Passed.\n");
        return 0;
    }
    else if (argc >= 2)
    {

        printf("\nSeed Value is: %s\n", argv[1]);
    }

    /* open or create the logfile for writing*/
    fp = fopen("logfile.txt", "w");


    // Let us create five threads
    do
    {
        for (int i = 0; i < 4; i++)
        {
            sleep(1);
            pthread_create(&callThd[i], NULL, throwdice, (void*)i);
        }

        for (int j = 0; j < 4; j++)
        {
            pthread_join(callThd[j], NULL);
        }

        pthread_create(&callThd[4], NULL, throwdice, (void*)4);
        pthread_join(callThd[4], NULL);





    }
    while(dealerFlag);
    char playername;
    for (int k = 0; k < 4; k++)
    {
        if (k == 0)
        {
            playername = 'A';
        }
        else if (k == 1)
        {
            playername = 'B';
        }
        else if (k == 2)
        {
            playername = 'C';
        }
        else if (k == 3)
        {
            playername = 'D';
        }
    }
    printf("\n");
    printf("***************************************\n");
    printf("Score Board\n");
    printf("****************************************\n");
    for (int k = 0; k < 4; k++)
    {
        if (k == 0)
        {
            playername = 'A';
        }
        else if (k == 1)
        {
            playername = 'B';
        }
        else if (k == 2)
        {
            playername = 'C';
        }
        else if (k == 3)
        {
            playername = 'D';
        }
        printf("Player %c: %d, %d \n", playername, scoreofallplayers[(k * 2)], scoreofallplayers[(k * 2) + 1]);
    }
    /* close the file*/
    fclose(fp);
    /* Closing all the threads*/
    pthread_exit(NULL);
    return 0;
}
