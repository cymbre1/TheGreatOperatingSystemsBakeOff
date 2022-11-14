#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "recipe.h"

int main(int argv, char* argc[])
{
    Recipe recipes[6] = {chocolate_cake, vanilla_cake, cupcakes_with_cherry, cupcakes_with_sprinkles, chocolate_chip_muffins, chocolate_chip_cookies};
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
            // Randomly assign the baker a name
            // Randomly assign the baker a recipe
            break;
        }
    }

    sleep(1);

    return 0;
}

void bake(Recipe r) 
{

}

void mix_dry_ingredients(Recipe r) 
{

}

void mix_wet_ingredients(Recipe r) 
{

}