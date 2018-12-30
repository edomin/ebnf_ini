#ifndef EBNF_CHECK_H
#define EBNF_CHECK_H

#include <stdbool.h> /* for bool */

#include "ini.h"

bool INI_IsNonnewlineSymbol(const char *p);
bool INI_IsWhitespaceSymbol(const char *p);
bool INI_IsWhitespace(const char *p, const char **end);
bool INI_IsComment(const char *p, const char **end);
bool INI_IsNewline(const char *p, const char **end);
bool INI_IsCommentOrNewline(const char *p, const char **end);
bool INI_IsNotreservedSymbol(const char *p);
bool INI_IsNoncommentSymbol(const char *p);
bool INI_IsNonemptySymbol(const char *p);
bool INI_IsKey(const char *p, const char **end);
bool INI_IsValue(const char *p, const char **end);
bool INI_IsKeyValue(const char *p, const char **end, IniRecord **record);
bool INI_IsEmptyLine(const char *p, const char **end);
bool INI_IsSectionHeader(const char *p, const char **end);
bool INI_IsSectionLine(const char *p, const char **end, IniRecord **record);
bool INI_IsHeadlessSection(const char *p, const char **end,
 IniSection **headlessSection);
bool INI_IsSection(const char *p, const char **end, IniSection **section);

#endif
