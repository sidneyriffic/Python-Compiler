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
		  Elf64_Addr address, Elf64_Xword size)
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
	new->p_align = 0;
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
