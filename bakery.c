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

int main(int argv, char* argc[])
{
    Recipe recipes[6] = {chocolate_cake, vanilla_cake, cupcakes_with_cherry, cupcakes_with_sprinkles, chocolate_chip_muffins, chocolate_chip_cookies};
    BakeryIngredients ingredients;
    BakeryIngredients equipment;
    pid_t pid;
    int bakers;

    // Allocate a shared memory segment
    int sharedMemoryID = shmget(IPC_PRIVATE, sizeof(BakeryIngredients) + sizeof(BakeryEquipment), IPC_CREAT | S_IRUSR | S_IWUSR);
    if (sharedMemoryID < 0)
    {
        perror("Unable to obtain shared memory\n");
        exit(1);
    }

    // Attach the shared memory segment
    long int *sharedMemoryPointer = shmat(sharedMemoryID, 0, 0);
    if (sharedMemoryPointer == (void *)-1)
    {
        perror("Unable to attach\n");
        exit(1);
    }

    // TEMPORARY: testing that shared memory works
    ingredients.baking_powder = 10;
    sharedMemoryPointer[0] = ingredients.baking_powder;

    printf("Enter number of bakers:\n");
    scanf("%d", &bakers);

    // generates a user-defined number of bakers
    for (int i = 0; i < bakers; i++)
    {
        if ((pid = fork()) != 0)    // parent process
        {
            printf("I am baker #%d!\n", i+1);

            // TEMPORARY: testing that shared memory works
            printf("testing: %ld\n", sharedMemoryPointer[0]);
            sharedMemoryPointer[0] += 10;
            // Randomly assign the baker a name
            // Randomly assign the baker a recipe
            break;
        }
    }

    sleep(1); // 

    // Detach the shared memory segment
    shmdt(sharedMemoryPointer); 

    // Deallocate the shared memory segment
    shmctl(sharedMemoryID, IPC_RMID, 0);

    return 0;
}

void generateDefaultValues(int bakers)
{

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