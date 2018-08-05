#ifndef SECTHEADER_H
#define SECTHEADER_H

#include <elf.h>

/**
 * SectionData - data storage list for section data. machine code, strings, etc
 *
 * @data: string of data
 * @next: next node in list
 */
typedef struct SectionData
{
	char *data;
	size_t len;
	struct SectionData *next;
}

/**
 * SectHeadBlock - section header entry block
 *
 * @name: Human readable name. To be put in .shstrtab. Omit leading '.'
 * @sh_name: offset in .shstrtab section that represents name of section
 * @sh_type: type of header
 * @sh_flags: flag attributes
 * @sh_addr: virtual address of the section in memory if loaded
 * @sh_offset: offset of section in file image
 * @sh_size: size in bytes of section
 * @sh_link: section index of associated section
 * @sh_info: extra info about section
 * @sh_addralign: alignment of section. must be power of two
 * @sh_entsize: size, in bytes, of each entry in section for sections with
 * fixed-size entries. Otherwise 0.
 * @datahead: head of data linked list
 * @next: next structure header block
 */
typdef struct SectHeaderBlock
{
	char *name;
	Elf64_Word sh_name;		/* Section name, index in string tbl */
	Elf64_Word sh_type;		/* Type of section */
	Elf64_Xword sh_flags;		/* Miscellaneous section attributes */
	Elf64_Addr sh_addr;		/* Section virtual addr at execution */
	Elf64_Off sh_offset;		/* Section file offset */
	Elf64_Xword sh_size;		/* Size of section in bytes */
	Elf64_Word sh_link;		/* Index of another section */
	Elf64_Word sh_info;		/* Additional section information */
	Elf64_Xword sh_addralign;	/* Section alignment */
	Elf64_Xword sh_entsize;	/* Entry size if section holds table */
	SectionData *datahead;
	struct SectHeaderBlock *next;
} SectHeaderBlock;

/* head of section header list */
SectHeaderBlock *SectHeaderhead = NULL;

int addSectHeader(char *name, Elf64_Word type, Elf64_Xword flags);
int appendsectdata(char *section, char *data);
int sizeSectHeader();
int writeSectHeader();

#endif
