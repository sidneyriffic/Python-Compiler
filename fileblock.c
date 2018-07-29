#include "fileblock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FileBlock *FileBlockhead;
extern FileBlock *FileBlocktail;

/**
 * appendFileBlock - append a FileBlock to the list.
 * If this fails, you probably want to stop as out of memory.
 *
 * @type: string. type of block appended
 * @len: length of block appended
 * @data: data to append.
 *
 * Return: 0 if successful, -1 on alloc error
 */
int appendFileBlock(char *type, size_t len, char *data)
{
	FileBlock *newblock;

	if (type == NULL || len < 1 || data == NULL)
		return (-1);
	newblock = malloc(sizeof(FileBlock));
	if (newblock == NULL)
		return (-1);
	newblock->type = strdup(type);
	if (newblock->type == NULL)
	{
		free(newblock);
		return (-1);
	}
	newblock->len = len;
	newblock->data = strndup(data, len);
	if (newblock->data == NULL)
	{
		free(newblock->type);
		free(newblock);
		return (-1);
	}
	if (FileBlockhead == NULL)
	{
		FileBlockhead = FileBlocktail = newblock;
	}
	else
	{
		FileBlocktail->next = newblock;
		FileBlocktail = newblock;
	}
	return (0);
}

/**
 * writeFileBlock - write file block list to the given File
 *
 * @fd: File to write to
 *
 * Return: 1 if successful (all bytes written), 0 if not
 */
int writeFileBlock(FILE *fd)
{
	size_t bytes;
	FileBlock *head = FileBlockhead;

	for (; head != NULL; head = head->next)
	{
		bytes = fwrite(head->data, 1, head->len, fd);
		if (bytes != head->len)
			return (-1);
	}
	return (0);
}
