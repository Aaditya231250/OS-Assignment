# Quick Start Guide

## Prerequisites
- Docker and Docker Compose installed
- Git installed
- xv6 source code (download from course link or GitHub)

## Setup Steps

### 1. Clone Your Repository
```bash
git clone <your-repo-url>
cd xv6-memory-mgmt-project
```

### 2. Add xv6 Source Code
Place your xv6 source code in the `xv6-src/` directory:

```bash
# Option A: If you have xv6 as a tarball
tar -xzf xv6.tar.gz
mv xv6-public/* xv6-src/

# Option B: If cloning from GitHub
git clone https://github.com/mit-pdos/xv6-public.git xv6-src
```

### 3. Copy Test Programs
Copy test programs from `tests/` to `xv6-src/`:

```bash
cp tests/*.c xv6-src/
```

### 4. Update xv6 Makefile
Edit `xv6-src/Makefile` and add test programs to UPROGS:

```makefile
UPROGS=\
	_cat\
	_echo\
	# ... existing programs ...
	_test_parta\
	_test_mmap\
	_test-shared1\
	_test-shared2\
	_test_cow\
```

### 5. Build Docker Container
```bash
docker-compose build
```

### 6. Start Development Environment
```bash
docker-compose up -d
docker-compose exec xv6-dev bash
```

You're now inside the container!

### 7. Build xv6
```bash
cd /xv6
make clean
make
```

### 8. Run xv6
```bash
make qemu-nox
```

Exit QEMU: Press `Ctrl+A` then `X`

## Implementation Order

Implement the parts in order:

1. **Part A** (Foundation): Memory information system calls
2. **Part B** (Demand Paging): mmap with page fault handling
3. **Part C** (Shared Memory): IPC mechanism
4. **Part D** (Advanced): Copy-on-Write fork

## Daily Workflow

### Starting Work
```bash
# Start container (if not running)
docker-compose up -d

# Enter container
docker-compose exec xv6-dev bash

# Navigate to xv6
cd /xv6

# Pull latest changes from team
git pull origin main
```

### During Development
```bash
# Edit files in xv6-src/ (use vim or nano in container, or your host editor)

# Rebuild after changes
make clean && make

# Test your changes
make qemu-nox

# In xv6 shell, run tests
$ test_parta
```

### Committing Changes
```bash
# Check status
git status

# Add changes
git add xv6-src/syscall.c xv6-src/vm.c

# Commit with descriptive message
git commit -m "Part A: Implement numvp() system call"

# Push to remote
git push origin main
```

### Ending Session
```bash
# Exit xv6 (if running)
Ctrl+A, X

# Exit container
exit

# Stop container (optional)
docker-compose down
```

## Common Commands

### Docker
```bash
docker-compose build          # Build/rebuild container
docker-compose up -d          # Start container in background
docker-compose down           # Stop and remove container
docker-compose exec xv6-dev bash  # Enter running container
docker-compose logs xv6-dev   # View container logs
```

### xv6
```bash
make clean     # Clean build artifacts
make           # Build xv6
make qemu      # Run with GUI
make qemu-nox  # Run without GUI (recommended)
make qemu-gdb  # Run with GDB support
```

### GDB Debugging
Terminal 1:
```bash
make qemu-nox-gdb
```

Terminal 2:
```bash
docker-compose exec xv6-dev bash
gdb kernel
(gdb) target remote localhost:26000
(gdb) b trap        # Set breakpoint
(gdb) c             # Continue
```

## Testing Each Part

### Part A
```bash
$ test_parta
# Should show virtual pages, physical pages, and page table size
```

### Part B
```bash
$ test_mmap
# Should show demand paging working correctly
```

### Part C
```bash
$ test-shared1
# Expected output:
# child 42
# parent 53

$ test-shared2
# Expected output:
# child 42
# parent 53
# child again 43
# parent again 54
```

### Part D
```bash
$ test_cow
# Should show pages copied only on write
```

## Troubleshooting

### Container won't start
```bash
docker-compose down
docker-compose build --no-cache
docker-compose up -d
```

### Build errors in xv6
```bash
# Check for syntax errors
# Ensure all system calls are declared in user.h
# Verify Makefile includes your test programs
make clean && make
```

### Test program not found
```bash
# Check if added to UPROGS in Makefile
# Rebuild
make clean && make
# Verify in xv6 shell
$ ls
```

### Page faults not handled
```bash
# Check trap.c has T_PGFLT handling
# Ensure switchuvm() or lcr3() is called
# Verify page table permissions
```

## Getting Help

1. Check `docs/implementation_notes.md` for detailed guidance
2. Read `docs/Mini_project_topic_Mem_Mgmt.pdf` for requirements
3. Review xv6 book: https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf
4. Ask team members or instructor

## File Organization

```
xv6-memory-mgmt-project/
├── xv6-src/              ← Your working directory
│   ├── syscall.c         ← Modify for system calls
│   ├── vm.c              ← Memory management
│   ├── trap.c            ← Page fault handling
│   ├── kalloc.c          ← Memory allocation
│   └── test_*.c          ← Your test programs
├── tests/                ← Test program templates
├── docs/                 ← Documentation
└── scripts/              ← Helper scripts
```

## Success Criteria

- [ ] All tests compile without errors
- [ ] Part A: Page counts are correct
- [ ] Part B: Demand paging works (virtual pages increase, physical only on access)
- [ ] Part C: Shared memory visible to both parent and child
- [ ] Part D: CoW fork doesn't copy pages until write

Good luck with your project! 🚀
