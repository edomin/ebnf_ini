#include "ebnf_check.h"

#include <string.h>

#include "uconvert.h"

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
    sectionHeaderBegin = (const char *)UC_StringUtf8NextCodepoint(
     (uint8_t *)sectionHeaderBegin);
    sectionHeaderEnd = (const char *)UC_StringUtf8PreviousCodepoint(
     (uint8_t *)sectionHeaderEnd);

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
    }

    while (INI_IsSectionLine(p, &tempEnd, firstRecord))
        p = tempEnd;

    *end = p;
    return true;
}
