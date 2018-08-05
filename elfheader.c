#include "elfheader.h"
#include <string.h>

ElfHeaderBlock elfheaderdata;

/**
 * writeelfheader - write elf header to FileBlock list. Eventually this might
 * have more compile options, but for now it is just x64 ubuntu
 *
 * 457f 464c 0102 0001 0000 0000 0000 0000
 * 0003 003e 0001 0000 0610 0000 0000 0000
 * 0040 0000 0000 0000 1998 0000 0000 0000
 * 0000 0000 0040 0038 0009 0040 001d 001c
 *
 * Return: 0 on success, 1 otherwise
 */
int initelfheader()
{
	int ct;
	char headerchars[64] = "\x7f""ELF";

	/* temporarily explicitly coding for one system */
	strcpy(elfheaderdata.e_ident, "\x7f""ELF" /* magic */
	       "\x02" /* 1 for 32-bit, 2 for 64-bit */
	       "\x01" /* 1 for little endian, 2 for big */
	       "\x01" /* 1 for original version of elf */
	       "\x00\x00" /* OSABI, set to unix systemv here */
	       "\x00\x00\x00\x00\x00\x00\x00"); /* padding */
	/* object type, 0x02 for exec */
	elfheaderdata.e_type = ET_EXEC;
	/* machine type */
	elfheaderdata.e_machine = EM_X86_64;
	/* elf version, set to 1 for original */
	elfheaderdata.e_version = 1;

	return (0);
}

/**
 * setelfexec - set offset addresses and table sizes for elf header.
 */
int setelfoffsize()
{
	return (0);
}

int writeelfheader()
{
	return (0);
}

/* old methodology. Preserving temporarily in case some of it is useful before
 * new method is done.
/**
 * setelfproghead - set program header attributes. For now uses a char array.
 * Eventually want to use size_t input.
 *
 * @startaddress: where the program header starts
 * @size: how large the program header block is
 * @num: number of entries
 *
 * Return: 0 on success, 1 on failure
int setelfproghead(char *startaddress, char *size, char *num)
{
	strncpy(FileBlockhead->data + 32, startaddress, 8);
	strncpy(FileBlockhead->data + 54, size, 2);
	strncpy(FileBlockhead->data + 56, num, 2);
	return (0);
}

/**
 * setelfsecthead - set section header attributes. For now uses a char array.
 * Eventually want to use size_t input.
 *
 * @startaddress: where the section header starts
 * @size: how large the section header block is
 * @num: number of entries
 *
 * Return: 0 on success, 1 on failure
int setelfsecthead(char *startaddress, char *size, char *num, char *index)
{
	strncpy(FileBlockhead->data + 40, startaddress, 8);
	strncpy(FileBlockhead->data + 58, size, 2);
	strncpy(FileBlockhead->data + 60, num, 2);
	strncpy(FileBlockhead->data + 62, index, 2);
	return (0);
}
*/
