// Test program for Part A: Memory Information System Calls
// Tests: numvp(), numpp(), getptsize()

#include "types.h"
#include "stat.h"
#include "user.h"

void print_memory_info(char *label) {
    printf(1, "\n=== %s ===\n", label);
    printf(1, "Virtual pages (numvp): %d\n", numvp());
    printf(1, "Physical pages (numpp): %d\n", numpp());
    printf(1, "Page table size (getptsize): %d pages\n", getptsize());
}

int main(int argc, char *argv[]) {
    printf(1, "Part A: Memory Information Test\n");
    printf(1, "================================\n");
    
    // Initial memory state
    print_memory_info("Initial State");
    
    // Allocate some memory using sbrk
    printf(1, "\nAllocating 4096 bytes (1 page) using sbrk...\n");
    char *p1 = sbrk(4096);
    if (p1 == (char*)-1) {
        printf(1, "sbrk failed!\n");
        exit();
    }
    print_memory_info("After sbrk(4096)");
    
    // Allocate more memory
    printf(1, "\nAllocating 8192 bytes (2 pages) using sbrk...\n");
    char *p2 = sbrk(8192);
    if (p2 == (char*)-1) {
        printf(1, "sbrk failed!\n");
        exit();
    }
    print_memory_info("After sbrk(8192)");
    
    // Verify the counts make sense
    int vp = numvp();
    int pp = numpp();
    int pts = getptsize();
    
    printf(1, "\n=== Verification ===\n");
    printf(1, "Virtual pages should equal physical pages (no demand paging yet): ");
    if (vp == pp) {
        printf(1, "PASS\n");
    } else {
        printf(1, "FAIL (vp=%d, pp=%d)\n", vp, pp);
    }
    
    printf(1, "Page table size should be reasonable (>= 1): ");
    if (pts >= 1) {
        printf(1, "PASS\n");
    } else {
        printf(1, "FAIL (pts=%d)\n", pts);
    }
    
    printf(1, "\nTest completed!\n");
    exit();
}
