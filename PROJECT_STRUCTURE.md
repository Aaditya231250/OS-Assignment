# xv6 Memory Management Project - Complete File Structure

## Directory Tree

```
xv6-memory-mgmt-project/
│
├── Dockerfile                          # Container definition with all dependencies
├── docker-compose.yml                  # Docker orchestration configuration
├── README.md                           # Main project documentation
├── .gitignore                          # Git ignore patterns
├── .gitattributes                      # Git line ending configuration
│
├── xv6-src/                           # ← Place xv6 source code here
│   ├── README.md                       # Instructions for adding xv6 source
│   ├── Makefile                        # (from xv6) Build configuration
│   ├── syscall.c                       # (from xv6) System call dispatch
│   ├── syscall.h                       # (from xv6) System call numbers
│   ├── sysproc.c                       # (from xv6) Process system calls
│   ├── vm.c                            # (from xv6) Virtual memory management
│   ├── proc.c                          # (from xv6) Process management
│   ├── trap.c                          # (from xv6) Trap handling
│   ├── kalloc.c                        # (from xv6) Physical memory allocator
│   ├── mmu.h                           # (from xv6) MMU definitions
│   ├── defs.h                          # (from xv6) Function declarations
│   ├── user.h                          # (from xv6) User-space declarations
│   ├── usys.S                          # (from xv6) User system call stubs
│   ├── test_parta.c                    # (copy from tests/) Part A test
│   ├── test_mmap.c                     # (copy from tests/) Part B test
│   ├── test-shared1.c                  # (copy from tests/) Part C test 1
│   ├── test-shared2.c                  # (copy from tests/) Part C test 2
│   ├── test_cow.c                      # (copy from tests/) Part D test
│   └── ... (many other xv6 files)
│
├── tests/                              # Test program templates (copy to xv6-src/)
│   ├── test_parta.c                    # Tests numvp, numpp, getptsize
│   ├── test_mmap.c                     # Tests mmap with demand paging
│   ├── test-shared1.c                  # Tests shared memory (simple)
│   ├── test-shared2.c                  # Tests shared memory (complex)
│   └── test_cow.c                      # Tests copy-on-write fork
│
├── docs/                               # Documentation
│   ├── Mini_project_topic_Mem_Mgmt.pdf # Original project specification
│   ├── QUICKSTART.md                   # Quick setup and workflow guide
│   ├── implementation_notes.md         # Detailed implementation guidance
│   └── makefile_guide.md               # How to modify Makefile
│
└── scripts/                            # Helper scripts
    ├── setup_xv6.sh                    # Setup and build script (executable)
    └── run_tests.sh                    # Test runner script (executable)
```

## File Descriptions

### Root Level Files

**Dockerfile**
- Defines Ubuntu 24.04 based container
- Installs: build-essential, QEMU, GDB, gcc-multilib
- Creates xv6dev user for proper permissions

**docker-compose.yml**
- Orchestrates container with volume mounts
- Maps host directories to container
- Enables interactive development

**README.md**
- Complete project overview
- Setup instructions
- Development workflow
- Testing strategy
- Git collaboration guide

**.gitignore**
- Excludes build artifacts (*.o, *.img, kernel)
- Excludes editor files (.vscode, .idea)
- Excludes Docker volumes

**.gitattributes**
- Ensures proper line endings (LF for scripts)
- Handles text/binary file detection

### xv6-src/ - Main Working Directory

This is where all your development happens. You'll modify these xv6 files:

**System Call Infrastructure:**
- `syscall.h` - Add system call numbers
- `syscall.c` - Register system calls
- `user.h` - Declare user-space functions
- `usys.S` - Assembly wrappers for system calls

**Memory Management:**
- `vm.c` - Virtual memory operations (allocuvm, copyuvm, etc.)
- `kalloc.c` - Physical page allocation
- `mmu.h` - MMU constants and flags

**Process Management:**
- `proc.c` - Process operations
- `sysproc.c` - Process system call implementations

**Trap Handling:**
- `trap.c` - Trap/interrupt/fault handling

**Build System:**
- `Makefile` - Add test programs to UPROGS

