#include "sectheader.h"
#include <string.h>
#include <stdlib.h>

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
	SectData *dptr;

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
	/* currently unused values set to 0 */
	sh_link = sh_info = sh_addralign = sh_entsize = 0;
	if (ptr->data == NULL)
		ptr->data = new;
	else
	{
		for(dptr = ptr->data; dptr->next != NULL; dptr = dptr->next)
			;
		dptr->next = new;
	}
	return (0);
}

/**
 * sizeSectHeaders - set size variables for all section headers
 *
 * Return: 0 if successful
 */
int sizeSectHeaders()
{
	size_t len;
	SectHeaderBlock *ptr;
	SectData *dptr;

	for (ptr = SectHeaderhead; ptr != NULL; ptr = ptr->next)
	{
		for (dptr = ptr->data; dptr != NULL; dptr = dptr->next)
			len += dptr->len;
		ptr->sh_size = len;
	}
	return (0);
}
