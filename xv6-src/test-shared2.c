// Test program for Part C: Shared Memory Test 2
// Multiple reads/writes between parent and child

#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    printf(1, "Part C: Shared Memory Test 2\n");
    printf(1, "============================\n");
    
    // Parent maps shared page
    char *shared = (char*)mapshared();
    if (shared == 0) {
        printf(1, "ERROR: mapshared failed!\n");
        exit();
    }
    
    printf(1, "Parent: Shared memory mapped at %p\n", shared);
    
    // Parent writes initial value
    shared[0] = 42;
    printf(1, "Parent: Initial value = 42\n");
    
    int pid = fork();
    
    if (pid < 0) {
        printf(1, "ERROR: fork failed!\n");
        exit();
    }
    
    if (pid == 0) {
        // Child process
        char *child_shared = (char*)getshared();
        if (child_shared == 0) {
            printf(1, "ERROR: Child getshared failed!\n");
            exit();
        }
        
        // First read
        printf(1, "child %d\n", (int)child_shared[0]);
        
        // Child modifies
        child_shared[0] = 43;
        
        // Let parent read
        sleep(10);
        
        // Second read (after parent writes)
        printf(1, "child again %d\n", (int)child_shared[0]);
        
        exit();
    } else {
        // Parent process
        sleep(5);
        
        // Parent reads child's modification
        printf(1, "parent %d\n", (int)shared[0]);
        
        // Parent modifies again
        shared[0] = 54;
        
        wait();
        
        // Parent reads final value
        printf(1, "parent again %d\n", (int)shared[0]);
        
        // Cleanup
        if (unmapshared() < 0) {
            printf(1, "ERROR: unmapshared failed!\n");
        }
    }
    
    exit();
}
