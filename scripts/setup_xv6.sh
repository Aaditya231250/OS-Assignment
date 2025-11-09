#!/bin/bash
# Setup script for xv6 development environment

set -e

echo "=== xv6 Memory Management Project Setup ==="
echo ""

# Check if xv6 source exists
if [ ! -f "xv6-src/Makefile" ]; then
    echo "ERROR: xv6 source code not found in xv6-src/"
    echo ""
    echo "Please do one of the following:"
    echo "1. Download xv6 from the course link and extract to xv6-src/"
    echo "2. Clone xv6: git clone https://github.com/mit-pdos/xv6-public.git xv6-src"
    echo ""
    exit 1
fi

echo "✓ xv6 source code found"

# Check if we're inside the container
if [ ! -f "/.dockerenv" ]; then
    echo ""
    echo "WARNING: Not running inside Docker container"
    echo "Run: docker-compose up -d && docker-compose exec xv6-dev bash"
    echo ""
fi

# Build xv6
echo ""
echo "Building xv6..."
cd xv6-src
make clean
make

if [ $? -eq 0 ]; then
    echo ""
    echo "✓ xv6 built successfully!"
    echo ""
    echo "To run xv6:"
    echo "  make qemu-nox    # Run without GUI"
    echo "  make qemu        # Run with GUI"
    echo ""
    echo "To exit QEMU: Ctrl+A then X"
else
    echo ""
    echo "✗ Build failed. Check the error messages above."
    exit 1
fi
