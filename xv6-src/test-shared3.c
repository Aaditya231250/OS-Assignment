// Test program for Part C: Shared Memory Test 3
// Tests if multiple children can access the same shared page.

#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    printf(1, "Part C: Shared Memory Test 3 (Multiple Children)\n");
    printf(1, "================================================\n");

    char *shared = (char*)mapshared();
    if (shared == 0) {
        printf(1, "ERROR: Parent mapshared failed!\n");
        exit();
    }
    printf(1, "Parent: Shared memory mapped at %p\n", shared);
    
    // Parent sets initial value
    shared[0] = 1;
    
    int pid1 = fork();
    
    if (pid1 == 0) {
        // Child 1
        sleep(5); // Let parent fork child 2
        char *child_shared = (char*)getshared();
        if (child_shared == 0) {
            printf(1, "ERROR: Child 1 getshared failed!\n");
            exit();
        }
        printf(1, "Child 1: Read value %d\n", (int)child_shared[0]);
        
        // Modify
        child_shared[0] = 10;
        printf(1, "Child 1: Wrote value 10\n");
        exit();
        
    } else {
        // Parent
        int pid2 = fork();
        
        if (pid2 == 0) {
            // Child 2
            sleep(10); // Let child 1 run first
            char *child_shared = (char*)getshared();
            if (child_shared == 0) {
                printf(1, "ERROR: Child 2 getshared failed!\n");
                exit();
            }
            printf(1, "Child 2: Read value %d\n", (int)child_shared[0]); // Should be 10
            
            // Modify
            child_shared[0] = 20;
            printf(1, "Child 2: Wrote value 20\n");
            exit();
            
        } else {
            // Parent
            // Wait for both children to exit
            wait();
            wait();
            
            printf(1, "Parent: Final value read is %d\n", (int)shared[0]); // Should be 20
            
            unmapshared();
        }
    }
    
    exit();
}