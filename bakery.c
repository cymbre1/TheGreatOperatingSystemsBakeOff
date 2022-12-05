#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>

#include "ingredients.h"

void wait_semaphore(int semId);
void signal_semaphore(int semId);

void bake(int semId, int baker); 
void mixDryAndWetIngredients(Recipe r, Bakery* bakery, int baker, int dishes, int pantry, int standMixer);

void generateDefaultValues(Bakery *b);
void adjustBakeryValues(int bakersCount, Bakery *b);

int main(int argv, char* argc[])
{
    // baker info
    char name[40];
    int temp; //for testing
    int dishCount = 0;

    int pantry = semget(IPC_PRIVATE, 1, 00600);
    semctl(pantry, 0, SETVAL, 1);

    int oven = semget(IPC_PRIVATE, 1, 00600);
    semctl(oven, 0, SETVAL, 1);

    int sink = semget(IPC_PRIVATE, 1, 00600);
    semctl(sink, 0, SETVAL, 1);

    int standMixer = semget(IPC_PRIVATE, 1, 00600);
    semctl(standMixer, 0, SETVAL, 1);


    Recipe recipes[6] = {chocolate_cake, vanilla_cake, cupcakes_with_cherry, cupcakes_with_sprinkles, chocolate_chip_muffins, chocolate_chip_cookies};
    Bakery *bakery;

    pid_t pid;
    int bakersCount;

    // Allocate a shared memory segment
    int bakeryMemoryID = shmget(IPC_PRIVATE, sizeof(Bakery), IPC_CREAT | S_IRUSR | S_IWUSR);
    if (bakeryMemoryID < 0)
    {
        perror("Unable to obtain shared memory\n");
        exit(1);
    }
    
    bakery = (Bakery *) shmat(bakeryMemoryID, 0, 0);
    if (bakery == (void *)-1)
    {
        perror("Unable to attach\n");
        exit(1);
    }

    printf("Enter number of bakersCount:\n");
    scanf("%d", &bakersCount);

    generateDefaultValues(bakery);
    adjustBakeryValues(bakersCount, bakery);

    // generates a user-defined number of bakersCount
    for (int i = 0; i < bakersCount; i++)
    {
        if ((pid = fork()) != 0)    // parent process
        {
            // TODO: randomly assign baker name & recipe
            temp = i + 1;
            break;
        }
    }

    mixDryAndWetIngredients(chocolate_cake, bakery, temp, dishCount, pantry, standMixer);

    // Detach the shared memory segment
    shmdt(bakery); 

    // Deallocate the shared memory segment
    shmctl(bakeryMemoryID, IPC_RMID, 0);

    return 0;
}

void generateDefaultValues(Bakery *b)
{
    // these are just placeholder values for now
    b->flour = 390;
    b->cocoa_powder = 64;
    b->baking_powder = 4;
    b->baking_soda = 12;
    b->salt = 6;
    b->butter = 170;
    b->sugar = 400;
    b-> vanilla = 14;
    b->eggs = 4;
    b->milk = 340;
    b->cream = 100;
    b->powdered_sugar = 400;
}

void adjustBakeryValues(int bakersCount, Bakery *b)
{
    if (bakersCount <= 1)
        return;
    
    // increase ingredients by .75x for every baker
    double ingredientMult = .75 * (bakersCount - 1);
    // increase equipment by 1 for every 3 bakersCount
    double equipmentMult = (bakersCount - 1) / 3;

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
    b->powdered_sugar += b->powdered_sugar * ingredientMult;

    // TODO: how to do counting semaphore?
}

int isAvailable(int isNeeded, int isAvailable)
{
    if (isAvailable - isNeeded > -1)
        return 1;

    return 0;
}

void bake(int semId, int baker) 
{
    wait_semaphore(semId);

    // TODO: print name not number
    printf("Baker #%d is baking their pastry!\n", baker);
    sleep(5);
    printf("ding ding ding!\n");

    signal_semaphore(semId);
}

void mixDryAndWetIngredients(Recipe r, Bakery* bakery, int baker, int dishes, int pantry, int standMixer) 
{    
    wait_semaphore(pantry);

    if(!( isAvailable(r.dry_ingredients.flour, bakery->flour) &&
            isAvailable(r.dry_ingredients.cocoa_powder, bakery->cocoa_powder) && 
            isAvailable(r.dry_ingredients.baking_powder, bakery->baking_powder) && 
            isAvailable(r.dry_ingredients.baking_soda, bakery->baking_soda) && 
            isAvailable(r.dry_ingredients.salt, bakery->salt) && 
            isAvailable(r.wet_ingredients.butter, bakery->butter) &&
            isAvailable(r.wet_ingredients.sugar, bakery->sugar) &&
            isAvailable(r.wet_ingredients.vanilla, bakery->vanilla) &&
            isAvailable(r.wet_ingredients.eggs, bakery->eggs) &&
            isAvailable(r.wet_ingredients.milk, bakery->milk) ))
    {
        printf("Baker #%d doesn't have enough ingredients to finish their recipe.\n", baker);

        // Do something to terminate the process since it can't finish
        return;
    }

    // Remove dry ingredients from bakery
    
    bakery->flour = bakery->flour - r.dry_ingredients.flour;
    bakery->cocoa_powder = bakery->cocoa_powder - r.dry_ingredients.cocoa_powder;
    bakery->baking_powder = bakery->baking_powder - r.dry_ingredients.baking_powder;
    bakery->baking_soda = bakery->baking_soda - r.dry_ingredients.baking_soda;
    bakery->salt = bakery->salt - r.dry_ingredients.salt;
    

    // Remove wet ingredients from bakery
    bakery->butter = bakery->butter - r.wet_ingredients.butter;
    bakery->sugar = bakery->sugar - r.wet_ingredients.sugar;
    bakery->vanilla = bakery->vanilla - r.wet_ingredients.vanilla;
    bakery->eggs = bakery->eggs - r.wet_ingredients.eggs;
    bakery->milk = bakery->milk - r.wet_ingredients.milk;
    
    signal_semaphore(pantry);

    printf("Baker #%d is mixing their dry ingredients.\n", baker);
    sleep(1);
    printf("Baker #%d is done mixing their dry ingredients and is mixing their wet ingredients.\n", baker);
    sleep(1);

    wait_semaphore(standMixer);

    printf("Baker #%d is done mixing their dry and wet ingredients together.\n", baker);
    sleep(1);

    signal_semaphore(standMixer);

    dishes += 2;
}

void wait_semaphore(int semId)
{
    struct sembuf sbuf;

    sbuf.sem_num = 0;
    sbuf.sem_op = -1;
    sbuf.sem_flg = 0;

    semop(semId, &sbuf, 1);
}

void signal_semaphore(int semId)
{
    struct sembuf sbuf;

    sbuf.sem_num = 0;
    sbuf.sem_op = 1;
    sbuf.sem_flg = 0;

    semop(semId, &sbuf, 1);
}