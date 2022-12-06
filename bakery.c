/*
 * The Great Operating Systems Bake Off
 * 
 * Authors: Anna Carvalho & Cymbre Spoehr
 * Date: 12/06/22
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <time.h>

#include "ingredients.h"

int isAvailable(int isNeeded, int isAvailable);
void getIngredients(Recipe r, Pantry *pantry, char bakerName[], char recipeName[], int* finished, int pantrySem);
void useStandMixer(char bakerName[], int standMixerSem);
void bake(char bakerName[], char recipeName[], int ovenSem);
void finish(Pantry *pantry, int pantrySem);

void generateDefaultValues(Pantry *p);
void adjustBakeryValues(int bakersCount, Pantry *p);

void wait_semaphore(int semId);
void signal_semaphore(int semId);
void sigHandler(int);

int main(int argv, char *argc[])
{
    signal(SIGINT, sigHandler);

    pid_t pid;

    // initialize semaphores
    int pantrySem = semget(IPC_PRIVATE, 1, 00600);
    semctl(pantrySem, 0, SETVAL, 1);

    int ovenSem = semget(IPC_PRIVATE, 1, 00600);
    semctl(ovenSem, 0, SETVAL, 1);

    int standMixerSem = semget(IPC_PRIVATE, 1, 00600);
    semctl(standMixerSem, 0, SETVAL, 1);

    // baker info
    char name[13];
    Recipe recipe;
    int id = 0;
    int finished = 0;

    Recipe recipes[7] = {chocolate_cake, vanilla_cake, funfetti_cake, cupcakes_with_cherry, cupcakes_with_sprinkles, chocolate_chip_muffins, chocolate_chip_cookies};
    Pantry *pantry;
    int bakersCount;

    // allocate a shared memory segment
    int bakeryMemoryID = shmget(IPC_PRIVATE, sizeof(Pantry), IPC_CREAT | S_IRUSR | S_IWUSR);
    if (bakeryMemoryID < 0)
    {
        perror("Unable to obtain shared memory\n");
        exit(1);
    }

    // attach pantry to shared memory segment
    pantry = (Pantry *)shmat(bakeryMemoryID, 0, 0);
    if (pantry == (void *)-1)
    {
        perror("Unable to attach\n");
        exit(1);
    }

    // get number of bakers
    printf("Enter number of bakersCount:\n");
    scanf("%d", &bakersCount);

    generateDefaultValues(pantry);
    adjustBakeryValues(bakersCount, pantry);

    // generate a user-defined number of bakers
    for (int i = 1; i < bakersCount; i++)
    {
        if ((pid = fork()) != 0) // parent process
        {
            break;
        }
        id = i;
    }

    // randomly assign baker name & recipe
    srand(time(NULL) + id);
    recipe = recipes[rand() % 7];
    strcpy(name, baker_names[rand() % 13]);

    // make the recipe!
    getIngredients(chocolate_cake, pantry, name, recipe.name, &finished, pantrySem);
    if (finished == 0)
        useStandMixer(name, standMixerSem);
    bake(name, recipe.name, ovenSem);
    finish(pantry, pantrySem);

    // parent process manages memory
    if (id == 0)
    {
        while (pantry->finished < bakersCount) {}

        // detach the shared memory segment
        shmdt(pantry);

        // deallocate the shared memory segment
        shmctl(bakeryMemoryID, IPC_RMID, 0);

        // deallocate semaphores
        semctl(pantrySem, 0, IPC_RMID);
        semctl(ovenSem, 0, IPC_RMID);
        semctl(standMixerSem, 0, IPC_RMID);
    }

    return 0;
}

/*
 * Method that checks if there are enough ingredients
 * for a recipe
 */
int isAvailable(int isNeeded, int isAvailable)
{
    if (isAvailable - isNeeded > -1)
        return 1;

    return 0;
}

/*
 * Method that simulates bakers gathering their ingredients
 */
