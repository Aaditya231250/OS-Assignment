// Test program for Part C: Shared Memory Test 1
// Simple parent-child communication via shared memory

#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    printf(1, "Part C: Shared Memory Test 1\n");
    printf(1, "============================\n");
    
    // Parent maps shared page
    char *shared = (char*)mapshared();
    if (shared == 0) {
        printf(1, "ERROR: mapshared failed!\n");
        exit();
    }
    
    printf(1, "Parent: Shared memory mapped at %p\n", shared);
    
    // Parent writes to shared memory
    shared[0] = 42;
    printf(1, "Parent: Wrote 42 to shared memory\n");
    
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
        
        printf(1, "child %d\n", (int)child_shared[0]);
        
        // Child modifies shared memory
        child_shared[0] = 43;
        
        exit();
    } else {
        // Parent process
        wait();
        
        printf(1, "parent %d\n", (int)shared[0]);
        
        // Cleanup
        if (unmapshared() < 0) {
            printf(1, "ERROR: unmapshared failed!\n");
        }
    }
    
    exit();
}
