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

void generateDefaultValues(Bakery *b, int *sink, int *oven, int *stand_mixer);
void adjustBakeryValues(int bakersCount, Bakery *b, int *sink, int *oven, int *stand_mixer);
void attachSharedMemorySegments(int* blah, int sharedMemoryID);
void getSharedMemorySegmentAndCheckIfValid(int *sharedMemoryID);

int main(int argv, char* argc[])
{
    struct sembuf sbuf;
    int semId = semget(IPC_PRIVATE, 1, 00600);
    semctl(semId, 0, SETVAL, 1);

    Recipe recipes[6] = {chocolate_cake, vanilla_cake, cupcakes_with_cherry, cupcakes_with_sprinkles, chocolate_chip_muffins, chocolate_chip_cookies};
    Bakery *bakery;

    // shared kitchen equipment
    int *sink, *oven, *stand_mixer;

    pid_t pid;
    int bakersCount;
    Baker bakers[bakersCount];

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

    int sinkMemoryID, ovenMemoryID, standMixerMemoryID;
    getSharedMemorySegmentAndCheckIfValid(&sinkMemoryID);
    getSharedMemorySegmentAndCheckIfValid(&ovenMemoryID);
    getSharedMemorySegmentAndCheckIfValid(&standMixerMemoryID);


    attachSharedMemorySegments(sink, sinkMemoryID);
    attachSharedMemorySegments(oven, ovenMemoryID);
    attachSharedMemorySegments(stand_mixer, standMixerMemoryID);

    printf("Enter number of bakersCount:\n");
    scanf("%d", &bakersCount);

    generateDefaultValues(bakery, sink, oven, stand_mixer);
    adjustBakeryValues(bakersCount, bakery, sink, oven, stand_mixer);

    // generates a user-defined number of bakersCount
    for (int i = 0; i < bakersCount; i++)
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
    shmctl(bakeryMemoryID, IPC_RMID, 0);

    return 0;
}

void generateDefaultValues(Bakery *b, int *sink, int *oven, int *stand_mixer)
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
    b->powdered_sugar = 100;

    *sink = 1;
    *oven = 1;
    *stand_mixer = 1;
}

void adjustBakeryValues(int bakersCount, Bakery *b, int *sink, int *oven, int *stand_mixer)
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

    sink += (int)equipmentMult;
    oven += (int)equipmentMult;
    stand_mixer += (int)equipmentMult;
}

int isAvailable(int isNeeded, int isAvailable)
{
    if (isAvailable - isNeeded > -1)
        return 1;

    return 0;
}

void bake(Recipe r, int ovenSemID) 
{
    wait_semaphore(ovenSemID);

    sleep(r.bake_time);

    signal_semaphore(ovenSemID);
}

void mixDryAndWetIngredients(Recipe r, int semId, Baker* baker, Bakery* bakery, int standMixerID) 
{    
    wait_semaphore(semId);

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
    
    signal_semaphore(semId);

    printf("Baker %s is mixing their dry ingredients.\n", baker->name);
    sleep(1);
    printf("Baker %s is done mixing their dry ingredients and is mixing their wet ingredients.\n", baker->name);
    sleep(1);

    wait_semaphore(standMixerID);

    printf("Baker %s is done mixing their dry and wet ingredients together.\n", baker->name);
    sleep(1);

    signal_semaphore(standMixerID);

    baker->dishCounter = baker->dishCounter + 2;
}

void wait_semaphore(int sem_id)
{
    struct sembuf sbuf;

    sbuf.sem_num = 0;
    sbuf.sem_op = -1;
    sbuf.sem_flg = 0;

    semop(sem_id, &sbuf, 1);
}

void signal_semaphore(int sem_id)
{
    struct sembuf sbuf;

    sbuf.sem_num = 0;
    sbuf.sem_op = 1;
    sbuf.sem_flg = 0;

    semop(sem_id, &sbuf, 1);
}

void attachSharedMemorySegments(int* blah, int sharedMemoryID)
{    

    blah = (int *) shmat(sharedMemoryID, 0, 0);
    if (blah == (void *)-1)
    {
        perror("Unable to attach\n");
        exit(1);
    }
}

void getSharedMemorySegmentAndCheckIfValid(int *sharedMemoryID)
{
    *sharedMemoryID = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | S_IRUSR | S_IWUSR);
    if (sharedMemoryID < 0)
    {
        perror("Unable to obtain shared memory\n");
        exit(1);
    }
}
