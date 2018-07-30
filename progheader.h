#ifndef PROGHEADER_H
#define PROGHEADER_H


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
 */
typdef struct ProgHeaderBlock
{
	char *name;
	char *p_type; // 4 bytes
	char *p_flags; // 4 bytes
	char *p_offset; // 8 bytes for 64bit, 4 for 32bit
	char *p_vaddr; // 8 bytes for 64bit, 4 for 32bit
	char *p_paddr; // 8 bytes for 64bit, 4 for 32bit
	char *p_filesz; // 8 bytes for 64bit, 4 for 32bit
	char *p_memsz; // 8 bytes for 64bit, 4 for 32bit
	char *p_align; // 8 bytes for 64bit, 4 for 32bit
} ProgHeaderBlock;

#endif
