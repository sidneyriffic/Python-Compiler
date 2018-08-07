#include "elfheader.h"
#include "progheader.h"
#include "sectheader.h"
#include "symtable.h"
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
	elfheaderdata.e_flags = 0;

	return (0);
}

/**
 * setelfexec - set offset addresses and table sizes for elf header.
 * To do this, all the program headers and sections will need to be sized.
 * Section names are controlled by the compiler, so we will pick out exact
 * names for ordering and assignment to program segments.
 *
 * After initial test, size var is to be reused to size every program segment
 *
 * lots of static settings to test simple file creation
 *
 * Return: 0 on success
 */
int setelfoffsize()
{
	SectHeaderBlock *sectptr;
	Elf64_Xword size = 0;

	elfheaderdata.e_ehsize = 0x40;
	elfheaderdata.e_phoff = 0x40;
	elfheaderdata.e_phentsize = 0x38;
	elfheaderdata.e_phnum = 1;
	elfheaderdata.e_shentsize = 0x40;
	elfheaderdata.e_shnum = 0;
	sizeSectHeaders();
	/* putting text section first */
	sectptr = getSectHeader("text");
	/* elf header plus 1 prog header, hardcoded for now */
	sectptr->sh_offset = 64 + 56;
	/* putting start at beginning of text */
	elfheaderdata.e_entry = sectptr->sh_offset + 0x400000;
	size += sectptr->sh_size;
	/* rodata second */
	sectptr = getSectHeader("rodata");
	if (sectptr != NULL)
	{
		/* offset is elf header + prog table + previous sections size */
		sectptr->sh_offset = 64 + 56 + size;
		size += sectptr->sh_size;
	}
	/* only adding regular load sections for now, so section size is prog
	 * header LOAD size */
	addProgHeader(PT_LOAD, PF_R + PF_X, 0, 0x400000, size, 0x200000);
	/* not writing section table for now, but we would put the offset here
	 * after size of data is calculated */
	elfheaderdata.e_shoff = 0;
	elfheaderdata.e_shstrndx = 0;
//	addProgHeader(PT_GNU_STACK, PF_R + PF_W, 0, 0, 0, 0x10);
	return (0);
}

int writeelfheader(FILE *fd)
{
	fwrite(elfheaderdata.e_ident, 1, EI_NIDENT, fd);
	fwrite(&elfheaderdata.e_type, 1, sizeof(Elf64_Half), fd);
	fwrite(&elfheaderdata.e_machine, 1, sizeof(Elf64_Half), fd);
	fwrite(&elfheaderdata.e_version, 1, sizeof(Elf64_Word), fd);
	fwrite(&elfheaderdata.e_entry, 1, sizeof(Elf64_Addr), fd);
	fwrite(&elfheaderdata.e_phoff, 1, sizeof(Elf64_Off), fd);
	fwrite(&elfheaderdata.e_shoff, 1, sizeof(Elf64_Off), fd);
	fwrite(&elfheaderdata.e_flags, 1, sizeof(Elf64_Word), fd);
	fwrite(&elfheaderdata.e_ehsize, 1, sizeof(Elf64_Half), fd);
	fwrite(&elfheaderdata.e_phentsize, 1, sizeof(Elf64_Half), fd);
	fwrite(&elfheaderdata.e_phnum, 1, sizeof(Elf64_Half), fd);
	fwrite(&elfheaderdata.e_shentsize, 1, sizeof(Elf64_Half), fd);
	fwrite(&elfheaderdata.e_shnum, 1, sizeof(Elf64_Half), fd);
	fwrite(&elfheaderdata.e_shstrndx, 1, sizeof(Elf64_Half), fd);
	return (0);
}

/**
 * writeelf - write the ELF file
 *
 * Ordering:
 * 1. Elf header
 * 2. Program header table
 * 3. Section data
 * 3a. rodata
 * 3b. text
 * 4. Section header table
 *
 * Return: 0 if successful
 */
int writeelf(FILE *fd)
{
	writeelfheader(fd);
	writeProgHeadertable(fd);
	writeSectData("text", fd);
	writeSectData("rodata", fd);
}
