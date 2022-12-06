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
    int finished;
} Pantry;

typedef struct baker
{
    int stationNumber;
    Recipe recipe;
    int dishCounter;
    char name[40];
} Baker;