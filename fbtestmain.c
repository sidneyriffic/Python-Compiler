#include "elfheader.h"
#include "progheader.h"
#include "sectheader.h"
#include <stdio.h>

/**
 * main 
 */
int main()
{
	initelfheader();
	
	addSectHeader("text", 0, 0);
	appendsectdata("text", "\xb8\x3c\x00\x00\x00"/* mov $0x3c,%eax*/
		       "\xbf\x1e\x00\x00\00"/* mov $0x3c,%edi */
		       "\x0f\x05", 12);/*syscall*/
	setelfoffsize();
	writeelf(stdout);
	return (1);
}
