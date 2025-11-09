# 🚀 Getting Started - xv6 Memory Management Project

Welcome to your containerized xv6 development environment! This project is ready to sync across your team via GitHub.

## 📋 What You Have

✅ **Complete Docker Environment** - Ubuntu 24.04 with all xv6 dependencies  
✅ **Test Programs** - Pre-written tests for all 4 parts  
✅ **Comprehensive Documentation** - Step-by-step guides for implementation  
✅ **Helper Scripts** - Automated setup and testing  
✅ **Git Ready** - Proper .gitignore and structure for version control  

## 🎯 Quick Start (5 Minutes)

### Step 1: Clone and Enter
```bash
git clone <your-repo-url>
cd xv6-memory-mgmt-project
```

### Step 2: Add xv6 Source Code
```bash
# Download xv6 from course link OR clone from GitHub:
git clone https://github.com/mit-pdos/xv6-public.git xv6-src-temp
mv xv6-src-temp/* xv6-src/
rm -rf xv6-src-temp
```

### Step 3: Copy Test Programs
```bash
cp tests/*.c xv6-src/
```

### Step 4: Update Makefile
Edit `xv6-src/Makefile` and add to UPROGS:
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

### Step 5: Build and Run
```bash
# Build container
docker-compose build

# Start container
docker-compose up -d

# Enter container
docker-compose exec xv6-dev bash

# Inside container:
cd /xv6
make clean && make
make qemu-nox
```

Exit QEMU: `Ctrl+A` then `X`

## 📚 Documentation Files

| File | Purpose |
|------|---------|
| `README.md` | Complete project overview and reference |
| `docs/QUICKSTART.md` | Daily workflow and commands |
| `docs/implementation_notes.md` | Detailed implementation guidance |
| `docs/makefile_guide.md` | How to add test programs |
| `PROJECT_STRUCTURE.md` | Complete file structure explanation |

## 🔨 Implementation Order

1. **Part A** → Memory Info (numvp, numpp, getptsize)
2. **Part B** → mmap + Demand Paging
3. **Part C** → Shared Memory IPC
4. **Part D** → Copy-on-Write Fork

## 📝 Each Part Requires

### Part A: Memory Information
- Add 3 system calls: numvp(), numpp(), getptsize()
- Modify: syscall.{c,h}, user.h, usys.S, sysproc.c, vm.c
- Test: `$ test_parta`

### Part B: mmap with Demand Paging
- Add 1 system call: mmap(int bytes)
- Implement page fault handler in trap.c
- Allocate physical memory only on access
- Test: `$ test_mmap`

### Part C: Shared Memory
- Add 3 system calls: mapshared(), getshared(), unmapshared()
- Define PTE_S flag in mmu.h
- Modify copyuvm() and deallocuvm()
- Test: `$ test-shared1` and `$ test-shared2`

### Part D: Copy-on-Write Fork
- Add getNumFreePages() system call
- Implement reference counting in kalloc.c
- Modify copyuvm() for CoW
- Handle CoW page faults in trap.c
- Test: `$ test_cow`

## 🎓 Important Files to Modify

| File | What to Change |
|------|----------------|
| `syscall.h` | Add system call numbers |
| `syscall.c` | Register system calls |
| `user.h` | Declare user-space functions |
| `usys.S` | Assembly wrappers |
| `sysproc.c` | Simple system call implementations |
| `vm.c` | Memory management logic |
| `trap.c` | Page fault handling |
| `kalloc.c` | Reference counting (Part D) |
| `mmu.h` | Add new flags (PTE_M, PTE_S) |

## 🐛 Common Issues and Solutions

**Build fails in container:**
```bash
docker-compose down
docker-compose build --no-cache
docker-compose up -d
```

**Test program not found in xv6:**
- Add to UPROGS in Makefile with `_` prefix
- Rebuild: `make clean && make`

**Page faults cause kernel panic:**
- Check trap.c handles T_PGFLT
- Call switchuvm() after modifying page tables
- Verify page permissions

**Double free error:**
- Implement proper reference counting
- Don't free shared pages twice

## 👥 Team Collaboration

Each team member should:
1. Clone the repository
2. Run `docker-compose build` once
3. Start working with `docker-compose up -d && docker-compose exec xv6-dev bash`
4. Commit and push changes regularly
5. Pull team changes before starting work

## 📊 Testing Strategy

1. **Part A** first - provides foundation for monitoring other parts
2. After each part, verify with provided test programs
3. Write additional edge-case tests
4. Use numvp()/numpp() to verify page counts
5. Monitor output carefully for unexpected behavior

## 💡 Pro Tips

- **Use Part A system calls** to debug other parts
- **Print reference counts** liberally in Part D
- **Check page table flags** when debugging
- **Use GDB** for kernel debugging
- **Read xv6 book** for deep understanding
- **Test incrementally** - don't implement everything at once

## 🎯 Success Criteria

- [ ] Docker container builds and runs
- [ ] xv6 compiles without errors
- [ ] All test programs run in xv6 shell
- [ ] Part A: Correct page counts
- [ ] Part B: Virtual pages increase, physical only on access
- [ ] Part C: Parent and child see each other's writes
- [ ] Part D: Pages copied only on write

## 🆘 Need Help?

1. Check `docs/implementation_notes.md` for detailed guidance
2. Review xv6 book: https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf
3. Read project PDF in `docs/Mini_project_topic_Mem_Mgmt.pdf`
4. Ask team members or instructor

## 📦 What's Included

```
xv6-memory-mgmt-project/
├── Dockerfile              # Development environment
├── docker-compose.yml      # Container orchestration
├── README.md              # Main documentation
├── xv6-src/               # Your working directory
├── tests/                 # Test program templates
├── docs/                  # Detailed guides
└── scripts/               # Helper automation
```

## 🚀 Next Steps

1. **Read** `docs/QUICKSTART.md` for detailed workflow
2. **Review** `docs/implementation_notes.md` for Part A
3. **Start** implementing Part A system calls
4. **Test** with `test_parta` program
5. **Move** to Part B after Part A works

---

**Ready to start coding?** Jump into `docs/implementation_notes.md` for Part A! 💪

Good luck with your project! 🎉
