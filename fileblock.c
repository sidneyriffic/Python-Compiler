#include "fileblock.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FileBlock *FileBlockhead;
extern FileBlock *FileBlocktail;

/**
 * appendFileBlock - append a FileBlock to the list
 *
 * @type: string. type of block appended
 * @len: length of block appended
 * @data: data to append
 *
 * Return: 1 if successful, 0 otherwise
 */
int appendFileBlock(char *type, size_t len, char *data)
{
	FileBlock *newblock;

	if (type == NULL || len < 1 || data == NULL)
		return (0);
	newblock = malloc(sizeof(FileBlock));
	if (newblock == NULL)
		return (0);
	newblock->type = strdup(type);
	if (newblock->type == NULL)
	{
		free(newblock);
		return (0);
	}
	newblock->len = len;
	newblock->data = strdup(data);
	if (newblock->data == NULL)
	{
		free(newblock->type);
		free(newblock);
		return (0);
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
	return (1);
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
			return (0);
	}
}
