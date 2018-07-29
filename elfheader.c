#include "fileblock.h"

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
int writeelfheader()
{
	char headerstring[] = "\x45\x7f\x46\x4c\x01\x02\x00\x01\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x03\x00\x3e\x00\x01\x00\x00\x06\x10\x00\x00\x00\x00\x00\x00\
\x00\x40\x00\x00\x00\x00\x00\x00\x19\x98\x00\x00\x00\x00\x00\x00\
\x00\x00\x00\x00\x00\x40\x00\x38\x00\x09\x00\x40\x00\x1d\x00\x1c";

	return appendFileBlock("ELF Header", 64, headerstring);
}
