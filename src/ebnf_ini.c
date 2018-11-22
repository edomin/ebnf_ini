#include <stdint.h>
#include <string.h>

#include "uconvert.h"

#include "ebnf_ini_internal.h"

void INI_Parse(const char *text) {

}

bool INI_IsNonnewlineSymbol(const char *p) {
    char symbol[7];

    UC_Utf8Copy((uint8_t *)symbol, (const uint8_t *)p);
    if (strcmp(symbol, "\n") != 0)
        return true;
    else
        return false;
}

bool INI_IsWhitespaceSymbol(const char *p) {
    char symbol[7];

    UC_Utf8Copy((uint8_t *)symbol, (const uint8_t *)p);
    if ((strcmp(symbol, " ") == 0) || ((strcmp(symbol, "\t") == 0)))
        return true;
    else
        return false;
}

bool INI_IsWhitespace(const char *p, const char **end) {
    if (INI_IsWhitespaceSymbol(p)) {
        do
            p = (const char *)UC_StringUtf8NextCodepoint((uint8_t *)p);
        while (INI_IsWhitespaceSymbol(p));
    } else
        return false;
    *end = p;
    return true;
}

bool INI_IsComment(const char *p, const char **end) {
    const char *tempEnd;
    char        symbol[7];

    if (INI_IsWhitespace(p, &tempEnd))
        p = tempEnd;

    UC_Utf8Copy((uint8_t *)symbol, (const uint8_t *)p);
    if (strcmp(symbol, ";") == 0)
        p = (const char *)UC_StringUtf8NextCodepoint((uint8_t *)p);
    else
        return false;

    while (INI_IsNonnewlineSymbol(p))
        p = (const char *)UC_StringUtf8NextCodepoint((uint8_t *)p);

    UC_Utf8Copy((uint8_t *)symbol, (const uint8_t *)p);
    if (strcmp(symbol, "\n") == 0)
        p = (const char *)UC_StringUtf8NextCodepoint((uint8_t *)p);
    else
        return false;

    *end = p;
    return true;
}

bool INI_IsNewline(const char *p, const char **end) {
    const char *tempEnd;
    char        symbol[7];

    if (INI_IsWhitespace(p, &tempEnd))
        p = tempEnd;

    UC_Utf8Copy((uint8_t *)symbol, (const uint8_t *)p);
    if (strcmp(symbol, "\n") == 0)
        p = (const char *)UC_StringUtf8NextCodepoint((uint8_t *)p);
    else
        return false;

    *end = p;
    return true;
}

bool INI_IsCommentOrNewline(const char *p, const char **end) {
    const char *tempEnd;

    if (INI_IsComment(p, &tempEnd)) {
        *end = tempEnd;
        return true;
    }

    if (INI_IsNewline(p, &tempEnd)) {
        *end = tempEnd;
        return true;
    }

    return false;
}

bool INI_IsNotreservedSymbol(const char *p) {
    char symbol[7];

    if (!INI_IsNonnewlineSymbol(p))
        return false;

    UC_Utf8Copy((uint8_t *)symbol, (const uint8_t *)p);
    if ((strcmp(symbol, " ") != 0) &&
     (strcmp(symbol, "\t") != 0) &&
     (strcmp(symbol, ";") != 0) &&
     (strcmp(symbol, "=") != 0) &&
     (strcmp(symbol, "[") != 0) &&
     (strcmp(symbol, "]") != 0)
    )
        return true;
    else
        return false;
}

bool INI_IsNoncommentSymbol(const char *p) {
    char symbol[7];

    if (!INI_IsNonnewlineSymbol(p))
        return false;

    UC_Utf8Copy((uint8_t *)symbol, (const uint8_t *)p);
    if (strcmp(symbol, ";") != 0)
        return true;
    else
        return false;
}

bool INI_IsNonemptySymbol(const char *p) {
    char symbol[7];

    if (!INI_IsNonnewlineSymbol(p))
        return false;

    UC_Utf8Copy((uint8_t *)symbol, (const uint8_t *)p);
    if ((strcmp(symbol, " ") != 0) &&
     (strcmp(symbol, "\t") != 0) &&
     (strcmp(symbol, ";") != 0)
    )
        return true;
    else
        return false;
}

bool INI_IsKey(const char *p, const char **end) {
    if (INI_IsNotreservedSymbol(p)) {
        do
            p = (const char *)UC_StringUtf8NextCodepoint((uint8_t *)p);
        while (INI_IsNotreservedSymbol(p));
    } else
        return false;
    *end = p;
    return true;
}

bool INI_IsValue(const char *p, const char **end) {
    if (INI_IsNotreservedSymbol(p)) {
        do
            p = (const char *)UC_StringUtf8NextCodepoint((uint8_t *)p);
        while (INI_IsNoncommentSymbol(p));
        do
            p = (const char *)UC_StringUtf8PreviousCodepoint((uint8_t *)p);
        while (!INI_IsNonemptySymbol(p));
        p = (const char *)UC_StringUtf8NextCodepoint((uint8_t *)p);
    } else
        return false;
    *end = p;
    return true;
}

