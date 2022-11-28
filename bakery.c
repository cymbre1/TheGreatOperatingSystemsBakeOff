#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>

#include "recipe.h"
#include "ingredients.h"

void generateDefaultValues(Bakery *b);
void adjusBakeryValues(int bakers, Bakery *b);

int main(int argv, char* argc[])
{
    Recipe recipes[6] = {chocolate_cake, vanilla_cake, cupcakes_with_cherry, cupcakes_with_sprinkles, chocolate_chip_muffins, chocolate_chip_cookies};
    Bakery *bakery;
    pid_t pid;
    int bakers;
    int dishes;

    // Allocate a shared memory segment
    int sharedMemoryID = shmget(IPC_PRIVATE, sizeof(Bakery), IPC_CREAT | S_IRUSR | S_IWUSR);
    if (sharedMemoryID < 0)
    {
        perror("Unable to obtain shared memory\n");
        exit(1);
    }

    // Attach the shared memory segment
    bakery = (Bakery *) shmat(sharedMemoryID, 0, 0);
    if (bakery == (void *)-1)
    {
        perror("Unable to attach\n");
        exit(1);
    }

    printf("Enter number of bakers:\n");
    scanf("%d", &bakers);

    generateDefaultValues(bakery);
    adjusBakeryValues(bakers, bakery);

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

    // Detach the shared memory segment
    shmdt(bakery); 

    // Deallocate the shared memory segment
    shmctl(sharedMemoryID, IPC_RMID, 0);

    return 0;
}

void generateDefaultValues(Bakery *b)
{
    // these are just placeholder values for now
    b->flour = 100;
    b->cocoa_powder = 100;
    b->baking_powder = 100;
    b->baking_soda = 100;
    b->salt = 100;
    b->butter = 100;
    b->sugar = 100;
    b-> vanilla = 100;
    b->eggs = 100;
    b->milk = 100;
    b->cream = 100;
    b->powdered_suga = 100;

    b->sink = 1;
    b->oven = 1;
    b->stand_mixer = 1;
}

void adjusBakeryValues(int bakers, Bakery *b)
{
    if (bakers <= 1)
        return;
    
    // increase ingredients by .75x for every baker
    double ingredientMult = .75 * (bakers - 1);
    // increase equipment by 1 for every 3 bakers
    double equipmentMult = (bakers - 1) / 3;

    b->flour += b->flour * ingredientMult;
    b->cocoa_powder += b->cocoa_powder * ingredientMult;
    b->baking_powder += b->baking_powder * ingredientMult;
    b->baking_soda += b->baking_soda * ingredientMult;
    b->salt += b->salt * ingredientMult;
    b->butter += b->butter * ingredientMult;
    b->sugar += b->sugar * ingredientMult;
    b->vanilla += b->vanilla * ingredientMult;
    b->eggs += b->eggs * ingredientMult;
    b->milk += b->milk * ingredientMult;
    b->cream += b->cream * ingredientMult;
    b->powdered_suga += b->powdered_suga * ingredientMult;

    b->sink += equipmentMult;
    b->oven += equipmentMult;
    b->stand_mixer += equipmentMult;
}

int isAvailable(int isNeeded, int isAvailable)
{
    if (isAvailable - isNeeded > -1)
        return 1;

    return 0;
}

void bake(Recipe r) 
{

}

void mixDryIngredients(Recipe r) 
{

}

void mixWetIngredients(Recipe r) 
{

}