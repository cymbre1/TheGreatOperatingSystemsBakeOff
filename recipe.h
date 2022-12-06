#include <string.h>
#include <stdio.h>

typedef struct dry_ingredients
{
    int flour;
    int cocoa_powder;
    int baking_powder;
    int baking_soda;
    int salt;
} DryIngredients;

typedef struct wet_ingredients
{
    int butter;
    int sugar;
    int vanilla;
    int eggs;
    int milk;
} WetIngredients;

typedef struct recipe
{
    char name[50];
    // dry ingredients
    DryIngredients dry_ingredients;
    WetIngredients wet_ingredients;
    int bake_time;

} Recipe;

Recipe chocolate_cake = {"Chocolate Cake", (DryIngredients){ 210, 64, 4, 12, 6}, (WetIngredients){1, 400, 4, 2, 240}, 5};
Recipe vanilla_cake = {"Vanilla Cake", (DryIngredients){390, 0, 4, 0, 2}, (WetIngredients){170, 397, 14, 4, 340}, 5};
Recipe funfetti_cake = {"Vanilla Cake", (DryIngredients){390, 0, 4, 0, 2}, (WetIngredients){170, 397, 14, 4, 340}, 5};
Recipe cupcakes_with_cherry = {"Vanilla Cake", (DryIngredients){390, 0, 4, 0, 2}, (WetIngredients){170, 397, 14, 4, 340}, 4};
Recipe cupcakes_with_sprinkles = {"Vanilla Cake", (DryIngredients){390, 0, 4, 0, 2}, (WetIngredients){170, 397, 14, 4, 340}, 4};
Recipe chocolate_chip_muffins = {"Chocolate Chip Muffins", (DryIngredients){390, 0, 4, 0, 2}, (WetIngredients){170, 397, 14, 4, 340}, 4};
Recipe chocolate_chip_cookies = {"Chocolate Chip Cookies", (DryIngredients){390, 0, 4, 0, 2}, (WetIngredients){170, 397, 14, 4, 340}, 3};


char baker_names[21][13] = {"Cymbre", "Anna", "Chase", "Dr. Bobeldyk", "Wyatt", "Dale", 
                           "Mohammad", "Lucas", "Ethan", "Andrew", "Hanna", "Ari", "Donovan", 
                           "Michael", "Alex", "Raphael", "Trevor", "Steven", "Eldin", "Calob", "Cameron"};