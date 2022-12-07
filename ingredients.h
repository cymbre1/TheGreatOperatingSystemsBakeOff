#include "recipe.h"

typedef struct pantry
{
    // ingredients
    int flour;
    int cocoa_powder;
    int baking_powder;
    int baking_soda;
    int salt;
    int butter;
    int sugar;
    int vanilla;
    int eggs;
    int milk;
    int cream;
    int powdered_sugar;

    // used to determine process termination
    int finished; 
} Pantry;