bool INI_IsKeyValue(const char *p, const char **end, IniRecord **record) {
    char         symbol[7];
    const char  *tempEnd;
    IniRecord  **currentRecord;
    const char  *keyBegin;
    const char  *keyEnd;
    ptrdiff_t    keyLen;
    const char  *valueBegin;
    const char  *valueEnd;
    ptrdiff_t    valueLen;

    if (INI_IsWhitespace(p, &tempEnd))
        p = tempEnd;

    if (INI_IsKey(p, &tempEnd)) {
        keyBegin = p;
        keyEnd = tempEnd;
        keyLen = keyEnd - keyBegin;
        p = tempEnd;
    } else
        return false;

    if (INI_IsWhitespace(p, &tempEnd))
        p = tempEnd;

    UC_Utf8Copy((uint8_t *)symbol, (const uint8_t *)p);
    if (strcmp(symbol, "=") == 0)
        p = (const char *)UC_StringUtf8NextCodepoint((uint8_t *)p);
    else
        return false;

    if (INI_IsWhitespace(p, &tempEnd))
        p = tempEnd;

    if (INI_IsValue(p, &tempEnd)) {
        valueBegin = p;
        valueEnd = tempEnd;
        valueLen = valueEnd - valueBegin;
        p = tempEnd;
    }

    if (record != NULL) {
        currentRecord = record;
        while (*currentRecord != NULL)
            currentRecord = &((*currentRecord)->next);

        *currentRecord = malloc(sizeof(IniRecord));
        (*currentRecord)->next = NULL;

        (*currentRecord)->key = malloc(keyLen + 1);
        strncpy((*currentRecord)->key, keyBegin, keyLen);
        (*currentRecord)->key[keyLen] = '\0';

        (*currentRecord)->value = malloc(valueLen + 1);
        strncpy((*currentRecord)->value, valueBegin, valueLen);
        (*currentRecord)->value[valueLen] = '\0';
    }

    *end = p;
    return true;
}

bool INI_IsEmptyLine(const char *p, const char **end) {
    const char *tempEnd;

    if (INI_IsWhitespace(p, &tempEnd))
        p = tempEnd;

    if (INI_IsCommentOrNewline(p, &tempEnd))
        p = tempEnd;
    else
        return false;

    *end = p;
    return true;
}

bool INI_IsSectionHeader(const char *p, const char **end) {
    char        symbol[7];

    UC_Utf8Copy((uint8_t *)symbol, (const uint8_t *)p);
    if (strcmp(symbol, "[") == 0)
        p = (const char *)UC_StringUtf8NextCodepoint((uint8_t *)p);
    else
        return false;

    if (INI_IsNotreservedSymbol(p)) {
        do
            p = (const char *)UC_StringUtf8NextCodepoint((uint8_t *)p);
        while (INI_IsNotreservedSymbol(p));
    } else
        return false;

    UC_Utf8Copy((uint8_t *)symbol, (const uint8_t *)p);
    if (strcmp(symbol, "]") == 0)
        p = (const char *)UC_StringUtf8NextCodepoint((uint8_t *)p);
    else
        return false;

    *end = p;
    return true;
}

bool INI_IsSectionLine(const char *p, const char **end, IniRecord **record) {
    const char *tempEnd;

    if (INI_IsEmptyLine(p, &tempEnd))
        p = tempEnd;
    else {
        if (INI_IsKeyValue(p, &tempEnd, record))
            p = tempEnd;
        else
            return false;

        if (INI_IsCommentOrNewline(p, &tempEnd))
            p = tempEnd;
        else
            return false;
    }

    *end = p;
    return true;
}

bool INI_IsHeadlessSection(const char *p, const char **end,
 IniSection **headlessSection) {
    const char *tempEnd;
    IniRecord **firstRecord = NULL;

    if (headlessSection != NULL) {
        *headlessSection = malloc(sizeof(IniSection));
        (*headlessSection)->name = NULL;
        (*headlessSection)->firstRecord = NULL;
        firstRecord = &(*headlessSection)->firstRecord;
    }

    if (INI_IsSectionLine(p, &tempEnd, firstRecord))
        do
            p = tempEnd;
        while (INI_IsSectionLine(p, &tempEnd, firstRecord));
    else
        return false;

    *end = p;
    return true;
}

bool INI_IsSection(const char *p, const char **end, IniSection **section) {
    const char *tempEnd;
    const char *sectionHeaderBegin = p;
    const char *sectionHeaderEnd;
    ptrdiff_t   sectionHeaderLen;
    IniRecord **firstRecord = NULL;

    if (INI_IsSectionHeader(p, &tempEnd))
        p = tempEnd;
    else
        return false;

    sectionHeaderEnd = p;

    /* Skipping open and close brackets */
    sectionHeaderBegin = (const char *)UC_StringUtf8NextCodepoint((uint8_t *)sectionHeaderBegin);
    sectionHeaderEnd = (const char *)UC_StringUtf8PreviousCodepoint((uint8_t *)sectionHeaderEnd);

    sectionHeaderLen = sectionHeaderEnd - sectionHeaderBegin;

    if (INI_IsCommentOrNewline(p, &tempEnd))
        p = tempEnd;
    else
        return false;

    if (section != NULL) {
        *section = malloc(sizeof(IniSection));
        (*section)->name = malloc(sectionHeaderLen + 1);
        strncpy((*section)->name, sectionHeaderBegin, sectionHeaderLen);
        (*section)->name[sectionHeaderLen] = '\0';
        (*section)->firstRecord = NULL;
        firstRecord = &(*section)->firstRecord;
        // printf("#%s# \n", (*section)->name);
    }

    while (INI_IsSectionLine(p, &tempEnd, firstRecord))
        p = tempEnd;

    *end = p;
    return true;
}

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

