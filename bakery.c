#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "recipe.h"

int main(int argv, char* argc[])
{
    pid_t pid;

    // represents the number of bakers in the kitchen
    int bakers;

    printf("Enter number of bakers:\n");
    scanf("%d", &bakers);

    // generates a user-defined number of bakers
    for (int i = 0; i < bakers; i++)
    {
        if ((pid = fork()) != 0)    // parent process
        {
            printf("I am baker #%d!\n", i+1);
            break;
        }
    }

    sleep(1);

    return 0;
}

void bake_cake() 
{

}

void mix_dry_ingredients() 
{

}

void mix_wet_ingredients() 
{

}