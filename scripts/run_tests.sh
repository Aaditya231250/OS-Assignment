#!/bin/bash
# Script to run all tests for the xv6 memory management project

set -e

echo "=== xv6 Memory Management Tests ==="
echo ""

if [ ! -f "xv6-src/Makefile" ]; then
    echo "ERROR: xv6 source not found. Run setup_xv6.sh first."
    exit 1
fi

cd xv6-src

# Rebuild xv6
echo "Building xv6..."
make clean && make

if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

echo ""
echo "Build successful!"
echo ""
echo "Available tests:"
echo "  1. Part A - Memory Information (numvp, numpp, getptsize)"
echo "  2. Part B - mmap with demand paging"
echo "  3. Part C - Shared Memory (test-shared1, test-shared2)"
echo "  4. Part D - Copy-on-Write Fork"
echo ""
echo "To run tests, start xv6 and run the test programs:"
echo "  make qemu-nox"
echo ""
echo "In the xv6 shell:"
echo "  $ test_parta"
echo "  $ test_mmap"
echo "  $ test-shared1"
echo "  $ test-shared2"
echo "  $ test_cow"
echo ""
