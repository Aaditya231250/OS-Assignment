# Makefile Modifications Guide

This document explains what needs to be added to the xv6 Makefile to include your test programs.

## Adding Test Programs to UPROGS

In the xv6 Makefile, find the `UPROGS` variable (usually around line 180-190). This variable lists all user programs that should be compiled.

Add your test programs to this list:

```makefile
UPROGS=\
	_cat\
	_echo\
	_forktest\
	_grep\
	_init\
	_kill\
	_ln\
	_ls\
	_mkdir\
	_rm\
	_sh\
	_stressfs\
	_usertests\
	_wc\
	_zombie\
	_mytest\
	_test_parta\
	_test_mmap\
	_test-shared1\
	_test-shared2\
	_test_cow\
```

**Note:** The underscore prefix is required by xv6's build system.

## Test Program Files Location

Your test program source files should be placed in the xv6 source directory (same level as other user programs like `cat.c`, `ls.c`, etc.):

```
xv6-src/
├── cat.c
├── ls.c
├── sh.c
├── ... (other existing files)
├── test_parta.c      # Your test program
├── test_mmap.c       # Your test program
├── test-shared1.c    # Your test program
├── test-shared2.c    # Your test program
└── test_cow.c        # Your test program
```

## How the Build System Works

When you run `make`:

1. The Makefile compiles each `.c` file in UPROGS to a `_filename` executable
2. These executables are included in the `fs.img` filesystem image
3. You can run them from the xv6 shell by typing the name without the underscore

Example:
```
$ test_parta
$ test_mmap
```

## Full Makefile Template

If you need to reference the complete Makefile structure, here's what it should look like:

```makefile
# ... (beginning of Makefile with build rules)

# User programs
UPROGS=\
	_cat\
	_echo\
	_forktest\
	_grep\
	_init\
	_kill\
	_ln\
	_ls\
	_mkdir\
	_rm\
	_sh\
	_stressfs\
	_usertests\
	_wc\
	_zombie\
	_mytest\
	_test_parta\
	_test_mmap\
	_test-shared1\
	_test-shared2\
	_test_cow\

# ... (rest of Makefile)
```

## Recompiling After Changes

After adding programs to UPROGS:

```bash
make clean   # Clean old build artifacts
make         # Rebuild everything
make qemu-nox # Run xv6
```

## Common Issues

### Issue: Test program not found in xv6 shell
**Solution:** Make sure you added it to UPROGS with underscore prefix and rebuilt with `make clean && make`

### Issue: Compilation errors for test programs
**Solution:** 
1. Check that all required system calls are declared in `user.h`
2. Ensure system calls are implemented in kernel
3. Verify no syntax errors in your test program

### Issue: "exec failed" error when running test
**Solution:**
1. The executable wasn't included in fs.img
2. Rebuild: `make clean && make`
3. If still failing, check that the source file has the correct name

## System Call Dependencies

Make sure these system calls are implemented before compiling:

**Part A:**
- `numvp()`
- `numpp()`
- `getptsize()`

**Part B:**
- `mmap(int bytes)`
- All from Part A

**Part C:**
- `mapshared()`
- `getshared()`
- `unmapshared()`
- All from Part A

**Part D:**
- `getNumFreePages()`
- All from Part A

## Verification

After rebuilding, verify your programs are included:

```bash
$ make qemu-nox
# In xv6 shell:
$ ls
# You should see your test programs listed
```
