# Implementation Notes

## Project Structure

This document tracks implementation decisions and progress for each part of the memory management project.

---

## Part A: Memory Information System Calls

### System Calls to Implement
1. `numvp()` - Returns number of virtual pages
2. `numpp()` - Returns number of physical pages  
3. `getptsize()` - Returns page table size in pages

### Implementation Steps

#### 1. Add System Call Declarations
- [ ] Add system call numbers to `syscall.h`
- [ ] Add system calls to `syscall.c` syscalls array
- [ ] Add user-space declarations to `user.h`
- [ ] Add assembly wrappers to `usys.S`

#### 2. Implement System Calls in `sysproc.c`
- [ ] `sys_numvp()` - Count virtual pages up to program break
- [ ] `sys_numpp()` - Walk page table, count valid PTEs
- [ ] `sys_getptsize()` - Count page directory + page table pages

#### 3. Helper Functions in `vm.c`
- [ ] Function to walk page table and count physical pages
- [ ] Function to count page table size

### Testing
- [ ] Test with initial process state
- [ ] Test after sbrk() allocations
- [ ] Verify virtual pages = physical pages (no demand paging yet)

---

## Part B: mmap with Demand Paging

### System Call to Implement
- `mmap(int bytes)` - Memory map with lazy allocation

### Implementation Steps

#### 1. System Call Setup
- [ ] Add mmap to system call infrastructure (like Part A)

#### 2. Implement mmap in `vm.c`
- [ ] Validate input (positive, multiple of PGSIZE)
- [ ] Increase process size (p->sz)
- [ ] Do NOT allocate physical memory
- [ ] Return starting virtual address

#### 3. Page Fault Handling in `trap.c`
- [ ] Detect T_PGFLT trap
- [ ] Get faulting address from CR2 register (`rcr2()`)
- [ ] Round down to page boundary with PGROUNDDOWN
- [ ] Allocate physical memory for faulting page
- [ ] Map page in page table
- [ ] Call `switchuvm()` or `lcr3()` to flush TLB
- [ ] Return to resume execution

#### 4. Memory Allocation
- [ ] Use `allocuvm()` or similar to allocate physical page
- [ ] Map page with proper permissions (PTE_W | PTE_U)

### Testing
- [ ] mmap increases virtual pages only
- [ ] Access triggers page fault and allocates physical page
- [ ] Only accessed pages get physical memory
- [ ] Multiple pages: partial access allocates only accessed pages

### Optional Extension: User-Specified Start Address
- [ ] Add `startaddr` parameter to mmap
- [ ] Validate address range (between break and KERNBASE)
- [ ] Check if address already mapped
- [ ] Define PTE_M flag in `mmu.h` for memory-mapped pages
- [ ] Update `numvp()`/`numpp()` to count flagged pages
- [ ] Update `fork` to copy memory-mapped pages

---

## Part C: Shared Memory IPC

### System Calls to Implement
1. `mapshared()` - Map shared page in parent
2. `getshared()` - Get shared page address
3. `unmapshared()` - Unmap shared page

### Implementation Steps

#### 1. System Call Setup
- [ ] Add system calls to infrastructure

#### 2. Define Shared Page Flag
- [ ] Define PTE_S flag in `mmu.h`

#### 3. Implement mapshared()
- [ ] Allocate page at end of address space
- [ ] Mark with PTE_S flag
- [ ] Return starting virtual address

#### 4. Implement getshared()
- [ ] Search page table for page with PTE_S flag
- [ ] Return virtual address

#### 5. Implement unmapshared()
- [ ] Find shared page
- [ ] Remove from page table
- [ ] Free physical memory
- [ ] Return status

#### 6. Modify `copyuvm()` in `vm.c`
- [ ] For pages with PTE_S flag, map same physical page
- [ ] Increment reference count if tracking

#### 7. Modify `deallocuvm()` in `vm.c`
- [ ] Don't free shared pages twice
- [ ] Check if page is shared before freeing

### Testing
- [ ] Parent writes, child reads (test-shared1)
- [ ] Multiple reads/writes (test-shared2)
- [ ] Verify same physical memory in both processes

---

## Part D: Copy-on-Write Fork

### System Call to Implement
- `getNumFreePages()` - Return number of free pages

### Implementation Steps

#### 1. Add Free Page Tracking in `kalloc.c`
- [ ] Maintain free page counter
- [ ] Increment on page free
- [ ] Decrement on page allocation
- [ ] Add locking for thread safety

#### 2. Implement getNumFreePages()
- [ ] Return current free page count

#### 3. Add Reference Counting in `kalloc.c`
- [ ] Data structure to track reference counts per page
- [ ] Initialize to 1 on allocation
- [ ] Increment when page is shared
- [ ] Decrement when reference is removed
- [ ] Free page only when count reaches 0
- [ ] Add increment/decrement functions with locks

