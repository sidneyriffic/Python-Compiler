#ifndef SYMTABLE_H
#define SYMTABLE_H

/* defines for use with SymbolEntry type */
#define SYMT_STRLIT 's'
#define SYMT_VAR 'v'
#define SYMT_FUNC 'f'

#include "sectheader.h"

struct SymbolScope;

/**
 * SymbolRef - reference of symbol
 *
 * @name: name of symbol
 * @dataptr: SectData entry of symbol/reference
 * @offend: offset to end of instruction for %rip offset
 * @next: next symbol in list in scope
 * @scope: scope the entry is in
 */
typedef struct SymbolRef
{
	char *name;
	SectData *dataptr;
	size_t offend;
	struct SymbolRef *next;
	struct SymbolScope *scope;
} SymbolRef;


/**
 * SymbolEntry - entry of symbol
 *
 * @name: name of symbol
 * @type: type of symbol entry
 * @dataptr: SectData entry of symbol/reference
 * @reflist: list of references to symbol
 * @next: next symbol in list in scope
 * @scope: scope the entry is in
 */
typedef struct SymbolEntry
{
	char *name;
	int type;
	SectData *dataptr;
	SymbolRef *reflist;
	struct SymbolEntry *next;
	struct SymbolScope *scope;
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
	struct SymbolScope *next;
	struct SymbolScope *subscopelist;
	struct SymbolScope *parent;
	SymbolEntry *symlist;
} SymbolScope;

/**
 * SymbolList - holds all symbol entries for quick & easy dereference pass
 *
 * @symbol: symbol entry
 * @next: next symbol in list
 */
typedef struct SymbolList
{
	SymbolEntry *symbol;
	struct SymbolList *next;
} SymbolList;

SymbolScope *getSymbolScope(char *scopename, SymbolScope *scope);
SymbolScope *addsubscope(SymbolScope *current, char *name);
int addSymbolScope(char *scopename, char *symbolname, int type,
		   size_t offset, size_t size);
int addSymbolEntry(SymbolScope *scope, char *section, char *symbolname,
		   int type, size_t size, char *data);
int addSymbolRef(SymbolScope *scope, char *section, char *symbolname,
		 size_t offend);
int derefsymbols();

#endif
