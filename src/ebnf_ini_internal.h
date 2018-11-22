#ifndef EBNF_INI_INTERNAL_H
#define EBNF_INI_INTERNAL_H

#include <stdbool.h> /* for bool */
#include <stddef.h> /* for ptrdiff_t */
#include <stdio.h> /* FOR DEBUG OUTPUT */

typedef struct IniRecord {
    char             *key;
    char             *value;
    struct IniRecord *next;
} IniRecord;

typedef struct IniSection {
    char              *name;
    IniRecord         *firstRecord;
    struct IniSection *next;
} IniSection;

typedef struct IniData {
    IniSection *headlessSection;
    IniSection *firstSection;
} IniData;

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

IniData *INI_ParseString(const char *string);

int INI_GetSectionsCount(IniData *iniData);

IniSection *INI_GetSectionByNum(IniData *iniData, int sectionNum);
IniSection *INI_GetSectionByName(IniData *iniData, const char *sectionName);
char *INI_GetSectionName(IniData *iniData, int sectionNum);
int INI_GetRecordsCountBySectionNum(IniData *iniData, int sectionNum);
int INI_GetRecordsCount(IniData *iniData, const char *sectionName);

IniRecord *INI_GetRecordByNum(IniData *iniData, int sectionNum, int recordNum);
const char *INI_GetKeyByNum(IniData *iniData, int sectionNum, int recordNum);

void INI_FreeIniRecord(IniRecord *iniRecord);
void INI_FreeIniSection(IniSection *iniSection);
void INI_FreeIniData(IniData *iniData);
#endif
