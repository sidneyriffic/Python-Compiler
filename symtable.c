#include "symtable.h"
#include "platdefines.h"
#include <string.h>

SymbolScope globalscope = { "global", NULL, NULL, NULL, NULL };

/**
 * getscope - get scope address from symbol table. Searches from given scope
 * down through children.
 *
 * @scopename: name of scope
 * @scope: scope address to start searching at. NULL for global
 *
 * Return: address of SymboleScope, null if not found
 */
SymbolScope getscope(char *scopename, SymbolScope *scope)
{
	SymbolScope *ptr, *ret;
	if (scope == NULL)
		scope = globalscope;
	if (!strcmp(scopename, scope->name))
		return (scope);
	for (ptr = scope->subscopelist; ptr != NULL; ptr = ptr->next)
	{
		ret = getscope(scopename, scope);
		if (ret != NULL)
			return (ret);
	}
}

/**
 * addsymbol - add a symbol to symbol table
 *
 * @scopename: scope to add symbol to
 * @section: section symbol is found in
 * @symbolname: name of symbol
 * @type: type of symbol ('v'ar, 'f'unc, 'r'ef, 's'tring literal, etc)
 * @size: size of symbol in bytes. address references should be sizeof address
 *
 * Return: 0 on success, -1 on alloc fail
 */
int addSymbolEntry(SymbolScope *scope, char *section, char *symbolname,
		   int type, size_t size, char *data)
{
	SectHeaderBlock *ptr = getSectHeader(section);
	SymbolEntry *new, *ptr;
	SectData *dataptr;

	if (ptr == NULL)
		return (1);

	if ((new = malloc(sizeof(SymbolEntry))) == NULL)
		return (-1);
	dataptr = appendsectdata(section, data, len);
	if (dataptr == NULL)
	{
		free(new);
		return (-1);
	}
	new->name = strdup(symbolname);
	if (new->name == NULL)
	{
		free(dataptr);
		free(new);
		return (NULL);
	}
	new->type = type;
	new->dataptr = dataptr;
	new->reflist = NULL;
	new->next = NULL;
	new->scope = scope;
	if (scope->symlist == NULL)
		scope->symlist = new;
	else
	{
		for (ptr = scope->symlist; ptr->next != NULL;)
			ptr = ptr->next;
		ptr->next = new;
	}
	return (0);
}

/**
 * addSymbolEntryref - add a reference to a symbol defined by top level
 * SymbolEntry. Checks all parent scopes for reference of same name.
 *
 * @scope: scope reference is in
 * 
 * Return: 0 if added successfully, -1 on alloc fail, 1 on success
 */
int addSymbolRef(SymbolScope scope, char *section, char *symbolname)
{
	SymbolEntry *ptr, *new;
	SymbolRef *refptr;
	SectData dataptr;

	for (scope; scope != NULL; scope = scope->parent)
		for (ptr = scope->symlist; ptr != NULL; ptr = ptr->next)
			if (!strcmp(symbolname,  ptr->name))
			    break;

	if ((new = malloc(sizeof(SymbolEntry)) == NULL))
		return (NULL);
	if (ptr->type == 's')
		dataptr = appendsectdata(section, "        ", ADDRSIZE);
	if (dataptr == NULL)
	{
		free(new);
		return (NULL);
	}
	new->name = ptr->name;
	new->dataptr = dataptr;
	new->next = NULL;
	new->scope = scope;
	if (ptr->reflist == NULL)
		ptr->reflist = new;
	else
	{
		for(refptr = ptr->reflist; refptr->next != NULL;)
			refptr = refptr->next;
		refptr->next = new;
	}
	return (1);
}
