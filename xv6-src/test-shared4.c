// Test program for Part C: Shared Memory Test 4
// Tests that a "grandchild" process cannot access the memory.

#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
    printf(1, "Part C: Shared Memory Test 4 (Grandchild Access)\n");
    printf(1, "================================================\n");

    char *shared = (char*)mapshared();
    if (shared == 0) {
        printf(1, "ERROR: Parent mapshared failed!\n");
        exit();
    }
    printf(1, "Parent: Shared memory mapped at %p\n", shared);
    
    int pid = fork();
    
    if (pid == 0) {
        // Child process
        char *child_shared = (char*)getshared();
        if (child_shared == 0) {
            printf(1, "ERROR: Child getshared failed!\n");
            exit();
        }
        printf(1, "Child: Successfully got shared memory.\n");

        // Now, fork a "grandchild"
        int gpid = fork();
        
        if (gpid == 0) {
            // Grandchild process
            printf(1, "Grandchild: Attempting to get shared memory...\n");
            char *g_shared = (char*)getshared();
            
            if (g_shared == 0) {
                printf(1, "Grandchild: getshared() correctly failed (returned 0).\n");
            } else {
                printf(1, "ERROR: Grandchild was able to get shared memory!\n");
            }
            exit();
            
        } else {
            // Child waits for grandchild
            wait();
            exit();
        }
        
    } else {
        // Parent process
        wait();
        printf(1, "Parent: Test complete.\n");
        unmapshared();
    }
    
    exit();
}