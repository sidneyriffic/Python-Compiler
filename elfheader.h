#ifndef ELFHEADER_H
#define ELFHEADER_H

/**
 * ElfHeaderBlock - stores elf header information
 *
 * @magic: \x7fELF magic number
 * @ei_class: 1 for 32 bit, 2 for 64 bit
 * @ei_data: 1 for little, 2 for big endian
 * @ei_version: 1 for original ELF
 * @ei_osabi: target operating system ABI
 * @ei_abiversion: version of ABI
 * @ei_pad: unused space
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
	char *magic; // 4 bytes
	char ei_class;
	char ei_data;
	char ei_version;
	char ei_osabi;
	char ei_abiversion;
	char *ei_pad; // 7 bytes, currently unused
	char *e_type; // 2 bytes
	char *e_machine; // 2 bytes
	char *e_version; // 4 bytes
	char *e_entry; // exec entry. 8 bytes 64bit, 4 bytes 32bit
	char *e_phoff; // 8 bytes
	char *e_shoff; // 8 bytes
	char *e_flags; // 4 bytes
	char *e_ehsize; // 2 bytes
	char *e_phentsize; // 2 bytes
	char *e_phnum; // 2 bytes
	char *e_shentsize; // 2 bytes
	char *e_shnum; // 2 bytes
	char *e_shstrndx; // 2 bytes
} ElfHeaderBlock;

ElfHeaderBlock elfheaderdata;

int initelfheader();
void setelfexec(char *address);
int setelfproghead(char *startaddress, char *size, char *num);
int setelfproghead(char *startaddress, char *size, char *num, char *index);

#endif
