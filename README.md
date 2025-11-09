# xv6 Memory Management Mini Project

**Course**: CSL-3030 Operating Systems Lab  
**Institution**: IIT Jodhpur  
**Topic**: Memory Management in xv6

## Project Overview

This project implements advanced memory management features in the xv6 operating system:

- **Part A**: Displaying memory information (numvp, numpp, getptsize system calls)
- **Part B**: Memory mapping with mmap system call (demand paging)
- **Part C**: Shared memory IPC mechanism
- **Part D**: Copy-on-Write (CoW) fork implementation

## Prerequisites

- Docker and Docker Compose
- Git
- At least 4GB of free disk space

## Quick Start

### 1. Clone the Repository

```bash
git clone <your-repo-url>
cd xv6-memory-mgmt-project
```

### 2. Build the Docker Container

```bash
docker-compose build
```

### 3. Start the Development Environment

```bash
docker-compose up -d
docker-compose exec xv6-dev bash
```

### 4. Build and Run xv6

Inside the container:

```bash
cd /xv6
make
make qemu-nox  # Run without GUI
# OR
make qemu      # Run with GUI (requires X11 forwarding)
```

To exit QEMU, press `Ctrl+A` then `X`.

## Project Structure

```
xv6-memory-mgmt-project/
├── Dockerfile              # Container definition
├── docker-compose.yml      # Docker compose configuration
├── README.md               # This file
├── xv6-src/               # xv6 source code (place xv6 files here)
│   ├── syscall.c
│   ├── syscall.h
│   ├── sysproc.c
│   ├── user.h
│   ├── usys.S
│   ├── vm.c
│   ├── proc.c
│   ├── trap.c
│   ├── defs.h
│   ├── mmu.h
│   ├── Makefile
│   └── ...
├── tests/                  # Test programs
│   ├── test_parta.c
│   ├── test_mmap.c
│   ├── test-shared1.c
│   ├── test-shared2.c
│   └── test_cow.c
├── docs/                   # Documentation
│   ├── Mini_project_topic_Mem_Mgmt.pdf
│   ├── implementation_notes.md
│   └── testing_guide.md
└── scripts/                # Helper scripts
    ├── setup_xv6.sh
    └── run_tests.sh
```

## Development Workflow

### Adding New System Calls

1. Define system call number in `syscall.h`
2. Add system call to `syscall.c` syscalls array
3. Implement system call in `sysproc.c` or `vm.c`
4. Add user-space wrapper in `usys.S`
5. Declare in `user.h`
6. Rebuild: `make clean && make`

### Adding New User Programs

1. Create your C file in the xv6-src directory (e.g., `mytest.c`)
2. Add the program name to `UPROGS` in the Makefile
3. Rebuild: `make`
4. Run in xv6 shell: `mytest`

### Running Tests

Inside the container:

```bash
# Build xv6 with your changes
make clean && make

# Start xv6
make qemu-nox

# In the xv6 shell, run your test programs
$ test_parta
$ test_mmap
$ test-shared1
$ test-shared2
$ test_cow
```

## Important Files to Modify

### Part A: Memory Information
- `syscall.h`, `syscall.c`, `user.h`, `usys.S`
- `sysproc.c` (system call implementations)
- `vm.c` (page table walking functions)

### Part B: mmap System Call
- All files from Part A
- `vm.c` (memory allocation logic)
- `trap.c` (page fault handling)
- `proc.c` (process management)

### Part C: Shared Memory
- `vm.c` (shared page management)
- `mmu.h` (define PTE_S flag)
- `proc.c` (fork modifications)

### Part D: Copy-on-Write Fork
- `kalloc.c` (reference counting)
- `vm.c` (copyuvm modifications)
- `trap.c` (page fault handling)
- `mmu.h` (macros for page manipulation)

## Debugging Tips

### Using GDB

Terminal 1:
```bash
make qemu-nox-gdb
```

Terminal 2:
```bash
docker-compose exec xv6-dev bash
gdb kernel
(gdb) target remote localhost:26000
(gdb) b trap
(gdb) c
```

### Common Issues

1. **Build errors**: Make sure all dependencies are installed
   ```bash
   sudo apt-get update
   sudo apt-get install build-essential gdb qemu-system-x86 gcc-multilib
   ```

2. **QEMU won't start**: Check if KVM is available or use `-no-kvm` flag

3. **Page faults not caught**: Ensure trap handling code checks for T_PGFLT

4. **Double free errors**: Check reference counting logic in kalloc.c

## Testing Strategy

1. **Part A**: Verify page counts match expected values
2. **Part B**: Check virtual pages increase but physical pages don't until access
3. **Part C**: Verify parent and child see each other's changes
4. **Part D**: Monitor free pages decrease only when pages are written to

## Git Workflow

```bash
# Create feature branch
git checkout -b feature/part-a-memory-info

# Make changes and commit
git add .
git commit -m "Implement Part A: memory information system calls"

# Push to remote
git push origin feature/part-a-memory-info

# Create pull request on GitHub
```

## Container Management

```bash
# Start container
docker-compose up -d

# Enter container
docker-compose exec xv6-dev bash

# Stop container
docker-compose down

# Rebuild container (after Dockerfile changes)
docker-compose build --no-cache

# View logs
docker-compose logs xv6-dev
```

## Team Collaboration

1. Each team member should have Docker installed
2. Clone the shared repository
3. Run `docker-compose up -d` to start the environment
4. All development happens inside the container
5. Changes to xv6-src are immediately reflected for all team members

## Resources

- [xv6 Book](https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf)
- [xv6 Source Code](https://github.com/mit-pdos/xv6-public)
- Course documentation: `docs/Mini_project_topic_Mem_Mgmt.pdf`

## License

Educational use only - IIT Jodhpur CSL-3030 Course

## Contributors

[Add your team member names here]