void getIngredients(Recipe r, Pantry *pantry, char bakerName[], char recipeName[], int* finished, int pantrySem)
{
    wait_semaphore(pantrySem);

    printf("%s is in the pantry\n\n", bakerName);
    
    sleep(1);

    if (!(isAvailable(r.dry_ingredients.flour, pantry->flour) &&
          isAvailable(r.dry_ingredients.cocoa_powder, pantry->cocoa_powder) &&
          isAvailable(r.dry_ingredients.baking_powder, pantry->baking_powder) &&
          isAvailable(r.dry_ingredients.baking_soda, pantry->baking_soda) &&
          isAvailable(r.dry_ingredients.salt, pantry->salt) &&
          isAvailable(r.wet_ingredients.butter, pantry->butter) &&
          isAvailable(r.wet_ingredients.sugar, pantry->sugar) &&
          isAvailable(r.wet_ingredients.vanilla, pantry->vanilla) &&
          isAvailable(r.wet_ingredients.eggs, pantry->eggs) &&
          isAvailable(r.wet_ingredients.milk, pantry->milk)))
    {
        printf("%s doesn't have enough ingredients to finish their %s. %s has left the kitchen.\n\n", bakerName, recipeName, bakerName);

        signal_semaphore(pantrySem);

        *finished = 1;

        return;
    }

    // Remove dry ingredients from pantry
    pantry->flour = pantry->flour - r.dry_ingredients.flour;
    pantry->cocoa_powder = pantry->cocoa_powder - r.dry_ingredients.cocoa_powder;
    pantry->baking_powder = pantry->baking_powder - r.dry_ingredients.baking_powder;
    pantry->baking_soda = pantry->baking_soda - r.dry_ingredients.baking_soda;
    pantry->salt = pantry->salt - r.dry_ingredients.salt;

    // Remove wet ingredients from pantry
    pantry->butter = pantry->butter - r.wet_ingredients.butter;
    pantry->sugar = pantry->sugar - r.wet_ingredients.sugar;
    pantry->vanilla = pantry->vanilla - r.wet_ingredients.vanilla;
    pantry->eggs = pantry->eggs - r.wet_ingredients.eggs;
    pantry->milk = pantry->milk - r.wet_ingredients.milk;

    printf("%s has left the pantry\n\n", bakerName);

    signal_semaphore(pantrySem);
}

/*
 * Method that simulares baker using the stand mixer
 */
void useStandMixer(char bakerName[], int standMixerSem)
{
    wait_semaphore(standMixerSem);

    printf("%s is now using the stand mixer\n\n", bakerName);
    
    sleep(4);

    signal_semaphore(standMixerSem);

    printf("%s is done using the stand mixer.\n\n", bakerName);
}

/*
 * Method that simulares baker using the oven
 */
void bake(char bakerName[], char recipeName[], int ovenSem)
{
    wait_semaphore(ovenSem);

    printf("%s is baking their %s!\n\n", bakerName, recipeName);

    sleep(5);

    printf("ding ding ding! The %s is done!\n\n", recipeName);

    signal_semaphore(ovenSem);
}

/*
 * Method that is called when the baker is finished;
 * is used to determine when to terminate processes
 */
void finish(Pantry *pantry, int pantrySem)
{
    wait_semaphore(pantrySem);
    pantry->finished++;
    signal_semaphore(pantrySem);
}

/*
 * Method that generates default ingredient values
 */
void generateDefaultValues(Pantry *p)
{
    p->flour = 390;
    p->cocoa_powder = 64;
    p->baking_powder = 4;
    p->baking_soda = 12;
    p->salt = 6;
    p->butter = 170;
    p->sugar = 400;
    p->vanilla = 14;
    p->eggs = 4;
    p->milk = 340;
    p->cream = 100;
    p->powdered_sugar = 400;
    p->finished = 0;
}

/*
 * Method that adjusts ingredient values based on the number
 * of bakers
 */
void adjustBakeryValues(int bakersCount, Pantry *p)
{
    if (bakersCount <= 1)
        return;

    // increase ingredients by .85x for every baker
    double ingredientMult = .85 * (bakersCount - 1);
    // increase equipment by 1 for every 3 bakersCount
    double equipmentMult = (bakersCount - 1) / 3;

    p->flour += p->flour * ingredientMult;
    p->cocoa_powder += p->cocoa_powder * ingredientMult;
    p->baking_powder += p->baking_powder * ingredientMult;
    p->baking_soda += p->baking_soda * ingredientMult;
    p->salt += p->salt * ingredientMult;
    p->butter += p->butter * ingredientMult;
    p->sugar += p->sugar * ingredientMult;
    p->vanilla += p->vanilla * ingredientMult;
    p->eggs += p->eggs * ingredientMult;
    p->milk += p->milk * ingredientMult;
    p->cream += p->cream * ingredientMult;
    p->powdered_sugar += p->powdered_sugar * ingredientMult;
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

void sigHandler(int sigNum)
{
    exit(0);
}