// Physical memory allocator, intended to allocate
// memory for user processes, kernel stacks, page table pages,
// and pipe buffers. Allocates 4096-byte pages.

#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"

void freerange(void *vstart, void *vend);
extern char end[]; // first address after kernel loaded from ELF file
                   // defined by the kernel linker script in kernel.ld

struct run {
  struct run *next;
};

struct {
  struct spinlock lock;
  int use_lock;
  struct run *freelist;
  uint numfree; // Our new free page counter
} kmem;

static uint pa_to_index(char *pa); // <-- ADD THIS LINE
static uint pg_ref_count[(PHYSTOP / PGSIZE)];
static struct spinlock ref_lock;

// Initialization happens in two phases.
// 1. main() calls kinit1() while still using entrypgdir to place just
// the pages mapped by entrypgdir on free list.
// 2. main() calls kinit2() with the rest of the physical pages
// after installing a full page table that maps them on all cores.
void
kinit1(void *vstart, void *vend)
{
  initlock(&kmem.lock, "kmem");
  initlock(&ref_lock, "ref_lock");
  kmem.use_lock = 0;
  kmem.numfree = 0; 
  freerange(vstart, vend);
}

void
kinit2(void *vstart, void *vend)
{
  freerange(vstart, vend);
  kmem.use_lock = 1;
}

void
freerange(void *vstart, void *vend)
{
  char *p;
  p = (char*)PGROUNDUP((uint)vstart);
  for(; p + PGSIZE <= (char*)vend; p += PGSIZE)
    kfree(p);
}
//PAGEBREAK: 21
// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(char *v)
{
  struct run *r;

  if((uint)v % PGSIZE || v < end || V2P(v) >= PHYSTOP)
    panic("kfree");

  if(kmem.use_lock) {
    int count = get_ref(v);

    if(count <= 0)
      panic("kfree: ref count 0 or less");

    dec_ref(v);

    if(count > 1) {
      return;
    }
  }

  memset(v, 1, PGSIZE);

  if(kmem.use_lock)
    acquire(&kmem.lock);

  kmem.numfree++;

  r = (struct run*)v;
  r->next = kmem.freelist;
  kmem.freelist = r;

  if(kmem.use_lock)
    release(&kmem.lock);
}
// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
char*
kalloc(void)
{
  struct run *r;

  if(kmem.use_lock)
    acquire(&kmem.lock);

  r = kmem.freelist;
  if(r){
    kmem.freelist = r->next;
    kmem.numfree--;
  }

  if(kmem.use_lock)
    release(&kmem.lock);

  if(r && kmem.use_lock){
    acquire(&ref_lock);
    pg_ref_count[pa_to_index((char*)r)] = 1;
    release(&ref_lock);
  }

  return (char*)r;
}
int
getNumFreePages(void)
{
  int n;
  acquire(&kmem.lock);
  n = kmem.numfree;
  release(&kmem.lock);
  return n;
}

// Get the page number from a physical address
static uint
pa_to_index(char *pa)
{
  return (V2P(pa) / PGSIZE);
}

// Increment the reference count for a page
void
inc_ref(char *pa)
{
  acquire(&ref_lock);
  pg_ref_count[pa_to_index(pa)]++;
  release(&ref_lock);
}

// Decrement the reference count for a page
void
dec_ref(char *pa)
{
  acquire(&ref_lock);
  pg_ref_count[pa_to_index(pa)]--;
  release(&ref_lock);
}

// Get the reference count for a page
int
get_ref(char *pa)
{
  int count;
  acquire(&ref_lock);
  count = pg_ref_count[pa_to_index(pa)];
  release(&ref_lock);
  return count;
}