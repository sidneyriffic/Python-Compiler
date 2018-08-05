#include "progheader.h"
#include "sectheader.h"
#include <stdlib.h>
#include <string.h>

ProgHeaderBlock *ProgHeaderhead = NULL;

/**
 * addProgHeader - add a program header block
 *
 * @type: type of program header entry
 * @flags: permission flags to use
 *
 * Return: 0 on success, -1 on alloc fail
 */
int addProgHeader(Elf64_Word type, Elf64_Word flags, Elf64_Off offset,
		  Elf64_Addr address, Elf64_Xword size, Elf64_Xword align)
{
	ProgHeaderBlock *ptr, *new;

	if ((new = malloc(sizeof(ProgHeaderBlock))) == NULL)
		return (-1);
	new->p_type = type;
	new->p_flags = flags;
	new->p_offset = offset;
	new->p_vaddr = address;
	new->p_paddr = address;
	new->p_filesz = size;
	new->p_memsz = size;
	new->p_align = align;
	new->next = NULL;
	if (ProgHeaderhead == NULL)
		ProgHeaderhead = new;
	else
	{
		ptr = ProgHeaderhead;
		while (ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = new;
	}
	return (0);
}

/**
 * writeProgHeadertable - write program header table to file. Currently
 * hardcoded to write the x64 entry size of 56 bytes.
 *
 * @fd: FILE* to write to
 *
 * Return: 0 on success
 */
int writeProgHeadertable(FILE *fd)
{
	ProgHeaderBlock *ptr;

	for (ptr = ProgHeaderhead; ptr != NULL; ptr = ptr->next)
	{
		fwrite(&ptr->p_type, 1, sizeof(Elf64_Word), fd);
		fwrite(&ptr->p_flags, 1, sizeof(Elf64_Word), fd);
		fwrite(&ptr->p_offset, 1, sizeof(Elf64_Off), fd);
		fwrite(&ptr->p_vaddr, 1, sizeof(Elf64_Addr), fd);
		fwrite(&ptr->p_paddr, 1, sizeof(Elf64_Addr), fd);
		fwrite(&ptr->p_filesz, 1, sizeof(Elf64_Xword), fd);
		fwrite(&ptr->p_memsz, 1, sizeof(Elf64_Xword), fd);
		fwrite(&ptr->p_align, 1, sizeof(Elf64_Xword), fd);
	}
	return (0);
}
