#include "ini.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "ebnf_check.h"

IniData *INI_ParseString(const char *string) {
    const char  *tempEnd;
    IniData     *iniData;
    IniSection **currentSection;

    iniData = malloc(sizeof(IniData));

    if (INI_IsHeadlessSection(string, &tempEnd, &(iniData->headlessSection)))
        string = tempEnd;

    currentSection = &(iniData->firstSection);

    while (INI_IsSection(string, &tempEnd, currentSection)) {
        currentSection = &((*currentSection)->next);
        string = tempEnd;
    }

    return iniData;
}

void INI_FreeIniRecord(IniRecord *iniRecord) {
    if (iniRecord->next != NULL)
        INI_FreeIniRecord(iniRecord->next);
    if (iniRecord->key != NULL)
        free(iniRecord->key);
    if (iniRecord->value != NULL)
        free(iniRecord->value);
    free(iniRecord);
}

void INI_FreeIniSection(IniSection *iniSection) {
    if (iniSection->next != NULL)
        INI_FreeIniSection(iniSection->next);
    if (iniSection->firstRecord != NULL)
        INI_FreeIniRecord(iniSection->firstRecord);
    if (iniSection->name != NULL)
        free(iniSection->name);
    free(iniSection);
}

void INI_FreeIniData(IniData *iniData) {
    if (iniData->headlessSection != NULL)
        INI_FreeIniSection(iniData->headlessSection);
    if (iniData->firstSection != NULL)
        INI_FreeIniSection(iniData->firstSection);
    free(iniData);
}

int INI_GetSectionsCount(IniData *iniData) {
    int         sectionsCount;
    IniSection *currentSection;

    if (iniData->firstSection != NULL) {
        currentSection = iniData->firstSection;
        sectionsCount = 1;
    } else
        return 0;

    while (currentSection->next != NULL) {
        currentSection = currentSection->next;
        sectionsCount++;
    }

    return sectionsCount;
}

IniSection *INI_GetSectionByNum(IniData *iniData, int sectionNum) {
    int         currentSectionNum = 0;
    IniSection *currentSection = iniData->firstSection;

    if (sectionNum >= INI_GetSectionsCount(iniData))
        return NULL;

    if (sectionNum == -1)
        return iniData->headlessSection;

    while (currentSectionNum != sectionNum) {
        currentSection = currentSection->next;
        currentSectionNum++;
    }

    return currentSection;
}

IniSection *INI_GetSectionByName(IniData *iniData, const char *sectionName) {
    int i;

    if ((sectionName == NULL) || (strcmp(sectionName, "") == 0))
        return iniData->headlessSection;

    for (i = 0; i < INI_GetSectionsCount(iniData); i++) {
        IniSection *currentSection = INI_GetSectionByNum(iniData, i);

        if (strcmp(currentSection->name, sectionName) == 0)
            return currentSection;
    }

    return NULL;
}

char *INI_GetSectionName(IniData *iniData, int sectionNum) {
    IniSection *section = INI_GetSectionByNum(iniData, sectionNum);

    if (section != NULL)
        return section->name;
    else
        return NULL;
}

int INI_GetRecordsCountBySectionNum(IniData *iniData, int sectionNum) {
    int         recordsCount;
    IniSection *section = INI_GetSectionByNum(iniData, sectionNum);
    IniRecord  *currentRecord;

    if (section == NULL)
        return 0;

    if (section->firstRecord != NULL) {
        currentRecord = section->firstRecord;
        recordsCount = 1;
    } else
        return 0;

    while (currentRecord->next != NULL) {
        currentRecord = currentRecord->next;
        recordsCount++;
    }

    return recordsCount;
}

int INI_GetRecordsCount(IniData *iniData, const char *sectionName) {
    int i;

    if ((sectionName == NULL) || (strcmp(sectionName, "") == 0))
        return INI_GetRecordsCountBySectionNum(iniData, -1);

    for (i = 0; i < INI_GetSectionsCount(iniData); i++) {
        char *currentSectionName = INI_GetSectionName(iniData, i);

        if (strcmp(currentSectionName, sectionName) == 0)
            return INI_GetRecordsCountBySectionNum(iniData, i);
    }

    return 0;
}

IniRecord *INI_GetRecordByNum(IniData *iniData, int sectionNum, int recordNum) {
    IniSection *section = INI_GetSectionByNum(iniData, sectionNum);
    int         currentRecordNum = 0;
    IniRecord  *currentRecord;

    if (section == NULL)
        return NULL;

    currentRecord = section->firstRecord;

    if (recordNum >= INI_GetRecordsCountBySectionNum(iniData, sectionNum))
        return NULL;

    while (currentRecordNum != recordNum) {
        currentRecord = currentRecord->next;
        currentRecordNum++;
    }

    return currentRecord;
}

const char *INI_GetKeyByNum(IniData *iniData, int sectionNum, int recordNum) {
    IniRecord *record = INI_GetRecordByNum(iniData, sectionNum, recordNum);

    if (record == NULL)
        return NULL;

    return record->key;
}

//const char *INI_GetKey(IniData *iniData, const char *sectionName,
 //int recordNum) {
    //IniSection *section = INI_GetSectionByName(iniData, sectionName);

    //if (section == NULL)
        //return NULL;



//}
