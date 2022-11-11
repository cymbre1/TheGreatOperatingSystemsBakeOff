#include <string.h>
#include <stdio.h>

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

    // instructions
    int bake_time;
    
    enum garnish g;

} Recipe;

Recipe chocolate_cake = {};
Recipe vanilla_cake = {};
Recipe pineapple_cake = {};
Recipe pumpkin_cake = {};
Recipe cupcakes = {};
Recipe muffins = {};
Recipe cookies = {};

char baker_names[9][13] = {"Cymbre", "Anna", "Chase", "Dr. Bobeldyk", "Wyatt", "Dale", "Mohammad", "Lucas", "Ethan"};