#ifndef SECTHEADER_H
#define SECTHEADER_H


/**
 * SectionData - data storage list for section data
 *
 * @data: string of data
 * @next: next node in list
 */
typedef struct SectionData
{
	char *data;
	struct SectionData *next;
}

/**
 * SectHeadBlock - section header entry block
 *
 * @name: compiler tracking name/id/etc
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
 * @progtype: program block type to put this section into
 * @datahead: head of data linked list
 */
typdef struct SectHeaderBlock
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
	char progtype;
	SectionData *datahead;
} SectHeaderBlock;

#endif
