#include "sectheader.h"
#include <string.h>
#include <stdlib.h>

SectHeaderBlock *SectHeaderhead = NULL;

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
	new->sh_type = type;
	new->sh_flags = flags;
	new->sh_size = 0;
	new->next = NULL;
	new->data = NULL;
	/* currently unused values set to 0 */
	new->sh_link = new->sh_info = new->sh_addralign = new->sh_entsize = 0;
	new->sh_name = 0;
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
 * Return: address of new SectData on success, NULL otherwise
 */
SectData *appendsectdata(char *section, char *data, size_t len)
{
	SectHeaderBlock *ptr = SectHeaderhead;
	SectData *dptr, *new;
	size_t offset = 0;

	while (strcmp(ptr->name, section) && ptr != NULL)
		ptr = ptr->next;
	if (ptr == NULL)
		return (NULL);
	if ((new = malloc(sizeof(SectData))) == NULL)
		return (NULL);
	if ((new->data = malloc(len)) == NULL)
	{
		free(new);
		return (NULL);
	}
	memcpy(new->data, data, 16);
	new->len = len;
	new->next = NULL;
	new->parent = ptr;
	if (ptr->data == NULL)
		ptr->data = new;
	else
	{
		for(dptr = ptr->data; dptr->next != NULL; dptr = dptr->next)
			offset += dptr->offset;
		offset += dptr->len;
		dptr->next = new;
	}
	new->offset = offset;
	return (new);
}

/**
 * sizeSectHeaders - set size variables for all section headers
 *
 * Return: 0 if successful
 */
int sizeSectHeaders()
{
	size_t len = 0;
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

/**
 * getSectHeader - get a pointer to a section header block by name
 *
 * @name: name of the section, omitting leading .
 *
 * Return: pointer to SectHeaderBlock, NULL if not found
 */
SectHeaderBlock *getSectHeader(char *name)
{
	SectHeaderBlock *ptr;

	for (ptr = SectHeaderhead; ptr != NULL; ptr = ptr->next)
		if (!strcmp(ptr->name, name))
			return (ptr);
	return (NULL);
}

/**
 * writeSectData - write data from a section to file
 *
 * @name: name of section, omitting the .
 * @fd: FILE* to write to
 *
 * Return: 0 if successful
 */
int writeSectData(char *name, FILE *fd)
{
	SectData *ptr = getSectHeader(name)->data;

	for (;ptr != NULL; ptr = ptr->next)
		fwrite(ptr->data, 1, ptr->len, fd);
	return (0);
}
