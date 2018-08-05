#include "sectheader.h"
#include <string.h>
#include <stdlib.h>

extern SectHeaderBlock *SectHeaderhead;

/**
 * addSectHeader - add a new section header entry. If it exists, do nothing.
 *
 * return: 0 on success, 1 if exists, -1 on alloc fail
 */
int addSectHeader(char *name, Elf64_Word type, Elf64_Xword flags)
{
	SectHeaderBlock *new, *ptr;
	for(new = SectHeaderhead; new != NULL; new = new->next)
		if (!strcmp(name, new->name))
			return (1);
	if ((new = malloc(sizeof(SectHeaderBlock))) == NULL)
		return (-1);
	new->name = strdup(name);
	if (new->name == NULL)
	{
		free(new);
		return (-1);
	}
	new->type = type;
	new->flags = flags;
	new->next = NULL;
	new->datahead = NULL;
	if (SectHeaderhead == NULL)
		SectHeaderhead = new;
	else
	{
		for (ptr = SectHeaderhead; ptr->next != NULL; ptr = ptr->next)
			;
		ptr->next = new;
	}
}

/**
 * appendsectdata - appends data to a section's data list
 *
 * @section: name of section, without leading .
 * @data: data to put into section
 * @len: length of data in bytes
 *
 * Return: 0 on success, 1 if section not found, -1 on alloc fail
 */
int appendsectdata(char *section, char *data, size_t len)
{
	SectHeaderBlock *ptr, *new;

	for (ptr = SectHeaderhead; strcmp(ptr->name, section) && ptr != NULL;)
		ptr = ptr->next;
	if (ptr == NULL)
		return (1);

	if ((new = malloc(sizeof(SectionData))) == NULL)
		return (-1);
	if ((new->data = malloc(len)) == NULL)
	{
		free(new);
		return (-1);
	}
	strncpy(new->data, data, len);
	new->len = len;
	new->next = NULL;
	if (ptr->data == NULL)
		ptr->data = new;
	else
	{
		for(ptr = ptr->data; ptr->next != NULL; ptr = ptr->next)
			;
		ptr->next = new;
	}
	return (0);
}
