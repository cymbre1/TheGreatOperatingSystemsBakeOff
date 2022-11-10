#include <stdio.h>
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



int main(int argv, char* argc)
{
    // fork off x processes for bakers


}