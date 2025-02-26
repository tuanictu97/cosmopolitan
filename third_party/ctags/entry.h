// clang-format off
/*
*   $Id: entry.h 443 2006-05-30 04:37:13Z darren $
*
*   Copyright (c) 1998-2002, Darren Hiebert
*
*   This source code is released for free distribution under the terms of the
*   GNU General Public License.
*
*   External interface to entry.c
*/
#ifndef _ENTRY_H
#define _ENTRY_H

/*
*   INCLUDE FILES
*/
#include "third_party/ctags/general.h"  /* must always come first */

#include "libc/calls/calls.h"
#include "libc/calls/dprintf.h"
#include "libc/calls/weirdtypes.h"
#include "libc/fmt/fmt.h"
#include "libc/mem/fmt.h"
#include "libc/stdio/stdio.h"
#include "libc/stdio/temp.h"
#include "third_party/musl/tempnam.h"

#include "third_party/ctags/vstring.h"

/*
*   MACROS
*/
#define WHOLE_FILE  -1L

/*
*   DATA DECLARATIONS
*/

/*  Maintains the state of the tag file.
 */
typedef struct eTagFile {
	char *name;
	char *directory;
	FILE *fp;
	struct sNumTags { unsigned long added, prev; } numTags;
	struct sMax { size_t line, tag, file; } max;
	struct sEtags {
		char *name;
		FILE *fp;
		size_t byteCount;
	} etags;
	vString *vLine;
} tagFile;

typedef struct sTagFields {
	unsigned int count;        /* number of additional extension flags */
	const char *const *label;  /* list of labels for extension flags */
	const char *const *value;  /* list of values for extension flags */
} tagFields;

/*  Information about the current tag candidate.
 */
typedef struct sTagEntryInfo {
	boolean     lineNumberEntry;  /* pattern or line number entry */
	unsigned long lineNumber;     /* line number of tag */
	fpos_t      filePosition;     /* file position of line containing tag */
	const char* language;         /* language of source file */
	boolean     isFileScope;      /* is tag visibile only within source file? */
	boolean     isFileEntry;      /* is this just an entry for a file name? */
	boolean     truncateLine;     /* truncate tag line at end of tag name? */
	const char *sourceFileName;   /* name of source file */
	const char *name;             /* name of the tag */
	const char *kindName;         /* kind of tag */
	char        kind;             /* single character representation of kind */
	struct {
		const char* access;
		const char* fileScope;
		const char* implementation;
		const char* inheritance;
		const char* scope [2];    /* value and key */
		const char* signature;

		/* type (union/struct/etc.) and name for a variable or typedef. */
		const char* typeRef [2];  /* e.g., "struct" and struct name */

	} extensionFields;  /* list of extension fields*/
} tagEntryInfo;

/*
*   GLOBAL VARIABLES
*/
extern tagFile TagFile;

/*
*   FUNCTION PROTOTYPES
*/
extern void freeTagFileResources (void);
extern const char *tagFileName (void);
extern void copyBytes (FILE* const fromFp, FILE* const toFp, const long size);
extern void copyFile (const char *const from, const char *const to, const long size);
extern void openTagFile (void);
extern void closeTagFile (const boolean resize);
extern void beginEtagsFile (void);
extern void endEtagsFile (const char *const name);
extern void makeTagEntry (const tagEntryInfo *const tag);
extern void initTagEntry (tagEntryInfo *const e, const char *const name);

#endif  /* _ENTRY_H */

/* vi:set tabstop=4 shiftwidth=4: */
