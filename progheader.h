#ifndef PROGHEADER_H
#define PROGHEADER_H

#include <elf.h>
#include <stdio.h>

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
typedef struct ProgHeaderBlock
{
	Elf64_Word p_type;
	Elf64_Word p_flags;
	Elf64_Off p_offset;		/* Segment file offset */
	Elf64_Addr p_vaddr;		/* Segment virtual address */
	Elf64_Addr p_paddr;		/* Segment physical address */
	Elf64_Xword p_filesz;		/* Segment size in file */
	Elf64_Xword p_memsz;		/* Segment size in memory */
	Elf64_Xword p_align;		/* Segment alignment, file & memory */
	struct ProgHeaderBlock *next;
} ProgHeaderBlock;

int addProgHeader(Elf64_Word type, Elf64_Word flags, Elf64_Off offset,
		  Elf64_Addr address, Elf64_Xword size);
int writeProgHeadertable(FILE *fd);

#endif
