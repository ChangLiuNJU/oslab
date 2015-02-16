#ifndef __X86_MEMORY_H__
#define __X86_MEMORY_H__

#define KOFFSET 0xC0000000

/* the maxinum kernel size is 16MB */
#define KMEM (16 * 1024 * 1024)
/* Nanos has 128MB physical memory - qemu default setting */
#define PHY_MEM (128 * 1024 * 1024)

#define SEG_CODEDATA			1
#define SEG_32BIT				1
#define SEG_4KB_GRANULARITY		1
#define SEG_TSS_32BIT			0x9

#define DPL_KERNEL              0
#define DPL_USER                3

#define SEG_WRITABLE			0x2
#define SEG_READABLE			0X2
#define SEG_EXECUTABLE			0X8

#define NR_SEGMENTS             6
#define SEG_NULL				0
#define SEG_KERNEL_CODE         1 
#define SEG_KERNEL_DATA         2
#define SEG_USER_CODE			3
#define SEG_USER_DATA			4
#define SEG_TSS					5

#define SELECTOR_KERNEL(s)		( (s << 3) | DPL_KERNEL )
#define SELECTOR_USER(s)		( (s << 3) | DPL_USER )
	
/* 32bit x86 uses 4KB page size */
#define PAGE_SIZE 	4096
#define NR_PDE		1024
#define NR_PTE		1024
#define PD_SIZE		((NR_PTE) * (PAGE_SIZE))
#define NR_PAGES	27684

/* Message for memory */
#define MSG_REQ_PAGE 1

/* force the data to be aligned with page boundart
   statically defined page tables uses this feature */
#define align_to_page __attribute((aligned(PAGE_SIZE)))

/* physical address to virtual address */
#define pa_to_va(addr) \
	((void*)(((uint32_t)(addr)) + KOFFSET))
#define va_to_pa(addr) \
	((void*)(((uint32_t)(addr)) - KOFFSET))

#ifndef __ASSEMBLER__

#include "common.h"

/* the 32bit Page Directory(first level page table) data structure */
typedef union PageDirectoryEntry {
	struct {
		/*	0: not in physical memory 
			1: in physical memory */
		uint32_t present             : 1;
		/* 	0: read-write 
			1: read only */
		uint32_t read_write          : 1; 
		/* 	0: accessed by all 
			1: only supervisor  */
		uint32_t user_supervisor     : 1;
		/*	0: write-through
			1: write-back */
		uint32_t page_write_through  : 1;
		/* 	0: cache enable 
			1: cache disable  */
		uint32_t page_cache_disable  : 1;
		/*	0: has not be read/written 
			1: has been read/written */
		uint32_t accessed            : 1;
		/*  All zero. Include 0,S,G,Avail.
			More on http://wiki.osdev.org/Page_table#Page_Directory */
		uint32_t pad0                : 6;
		/*	Page Address */
		uint32_t page_frame          : 20;
	};
	uint32_t val;
} PDE;

/* the 32bit Page Table Entry(second level page table) data structure */
typedef union PageTableEntry {
	struct {
		uint32_t present             : 1;
		uint32_t read_write          : 1;
		uint32_t user_supervisor     : 1;
		uint32_t page_write_through  : 1;
		uint32_t page_cache_disable  : 1;
		uint32_t accessed            : 1;
		/*	1: has been modified
			0: has not. */
		uint32_t dirty               : 1;
		uint32_t pad0                : 1;
		uint32_t global              : 1;
		uint32_t pad1                : 3;
		/* Frame Address */
		uint32_t page_frame          : 20;
	};
	uint32_t val;
} PTE;

/* the 64bit segment descriptor */
typedef struct SegmentDescriptor {
	uint32_t limit_15_0          : 16;
	uint32_t base_15_0           : 16;
	uint32_t base_23_16          : 8;
	uint32_t type                : 4;
	uint32_t segment_type        : 1;
	uint32_t privilege_level     : 2;
	uint32_t present             : 1;
	uint32_t limit_19_16         : 4;
	uint32_t soft_use            : 1;
	uint32_t operation_size      : 1;
	uint32_t pad0                : 1;
	uint32_t granularity         : 1;
	uint32_t base_31_24          : 8;
} SegDesc;

/* the 64bit segment descriptor */
typedef struct TSS {
	uint32_t prev;			// don't care
	uint32_t esp0;
	uint32_t ss0;
	uint8_t dont_care[88];
} TSS;

struct GateDescriptor {
	uint32_t offset_15_0      : 16;
	uint32_t segment          : 16;
	uint32_t pad0             : 8;
	uint32_t type             : 4;
	uint32_t system           : 1;
	uint32_t privilege_level  : 2;
	uint32_t present          : 1;
	uint32_t offset_31_16     : 16;
};

struct TrapFrame {
	uint32_t edi, esi, ebp, xxx, ebx, edx, ecx, eax;
	int32_t irq;
	// uint32_t err;
	uint32_t eip, cs, eflags; 
};

void make_invalid_pde(PDE *);
void make_invalid_pte(PTE *);
void make_pde(PDE *, void *);
void make_pte(PTE *, void *);
void init_page(void);
void init_segment(void);
void mm(void);		//Memory Management

#endif

#endif