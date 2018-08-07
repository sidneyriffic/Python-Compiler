#include "symtable.h"
#include "platdefines.h"
#include <string.h>
#include <stdlib.h>

SymbolScope globalscope = { "global", NULL, NULL, NULL, NULL };
SymbolList *headSymbolList = NULL;

/**
 * getSymbolScope - get scope address from symbol table. Searches from given scope
 * down through children.
 *
 * @scopename: name of scope
 * @scope: scope address to start searching at. NULL for global
 *
 * Return: address of SymboleScope, null if not found
 */
SymbolScope *getSymbolScope(char *scopename, SymbolScope *scope)
{
	SymbolScope *ptr, *ret;

	if (scope == NULL)
		scope = &globalscope;
	if (!strcmp(scopename, scope->name))
		return (scope);
	for (ptr = scope->subscopelist; ptr != NULL; ptr = ptr->next)
	{
		ret = getSymbolScope(scopename, scope);
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
	SymbolEntry *new, *symptr;
	SectData *dataptr;
	SymbolList *listptr, *newlist;

	if ((new = malloc(sizeof(SymbolEntry))) == NULL)
		return (-1);
	dataptr = appendsectdata(section, data, size);
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
		return (-1);
	}
	if ((newlist = malloc(sizeof(SymbolList))) == NULL)
	{
		free(dataptr);
		free(new->name);
		free(new);
		return (-1);
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
		for (symptr = scope->symlist; symptr->next != NULL;)
			symptr = symptr->next;
		symptr->next = new;
	}
	newlist->symbol = new;
	newlist->next = NULL;
	if (headSymbolList == NULL)
		headSymbolList = newlist;
	else
	{
		for(listptr = headSymbolList; listptr->next != NULL;)
			listptr = listptr->next;
		listptr->next = newlist;
	}
	return (0);
}

/**
 * addSymbolEntryref - add a reference to a symbol defined by top level
 * SymbolEntry. Checks all parent scopes for reference of same name.
 *
 * @scope: scope reference is in
 * @section: section ref is in
 * @symbolname: name of symbol being referenced
 * 
 * Return: 0 if added successfully, -1 on alloc fail
 */
int addSymbolRef(SymbolScope *scope, char *section, char *symbolname)
{
	SymbolEntry *ptr;
	SymbolRef *refptr, *new;
	SectData *dataptr;

	for (scope; scope != NULL; scope = scope->parent)
		for (ptr = scope->symlist; ptr != NULL; ptr = ptr->next)
			if (!strcmp(symbolname,  ptr->name))
			    break;

	if ((new = malloc(sizeof(SymbolEntry))) == NULL)
		return (-1);
	if (ptr->type == 's')
		dataptr = appendsectdata(section, "\x00\x00\x00\x00"
					 "\x00\x00\x00\x00", ADDRSIZE);
	if (dataptr == NULL)
	{
		free(new);
		return (-1);
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
	return (0);
}

/**
 * dereffileaddrsym - dereferences a symbol to a file address
 *
 * Return: 0 on success
 */
int dereffileaddrsym(SymbolEntry *symbol, SymbolRef *ref)
{
	size_t offset = 0;
	SectData *symptr, *refptr;

	symptr = symbol->dataptr;
	refptr = ref->dataptr;
	fprintf(stderr, "%lu, %lu, %lu, %lu\n", refptr->offset, refptr->parent->sh_offset, symptr->offset, symptr->parent->sh_offset);
	offset = (symptr->offset + symptr->parent->sh_offset) -
		(refptr->offset + refptr->parent->sh_offset + refptr->len);
	memcpy(refptr->data, (char *) &offset, ADDRSIZE);
	return (0);
}

/**
 * derefsymbols - insert relevant data into all symbol references
 *
 * Return: 0 on success
 */
int derefsymbols()
{
	SymbolList *ptr = headSymbolList;
	SymbolRef *refptr;

	for (ptr = headSymbolList; ptr != NULL; ptr = ptr->next)
		for (refptr = ptr->symbol->reflist; refptr != NULL;
		     refptr = refptr->next)
		{
			if (ptr->symbol->type == 's')
				dereffileaddrsym(ptr->symbol, refptr);
		}
	return (0);
}
