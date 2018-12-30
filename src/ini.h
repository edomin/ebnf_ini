#ifndef INI_H
#define INI_H

#include <stddef.h> /* for ptrdiff_t */

#include "types.h"

IniData *INI_ParseString(const char *string);

int INI_GetSectionsCount(IniData *iniData);

IniSection *INI_GetSectionByNum(IniData *iniData, int sectionNum);
IniSection *INI_GetSectionByName(IniData *iniData, const char *sectionName);
char *INI_GetSectionName(IniData *iniData, int sectionNum);
int INI_GetRecordsCountBySectionNum(IniData *iniData, int sectionNum);
int INI_GetRecordsCount(IniData *iniData, const char *sectionName);

IniRecord *INI_GetRecordByNum(IniData *iniData, int sectionNum, int recordNum);
const char *INI_GetKeyByNum(IniData *iniData, int sectionNum, int recordNum);
//const char *INI_GetKey(IniData *iniData, const char *sectionName,
 //int recordNum);

void INI_FreeIniRecord(IniRecord *iniRecord);
void INI_FreeIniSection(IniSection *iniSection);
void INI_FreeIniData(IniData *iniData);
#endif
