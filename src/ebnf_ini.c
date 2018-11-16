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
            p++;
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
        p++;
    else
        return false;

    while (INI_IsNonnewlineSymbol(p))
        p++;

    UC_Utf8Copy((uint8_t *)symbol, (const uint8_t *)p);
    if (strcmp(symbol, "\n") == 0)
        p++;
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
        p++;
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
            p++;
        while (INI_IsNotreservedSymbol(p));
    } else
        return false;
    *end = p;
    return true;
}

bool INI_IsValue(const char *p, const char **end) {
    if (INI_IsNotreservedSymbol(p)) {
        do
            p++;
        while (INI_IsNoncommentSymbol(p));
        do
            p--;
        while (!INI_IsNonemptySymbol(p));
        p++;
    } else
        return false;
    *end = p;
    return true;
}

bool INI_IsKeyValue(const char *p, const char **end) {
    char        symbol[7];
    const char *tempEnd;

    if (INI_IsWhitespace(p, &tempEnd))
        p = tempEnd;

    if (INI_IsKey(p, &tempEnd))
        p = tempEnd;
    else
        return false;

    if (INI_IsWhitespace(p, &tempEnd))
        p = tempEnd;

    UC_Utf8Copy((uint8_t *)symbol, (const uint8_t *)p);
    if (strcmp(symbol, "=") == 0)
        p++;
    else
        return false;

    if (INI_IsWhitespace(p, &tempEnd))
        p = tempEnd;

    if (INI_IsValue(p, &tempEnd))
        p = tempEnd;

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
        p++;
    else
        return false;

    if (INI_IsNotreservedSymbol(p)) {
        do
            p++;
        while (INI_IsNotreservedSymbol(p));
    } else
        return false;

    UC_Utf8Copy((uint8_t *)symbol, (const uint8_t *)p);
    if (strcmp(symbol, "]") == 0)
        p++;
    else
        return false;

    *end = p;
    return true;
}

bool INI_IsSectionLine(const char *p, const char **end) {
    const char *tempEnd;

    if (INI_IsEmptyLine(p, &tempEnd))
        p = tempEnd;
    else {
        if (INI_IsKeyValue(p, &tempEnd))
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

bool INI_IsHeadlessSection(const char *p, const char **end) {
    const char *tempEnd;

    if (INI_IsSectionLine(p, &tempEnd))
        do
            p = tempEnd;
        while (INI_IsSectionLine(p, &tempEnd));
    else
        return false;

    *end = p;
    return true;
}

bool INI_IsSection(const char *p, const char **end) {
    const char *tempEnd;

    if (INI_IsSectionHeader(p, &tempEnd))
        p = tempEnd;
    else
        return false;

    if (INI_IsCommentOrNewline(p, &tempEnd))
        p = tempEnd;
    else
        return false;

    while (INI_IsSectionLine(p, &tempEnd))
        p = tempEnd;

    *end = p;
    return true;
}

bool INI_IsIni(const char *p) {
    const char *tempEnd;

    if (INI_IsHeadlessSection(p, &tempEnd))
        p = tempEnd;

    while (INI_IsSection(p, &tempEnd))
        p = tempEnd;

    return true;
}
