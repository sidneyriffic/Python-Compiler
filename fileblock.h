#ifndef FILEBLOCK_H
#define FILEBLOCK_H

#include <stdio.h>

/**
 * FileBlock - stores blocks of data to be written to files.
 *
 * @type: String representing type of data. Header, token, etc
 * @len: length of data
 * @data: pointer to data to be written
 */
typedef struct FileBlock {
	char *type;
	size_t len;
	char *data;
	struct FileBlock *next;
} FileBlock;

FileBlock *FileBlockhead;
FileBlock *FileBlocktail;

int appendFileBlock(char *type, size_t len, char *data);
int writeFileBlock(FILE *fd);

#endif
