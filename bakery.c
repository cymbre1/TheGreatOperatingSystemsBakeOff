#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

enum garnish{ CHERRY, CHOCOLATE_CRUMBLES, SPRINKLES, FROSTED_FLOWERS, FROSTED_WORDS };

typedef struct recipe
{
    // dry ingredients
    int flour;
    int baking_powder;
    int baking_soda;
    
    // wet ingredients
    int butter;
    int sugar;
    int vanilla;
    int eggs;
    int milk;

    // frosting ingredients
    int frosting_butter;
    int frosting_vanilla;
    int frosting_cream;
    

    enum garnish g;

} Recipe;

Recipe chocolate_cake = {};
Recipe vanilla_cake = {};
Recipe pineapple_cake = {};
Recipe pumpkin_cake = {};

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