**Test Programs:**
- Copy from tests/ directory and add to Makefile

### tests/ - Test Program Templates

Pre-written test programs for each part:

**test_parta.c** (Part A)
- Tests numvp(), numpp(), getptsize()
- Verifies counts after sbrk() calls
- Validates page table size calculations

**test_mmap.c** (Part B)
- Tests mmap() with various sizes
- Verifies demand paging behavior
- Checks that physical pages allocated only on access
- Tests partial access to mapped region

**test-shared1.c** (Part C)
- Simple shared memory test
- Parent writes, child reads
- Expected output: child 42, parent 53

**test-shared2.c** (Part C)
- Complex shared memory test
- Multiple reads/writes by both processes
- Tests bidirectional communication

**test_cow.c** (Part D)
- Tests copy-on-write fork
- Monitors free page counts
- Verifies pages copied only on write
- Tests memory isolation between parent/child

### docs/ - Documentation

**Mini_project_topic_Mem_Mgmt.pdf**
- Original project specification from IIT Jodhpur
- Complete requirements for all four parts

**QUICKSTART.md**
- Step-by-step setup guide
- Daily workflow instructions
- Common commands reference
- Troubleshooting guide

**implementation_notes.md**
- Detailed implementation steps for each part
- Function references
- Important constants and flags
- Common pitfalls and solutions
- Progress tracking checklist

**makefile_guide.md**
- How to modify xv6 Makefile
- Adding test programs to UPROGS
- Build system explanation

### scripts/ - Helper Scripts

**setup_xv6.sh**
- Checks for xv6 source code
- Builds xv6 if source exists
- Provides helpful error messages
- Executable: `chmod +x`

**run_tests.sh**
- Rebuilds xv6
- Lists available tests
- Provides instructions for running tests
- Executable: `chmod +x`

## Usage Workflow

### Initial Setup
```bash
1. git clone <repo-url>
2. cd xv6-memory-mgmt-project
3. # Add xv6 source to xv6-src/
4. cp tests/*.c xv6-src/
5. # Edit xv6-src/Makefile to add test programs
6. docker-compose build
7. docker-compose up -d
8. docker-compose exec xv6-dev bash
```

### Development Cycle
```bash
1. Edit files in xv6-src/
2. make clean && make
3. make qemu-nox
4. # Test in xv6 shell
5. Ctrl+A, X (exit QEMU)
6. git commit changes
```

### Testing
```bash
# In xv6 shell:
$ test_parta
$ test_mmap
$ test-shared1
$ test-shared2
$ test_cow
```

## Implementation Order

1. **Part A** - Foundation
   - Implement: numvp(), numpp(), getptsize()
   - Files: syscall.{c,h}, sysproc.c, vm.c, user.h, usys.S

2. **Part B** - Demand Paging
   - Implement: mmap()
   - Files: vm.c (mmap logic), trap.c (page fault handler)

3. **Part C** - Shared Memory
   - Implement: mapshared(), getshared(), unmapshared()
   - Files: vm.c, mmu.h (PTE_S flag)

4. **Part D** - Copy-on-Write
   - Implement: getNumFreePages(), reference counting, CoW fork
   - Files: kalloc.c (ref counting), vm.c (copyuvm), trap.c (CoW handler)

## Key Features

✅ **Containerized** - Consistent development environment for all team members
✅ **Version Controlled** - Git-ready structure with proper .gitignore
✅ **Well Documented** - Multiple documentation files for different purposes
✅ **Test Programs** - Pre-written tests for each part
✅ **Helper Scripts** - Automation for common tasks
✅ **Organized** - Clear separation of source, tests, docs, and scripts

## Team Collaboration

1. Each member clones the repository
2. Everyone works in the same containerized environment
3. Changes to xv6-src/ are committed to Git
4. Test programs ensure consistency across implementations
5. Documentation provides shared knowledge base

## Success Metrics

- ✓ All files in proper locations
- ✓ Docker container builds successfully
- ✓ xv6 compiles without errors
- ✓ Test programs are executable in xv6
- ✓ All four parts pass their tests

---

This structure provides everything you need for successful completion of the xv6 memory management project!
