#ifndef PROGHEADER_H
#define PROGHEADER_H

#include <elf.h>

/**
 * ProgHeadBlock - program header entry block
 *
 * @name: compiler tracking name/tag/identifier
 * @p_type: type of the segment. PT_LOAC, PT_DYNAMIC, etc
 * @p_flags: segment dependent flags
 * @p_offset: offset of segment in file image
 * @p_vaddr: virtual address of segment in memory
 * @p_paddr: physical address of segment in memory (where relevant)
 * @p_filesz: size in bytes of segment in file image
 * @p_memsz: size in bytes of memory
 * @p_align: 0 and 1 no align. Otherwise should be a positive, integral power
 * of 2, with p_vaddr equating p_offset modulus p_align.
 * @index: index in program header table
 * @next: next program header block
 */
typdef struct ProgHeaderBlock
{
	char *name;
	Elf64_Word p_type;
	Elf64_Word p_flags;
	Elf64_Off p_offset;		/* Segment file offset */
	Elf64_Addr p_vaddr;		/* Segment virtual address */
	Elf64_Addr p_paddr;		/* Segment physical address */
	Elf64_Xword p_filesz;		/* Segment size in file */
	Elf64_Xword p_memsz;		/* Segment size in memory */
	Elf64_Xword p_align;		/* Segment alignment, file & memory */
	int index;
	struct ProgHeaderBlock *next;
} ProgHeaderBlock;

ProgHeaderBlock *ProgHeaderhead = NULL;

int addProgHeader(type name index flags);
int sizeProgHeader();
int writeProgHeader();

#endif
