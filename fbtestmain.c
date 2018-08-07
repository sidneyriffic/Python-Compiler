#include "elfheader.h"
#include "progheader.h"
#include "sectheader.h"
#include "symtable.h"
#include <stdio.h>

/**
 * main 
 */
int main()
{
	SymbolScope *scope;
	initelfheader();

	addSectHeader("text", 0, 0);
	addSectHeader("rodata", 0, 0);
	scope = getSymbolScope("global", NULL);
	/* write(1, "Hello symbols!", 15); */
	addSymbolEntry(scope, "rodata", "mystrlit", 's',
		       15, "Hello symbols!");     /* string literal symbol */

	appendsectdata("text",
		       "\xb8\x01\x00\x00\x00"         /* mov $0x01,%eax */
		       "\xba\x0e\x00\x00\x00"         /* mov $0x0f,%edx */
		       "\x48\x8d\x35", 13);           /* lea - */
	addSymbolRef(scope, "text", "mystrlit", 4);   /* - 0x??(%rip),rsi */
	appendsectdata("text", "\xbf\x01\x00\x00\x00" /* mov $0x01,%edi */
		       "\x0f\x05", 7);                /*syscall*/

	/* exit(30) */
	appendsectdata("text", "\xb8\x3c\x00\x00\x00" /* mov $0x3c,%eax*/
		       "\xbf\x1e\x00\x00\00"          /* mov $0x1e,%edi */
		       "\x0f\x05", 12);               /*syscall*/
	setelfoffsize();
	derefsymbols();
	writeelf(stdout);
	return (1);
}
