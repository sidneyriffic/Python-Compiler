#ifndef ELFHEADER_H
#define ELFHEADER_H

#include <elf.h>

/**
 * ElfHeaderBlock - stores elf header information
 *
 * @e_ident: magic number, class, data, version, OSABI, OSABI version
 * @e_type: object file type, exec, dyn, etc
 * @e_machine: target instruction set architechture
 * @e_version: set to 1 for original ELF
 * @e_entry: entry point for program execution.
 * @e_phoff: program header start offset.
 * @e_shoff: section header start offset.
 * @e_flags: flags for architecture.
 * @e_ehsize: size of elf header. 52 for 32bit, 64 for 64bit
 * @e_phentsize: size of a program header entry
 * @e_phnum: number of program header entries
 * @e_shentsize: size of section header entry
 * @e_shnum: number of section header entries
 * @e_shstrndx: index of section header entry with section names
 *
 * Comments below contain 64 bit sizes. Eventually error checking should be
 * moved into functions, but for now it is done manually.
 */
typdef struct ElfHeaderBlock
{
	unsigned char	e_ident[EI_NIDENT];	/* ELF "magic number" */
	Elf64_Half e_type;
	Elf64_Half e_machine;
	Elf64_Word e_version;
	Elf64_Addr e_entry;		/* Entry point virtual address */
	Elf64_Off e_phoff;		/* Program header table file offset */
	Elf64_Off e_shoff;		/* Section header table file offset */
	Elf64_Word e_flags;
	Elf64_Half e_ehsize;
	Elf64_Half e_phentsize;
	Elf64_Half e_phnum;
	Elf64_Half e_shentsize;
	Elf64_Half e_shnum;
	Elf64_Half e_shstrndx;
} ElfHeaderBlock;

ElfHeaderBlock elfheaderdata = NULL;

int initelfheader();
int setelfoffsize();
int writeelfheader();

#endif
