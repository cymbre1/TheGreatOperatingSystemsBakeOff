#include <string.h>
#include <stdio.h>

enum garnish{ CHERRY, CHOCOLATE_CRUMBLES, SPRINKLES, FROSTED_FLOWERS, FROSTED_WORDS };

typedef struct recipe
{
    char name[25] = "";
    // dry ingredients
    int flour;
    int cocoa_powder;
    int baking_powder;
    int baking_soda;
    int salt;
    
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
    int powdered_sugar;
    int frosting_cocoa;

    // extras
    int blueberries;
    int pumpkin_puree;
    int rasins;
    int chocolate_chips;

    // instructions
    int bake_time;
    
    enum garnish g;

} Recipe;

Recipe chocolate_cake = {"Chocolate Cake", 210, 64, 4, 12, 6, 1, 400, 4, 2, 240, 2, 4, 0, 50, 400, 0, 0, 0, 0, 45, garnish::CHOCOLATE_CRUMBLES};
Recipe vanilla_cake =   {"Vanilla Cake", 390, 0, 4, 0, 2, 170, 397, 14, 4, 340, 2, 4, 0, 0, 400, 0, 0, 0, 0, 45, garnish::CHERRY};
Recipe pumpkin_cake = {"Pumpkin Cake", 250, 0, };
Recipe cupcakes = {};
Recipe chocolate_chip_muffins = {};
Recipe pumpkin_muffins = {};
Recipe blueberry_muffins = {};
Recipe chocolate_chip_cookies = {};
Recipe snickerdoodle_cookies = {};
Recipe oatmeal_rasin_cookies = {};


char baker_names[9][13] = {"Cymbre", "Anna", "Chase", "Dr. Bobeldyk", "Wyatt", "Dale", "Mohammad", "Lucas", "Ethan"};

Recipe recipes[10] = {chocolate_cake, vanilla_cake, pumpkin_cake, cupcakes, chocolate_chip_muffins, pumpkin_muffins, blueberry_muffins, chocolate_chip_cookies, snickerdoodle_cookies, oatmeal_rasin_cookies};