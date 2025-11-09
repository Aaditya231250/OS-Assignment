// Test program for Part D: Copy-on-Write Fork
// Tests that pages are shared initially and copied only on write

#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096

void print_free_pages(char *label) {
    printf(1, "%s: Free pages = %d\n", label, getNumFreePages());
}

int main(int argc, char *argv[]) {
    printf(1, "Part D: Copy-on-Write Fork Test\n");
    printf(1, "================================\n");
    
    // Allocate some memory before fork
    printf(1, "\nAllocating 3 pages before fork...\n");
    char *mem = sbrk(3 * PGSIZE);
    if (mem == (char*)-1) {
        printf(1, "ERROR: sbrk failed!\n");
        exit();
    }
    
    // Write to all pages to ensure they're allocated
    mem[0] = 'A';
    mem[PGSIZE] = 'B';
    mem[2*PGSIZE] = 'C';
    
    print_free_pages("Before fork");
    int free_before_fork = getNumFreePages();
    
    printf(1, "\nForking...\n");
    int pid = fork();
    
    if (pid < 0) {
        printf(1, "ERROR: fork failed!\n");
        exit();
    }
    
    if (pid == 0) {
        // Child process
        printf(1, "\n=== CHILD PROCESS ===\n");
        print_free_pages("Child: Right after fork");
        
        int free_after_fork = getNumFreePages();
        
        // Check if CoW is working (pages should be shared, not copied)
        if (free_after_fork == free_before_fork) {
            printf(1, "✓ PASS: No pages consumed by fork (CoW working!)\n");
        } else {
            printf(1, "✗ FAIL: Pages consumed = %d (expected 0)\n", 
                   free_before_fork - free_after_fork);
        }
        
        printf(1, "\nChild: Reading from shared page (should not copy)...\n");
        (void)mem[0];
        print_free_pages("Child: After read");
        
        if (getNumFreePages() == free_after_fork) {
            printf(1, "✓ PASS: No pages consumed by read\n");
        } else {
            printf(1, "✗ FAIL: Read should not consume pages\n");
        }
        
        printf(1, "\nChild: Writing to first page (should trigger copy)...\n");
        mem[0] = 'X';
        print_free_pages("Child: After writing to page 1");
        
        if (getNumFreePages() == free_after_fork - 1) {
            printf(1, "✓ PASS: Exactly 1 page consumed by write (CoW triggered!)\n");
        } else {
            printf(1, "✗ FAIL: Expected 1 page consumed, got %d\n",
                   free_after_fork - getNumFreePages());
        }
        
        printf(1, "\nChild: Writing to third page (should trigger another copy)...\n");
        mem[2*PGSIZE] = 'Z';
        print_free_pages("Child: After writing to page 3");
        
        if (getNumFreePages() == free_after_fork - 2) {
            printf(1, "✓ PASS: Total 2 pages consumed\n");
        } else {
            printf(1, "✗ FAIL: Expected 2 pages consumed total\n");
        }
        
        // Don't write to middle page - parent will write to it
        printf(1, "\nChild: Exiting (page 2 never written by child)...\n");
        
        exit();
    } else {
        // Parent process
        sleep(20);  // Let child run first
        
        printf(1, "\n=== PARENT PROCESS ===\n");
        print_free_pages("Parent: After child exits");
        
        printf(1, "\nParent: Writing to middle page...\n");
        mem[PGSIZE] = 'Y';
        print_free_pages("Parent: After write");
        
        printf(1, "\nParent: Verifying memory isolation...\n");
        if (mem[0] == 'A') {
            printf(1, "✓ PASS: Page 1 not affected by child's write (isolated!)\n");
        } else {
            printf(1, "✗ FAIL: Page 1 was modified by child\n");
        }
        
        if (mem[PGSIZE] == 'Y') {
            printf(1, "✓ PASS: Page 2 has parent's value\n");
        } else {
            printf(1, "✗ FAIL: Page 2 has wrong value\n");
        }
        
        if (mem[2*PGSIZE] == 'C') {
            printf(1, "✓ PASS: Page 3 not affected by child's write\n");
        } else {
            printf(1, "✗ FAIL: Page 3 was modified by child\n");
        }
        
        wait();
        print_free_pages("Parent: Final state");
        
        printf(1, "\n=== Test completed ===\n");
    }
    
    exit();
}