#### 4. Modify `copyuvm()` in `vm.c`
- [ ] Don't allocate new pages for child
- [ ] Map child's page table to parent's physical pages
- [ ] Mark all shared pages as read-only (clear PTE_W)
- [ ] Increment reference count for each shared page
- [ ] Call `lcr3(v2p(pgdir))` to flush parent's TLB

#### 5. Implement CoW Page Fault Handler in `trap.c`
- [ ] Catch T_PGFLT trap
- [ ] Get faulting address from `rcr2()`
- [ ] Validate address is in valid range
- [ ] Check if fault is from write to read-only page
- [ ] Call CoW handler function

#### 6. Implement CoW Handler in `vm.c`
- [ ] Check reference count of faulting page
- [ ] If ref count > 1:
  - [ ] Allocate new physical page
  - [ ] Copy contents from old page to new page
  - [ ] Update page table to point to new page
  - [ ] Decrement ref count of old page
  - [ ] Mark new page as writable
- [ ] If ref count == 1:
  - [ ] Simply mark page as writable (no copy needed)
- [ ] Call `switchuvm()` or `lcr3()` to flush TLB

#### 7. Update Other Functions
- [ ] Modify `deallocuvm()` to handle reference counts
- [ ] Modify `freevm()` to handle reference counts

### Testing
- [ ] Fork doesn't consume pages initially
- [ ] Read doesn't trigger copy
- [ ] Write triggers copy for that page only
- [ ] Multiple writes copy multiple pages independently
- [ ] Parent and child have isolated memory after writes
- [ ] Reference counting works correctly

---

## Key Functions and Files Reference

### Important xv6 Functions

**vm.c:**
- `walkpgdir()` - Walk page table to get PTE
- `allocuvm()` - Allocate memory for process
- `deallocuvm()` - Deallocate memory
- `copyuvm()` - Copy process memory for fork
- `mappages()` - Map pages in page table
- `switchuvm()` - Switch to process page table

**kalloc.c:**
- `kalloc()` - Allocate a page
- `kfree()` - Free a page

**trap.c:**
- `trap()` - Trap handler
- `rcr2()` - Read CR2 register (faulting address)

**proc.c:**
- `myproc()` - Get current process PCB

### Important Constants and Flags

**mmu.h:**
- `PGSIZE` (4096) - Page size
- `KERNBASE` - Start of kernel address space
- `PTE_P` - Present flag
- `PTE_W` - Writable flag
- `PTE_U` - User accessible flag

### Macros
- `PGROUNDDOWN(a)` - Round address down to page boundary
- `PGROUNDUP(a)` - Round address up to page boundary
- `V2P(a)` - Virtual to physical address
- `P2V(a)` - Physical to virtual address

---

## Common Pitfalls and Solutions

### TLB Issues
**Problem:** Changes to page table not reflected in execution  
**Solution:** Call `switchuvm()` or `lcr3(v2p(pgdir))` after modifying page tables

### Double Free
**Problem:** `panic: kfree` when freeing shared pages  
**Solution:** Implement proper reference counting

### Page Fault Handling
**Problem:** Kernel panic on page fault  
**Solution:** Ensure trap handler checks for T_PGFLT and handles appropriately

### Reference Counting
**Problem:** Pages not freed or freed too early  
**Solution:** Careful increment/decrement at all sharing/unsharing points

---

## Development Workflow

1. Implement Part A first (foundation for testing)
2. Test Part A thoroughly
3. Implement Part B (builds on Part A)
4. Test Part B with provided test cases
5. Implement Part C (independent of Part B)
6. Test Part C thoroughly
7. Implement Part D (most complex, builds on understanding)
8. Test Part D extensively with custom tests

---

## Debugging Tips

1. Use `cprintf()` liberally to track execution
2. Print reference counts at key points
3. Monitor free pages before/after operations
4. Check page table flags with helper functions
5. Use GDB to debug kernel code
6. Test with small examples first
7. Verify assumptions with system call outputs

---

## Progress Tracking

### Part A: Memory Information
- [ ] System call infrastructure complete
- [ ] numvp() implemented
- [ ] numpp() implemented
- [ ] getptsize() implemented
- [ ] Testing complete

### Part B: mmap
- [ ] System call infrastructure complete
- [ ] mmap() implemented
- [ ] Page fault handler implemented
- [ ] Demand paging working
- [ ] Testing complete

### Part C: Shared Memory
- [ ] System call infrastructure complete
- [ ] mapshared() implemented
- [ ] getshared() implemented
- [ ] unmapshared() implemented
- [ ] copyuvm() modified
- [ ] deallocuvm() modified
- [ ] Testing complete

### Part D: Copy-on-Write
- [ ] getNumFreePages() implemented
- [ ] Reference counting in kalloc.c
- [ ] copyuvm() modified for CoW
- [ ] Page fault handler for CoW
- [ ] CoW memory copy logic
- [ ] Testing complete

---

## Notes and Observations

[Use this section to document any interesting findings, bugs encountered, or lessons learned during implementation]
