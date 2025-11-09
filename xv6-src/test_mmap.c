// Test program for Part B: mmap with Demand Paging
// Tests: mmap() system call with lazy allocation

#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096

void print_memory_info(char *label) {
    printf(1, "\n--- %s ---\n", label);
    printf(1, "Virtual pages: %d, Physical pages: %d\n", numvp(), numpp());
}

int main(int argc, char *argv[]) {
    printf(1, "Part B: mmap with Demand Paging Test\n");
    printf(1, "====================================\n");
    
    print_memory_info("Initial state");
    
    int initial_vp = numvp();
    int initial_pp = numpp();
    
    // Test 1: Map one page
    printf(1, "\nTest 1: Mapping 1 page (4096 bytes)\n");
    char *addr1 = (char*)mmap(PGSIZE);
    
    if (addr1 == 0) {
        printf(1, "ERROR: mmap failed!\n");
        exit();
    }
    
    printf(1, "mmap returned address: %p\n", addr1);
    print_memory_info("After mmap(4096) - before access");
    
    // Check: virtual pages should increase, physical should not
    if (numvp() > initial_vp && numpp() == initial_pp) {
        printf(1, "✓ PASS: Virtual pages increased, physical did not (demand paging works!)\n");
    } else {
        printf(1, "✗ FAIL: Expected vp to increase and pp to stay same\n");
    }
    
    // Test 2: Access the mapped page (should trigger page fault)
    printf(1, "\nTest 2: Accessing mapped memory (should trigger page fault)\n");
    addr1[0] = 'A';  // Write to first byte
    addr1[PGSIZE-1] = 'Z';  // Write to last byte
    
    print_memory_info("After accessing mmap'd page");
    
    if (numpp() > initial_pp) {
        printf(1, "✓ PASS: Physical page allocated on demand!\n");
    } else {
        printf(1, "✗ FAIL: Physical page not allocated\n");
    }
    
    // Verify we can read what we wrote
    if (addr1[0] == 'A' && addr1[PGSIZE-1] == 'Z') {
        printf(1, "✓ PASS: Data written and read successfully\n");
    } else {
        printf(1, "✗ FAIL: Data verification failed\n");
    }
    
    // Test 3: Map multiple pages
    printf(1, "\nTest 3: Mapping 3 pages (12288 bytes)\n");
    int vp_before = numvp();
    int pp_before = numpp();
    
    char *addr2 = (char*)mmap(3 * PGSIZE);
    if (addr2 == 0) {
        printf(1, "ERROR: mmap failed!\n");
        exit();
    }
    
    printf(1, "mmap returned address: %p\n", addr2);
    print_memory_info("After mmap(12288) - before access");
    
    if (numvp() == vp_before + 3 && numpp() == pp_before) {
        printf(1, "✓ PASS: 3 virtual pages added, no physical pages yet\n");
    } else {
        printf(1, "✗ FAIL: Unexpected page counts\n");
    }
    
    // Test 4: Access only some of the mapped pages
    printf(1, "\nTest 4: Accessing only 2 out of 3 mapped pages\n");
    addr2[0] = 'X';  // Access first page
    addr2[2 * PGSIZE] = 'Y';  // Access third page (skip second)
    
    print_memory_info("After accessing 2 out of 3 pages");
    
    if (numpp() == pp_before + 2) {
        printf(1, "✓ PASS: Only accessed pages allocated physical memory!\n");
    } else {
        printf(1, "✗ FAIL: Expected 2 physical pages, got %d\n", numpp() - pp_before);
    }
    
    // Test 5: Invalid input handling
    printf(1, "\nTest 5: Testing invalid input\n");
    char *addr3 = (char*)mmap(0);  // Zero bytes
    if (addr3 == 0) {
        printf(1, "✓ PASS: mmap(0) correctly returned 0\n");
    } else {
        printf(1, "✗ FAIL: mmap(0) should return 0\n");
    }
    
    char *addr4 = (char*)mmap(-1);  // Negative
    if (addr4 == 0) {
        printf(1, "✓ PASS: mmap(-1) correctly returned 0\n");
    } else {
        printf(1, "✗ FAIL: mmap(-1) should return 0\n");
    }
    
    printf(1, "\n=== All tests completed ===\n");
    exit();
}
