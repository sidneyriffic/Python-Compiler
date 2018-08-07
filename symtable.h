#ifndef SYMTABLE_H
#define SYMTABLE_H

/* defines for use with SymbolEntry type */
#define SYMT_REF 0
#define SYMT_STRLIT 's'
#define SYMT_VAR 'v'
#define SYMT_FUNC 'f'

/**
 * SymbolEntry - entry of symbol
 *
 * @name: name of symbol
 * @type: type of symbol entry
 * @section: section symbol data can be found in
 * @offset: offset in section of symbol
 * @size: size of entry in bytes
 * @reflist: list of references to symbol
 * @next: next symbol in list in scope
 */
typedef struct SymbolEntry
{
	char *name;
	int type;
	char *section;
	size_t offset;
	size_t size;
	SymbolEntry *reflist;
	SymbolEntry *next;
} SymbolEntry;

/**
 * SymbolScope - symbol table scope entry
 *
 * @name: name of scope (such as function name, prepended with scope names)
 * @next: next scope inside parent scope
 * @subscopelist: scopes inside this scope
 * @parent: parent scope
 * @symlist: list of symbol entries
 */
typedef struct SymbolScope
{
	char *name;
	SymbolScope *next;
	SymbolScope *subscopelist;
	SymbolScope *parent;
	SymbolEntry *symlist;
} SymbolScope;

int addsubscope(SymbolScope *current, char *name);
int addsymbol(char *scopename, char *section, char *symbolname,
	      size_t offset, size_t size);
int addsymbolref(char *scopename, char *section, char *symbolname,
		 size_t offset, size_t size);
int derefsymbols();

#endif
