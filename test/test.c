#include <stdio.h>

#include "ebnf_ini_internal.h"

typedef bool (IsSymbolFunc)(const char *);
typedef bool (IsRangeFunc)(const char *, const char **);
typedef bool (IsRangeWithRecordFunc)(const char *, const char **, IniRecord **);
typedef bool (IsRangeWithSectionFunc)(const char *, const char **,
 IniSection **);

void TestIsSymbol(IsSymbolFunc func, const char *testValue,
 const char *passMessage, const char *failMessage) {
    if (func(testValue))
        printf(passMessage, testValue);
    else
        printf(failMessage, testValue);
}

void TestIsRange(IsRangeFunc func, const char *testValue,
 const char *failMessage) {
    const char *p;

    if (func(testValue, &p))
        printf("'%s' -> '%s' \n", testValue, p);
    else
        printf(failMessage, testValue);
}

void TestIsRangeWithRecord(IsRangeWithRecordFunc func, const char *testValue,
 const char *failMessage) {
    const char *p;

    if (func(testValue, &p, NULL))
        printf("'%s' -> '%s' \n", testValue, p);
    else
        printf(failMessage, testValue);
}

void TestIsRangeWithSection(IsRangeWithSectionFunc func, const char *testValue,
 const char *failMessage) {
    const char *p;

    if (func(testValue, &p, NULL))
        printf("'%s' -> '%s' \n", testValue, p);
    else
        printf(failMessage, testValue);
}

void TestParser(void) {
    IniData *iniData = INI_ParseString(
     "key0=value0\n"
     "key0.1=value0.1\n"
     "key0.2=value0.2\n"
     ";comment\n"
     "[header]\n"
     "key1=value1\n"
     ";comment\n"
     "[header2]\n"
     "key2=value2\n"
     "key3=value3\n"
     ";comment\n"
    );

    (void)iniData;

    printf("INI_GetSectionsCount(iniData) - %i \n",
     INI_GetSectionsCount(iniData));

    printf("INI_GetSectionByNum(iniData, 0)->name - %s \n",
     INI_GetSectionByNum(iniData, 0)->name);
    printf("INI_GetSectionByNum(iniData, 1)->name - %s \n",
     INI_GetSectionByNum(iniData, 1)->name);

    printf("INI_GetSectionByName(iniData, NULL)->name - %s \n",
     INI_GetSectionByName(iniData, NULL)->name);
    printf("INI_GetSectionByName(iniData, \"header\")->name - %s \n",
     INI_GetSectionByName(iniData, "header")->name);
    printf("INI_GetSectionByName(iniData, \"header2\")->name - %s \n",
     INI_GetSectionByName(iniData, "header2")->name);

    printf("INI_GetSectionName(iniData, 0) - %s \n",
     INI_GetSectionName(iniData, 0));
    printf("INI_GetSectionName(iniData, 1) - %s \n",
     INI_GetSectionName(iniData, 1));
    printf("INI_GetSectionName(iniData, 2) - %s \n",
     INI_GetSectionName(iniData, 2));

    printf("INI_GetRecordsCountBySectionNum(iniData, -1) - %i \n",
     INI_GetRecordsCountBySectionNum(iniData, -1));
    printf("INI_GetRecordsCountBySectionNum(iniData, 0) - %i \n",
     INI_GetRecordsCountBySectionNum(iniData, 0));
    printf("INI_GetRecordsCountBySectionNum(iniData, 1) - %i \n",
     INI_GetRecordsCountBySectionNum(iniData, 1));
    printf("INI_GetRecordsCountBySectionNum(iniData, 2) - %i \n",
     INI_GetRecordsCountBySectionNum(iniData, 2));

    printf("INI_GetRecordsCount(iniData, NULL) - %i \n",
     INI_GetRecordsCount(iniData, NULL));
    printf("INI_GetRecordsCount(iniData, \"\") - %i \n",
     INI_GetRecordsCount(iniData, ""));
    printf("INI_GetRecordsCount(iniData, \"header\") - %i \n",
     INI_GetRecordsCount(iniData, "header"));
    printf("INI_GetRecordsCount(iniData, \"header2\") - %i \n",
     INI_GetRecordsCount(iniData, "header2"));
    printf("INI_GetRecordsCount(iniData, \"header3\") - %i \n",
     INI_GetRecordsCount(iniData, "header3"));

    printf("INI_GetRecordByNum(iniData, 0, 0)->key - %s \n",
     INI_GetRecordByNum(iniData, 0, 0)->key);
    printf("INI_GetRecordByNum(iniData, 0, 0)->value - %s \n",
     INI_GetRecordByNum(iniData, 0, 0)->value);

    printf("INI_GetKeyByNum(iniData, 0, 0) - %s \n",
     INI_GetKeyByNum(iniData, 0, 0));
    printf("INI_GetKeyByNum(iniData, 0, 666) - %s \n",
     INI_GetKeyByNum(iniData, 0, 666));
    printf("INI_GetKeyByNum(iniData, 666, 0) - %s \n",
     INI_GetKeyByNum(iniData, 666, 0));

    INI_FreeIniData(iniData);
}

int main(int argc, char **argv) {
    /* INI_IsNonnewlineSymbol */
    TestIsSymbol(
     INI_IsNonnewlineSymbol,
     "a",
     "%s is nonnewline symbol \n",
     "%s is newline symbol \n"
    );
    TestIsSymbol(
     INI_IsNonnewlineSymbol,
     "ю",
     "%s is nonnewline symbol \n",
     "%s is newline symbol \n"
    );
    TestIsSymbol(
     INI_IsNonnewlineSymbol,
     "\n",
     "%s is nonnewline symbol \n",
     "%s is newline symbol \n"
    );

    /* INI_IsWhitespaceSymbol */
    TestIsSymbol(
     INI_IsWhitespaceSymbol,
     " ",
     "'%s' is whitespace symbol \n",
     "'%s' is not whitespace symbol \n"
    );
    TestIsSymbol(
     INI_IsWhitespaceSymbol,
     "\t",
     "'%s' is whitespace symbol \n",
     "'%s' is not whitespace symbol \n"
    );
    TestIsSymbol(
     INI_IsWhitespaceSymbol,
     "a",
     "'%s' is whitespace symbol \n",
     "'%s' is not whitespace symbol \n"
    );
    TestIsSymbol(
     INI_IsWhitespaceSymbol,
     "ю",
     "'%s' is whitespace symbol \n",
     "'%s' is not whitespace symbol \n"
    );

    /* INI_IsWhitespace */
    printf(
     "%s \n", "Skipping leading whitespaces with INI_IsWhitespace function:");
    TestIsRange(
     INI_IsWhitespace,
     "    hello",
     "'%s' has not leading whitespaces \n"
    );
    TestIsRange(
     INI_IsWhitespace,
     "\t\t\t\thello",
     "'%s' has not leading whitespaces \n"
    );
    TestIsRange(
     INI_IsWhitespace,
     "  \t\thello",
     "'%s' has not leading whitespaces \n"
    );
    TestIsRange(
     INI_IsWhitespace,
     "hello",
     "'%s' has not leading whitespaces \n"
    );

    /* INI_IsComment */
    printf("%s \n", "Skipping leading comment with INI_IsComment function:");
    TestIsRange(
     INI_IsComment,
     "; comment\nhello",
     "'%s' has not leading comment \n"
    );
    TestIsRange(
     INI_IsComment,
     "    ; comment\nhello",
     "'%s' has not leading comment \n"
    );
    TestIsRange(
     INI_IsComment,
     "    ; Комментарий\nhello",
     "'%s' has not leading comment \n"
    );
    TestIsRange(
     INI_IsComment,
     "comment\nhello",
     "'%s' has not leading comment \n"
    );

    /* INI_IsNewline */
    printf("%s \n", "Skipping leading newline with INI_IsNewline function:");
    TestIsRange(
     INI_IsNewline,
     "\nhello",
     "'%s' has not leading newline \n"
    );
    TestIsRange(
     INI_IsNewline,
     "    \nhello",
     "'%s' has not leading newline \n"
    );
    TestIsRange(
     INI_IsNewline,
     "hello",
     "'%s' has not leading newline \n"
    );

    /* INI_IsCommentOrNewline */
    printf(
     "%s \n",
     "Skipping leading comment or newline with INI_IsCommentOrNewline function:"
    );
    TestIsRange(
     INI_IsCommentOrNewline,
     "; comment\nhello",
     "'%s' has not leading comment or newline \n"
    );
    TestIsRange(
     INI_IsCommentOrNewline,
     "    ; comment\nhello",
     "'%s' has not leading comment or newline \n"
    );
    TestIsRange(
     INI_IsCommentOrNewline,
     "\nhello",
     "'%s' has not leading comment or newline \n"
    );
    TestIsRange(
     INI_IsCommentOrNewline,
     "comment\nhello",
     "'%s' has not leading comment or newline \n"
    );

    /* INI_IsNotreservedSymbol */
    TestIsSymbol(
     INI_IsNotreservedSymbol,
     "a",
     "'%s' is notreserved symbol \n",
     "'%s' is reserved symbol \n"
    );
    TestIsSymbol(
     INI_IsNotreservedSymbol,
     "ю",
     "'%s' is notreserved symbol \n",
     "'%s' is reserved symbol \n"
    );
    TestIsSymbol(
     INI_IsNotreservedSymbol,
     " ",
     "'%s' is notreserved symbol \n",
     "'%s' is reserved symbol \n"
    );
    TestIsSymbol(
     INI_IsNotreservedSymbol,
     "\t",
     "'%s' is notreserved symbol \n",
     "'%s' is reserved symbol \n"
    );
    TestIsSymbol(
     INI_IsNotreservedSymbol,
     ";",
     "'%s' is notreserved symbol \n",
     "'%s' is reserved symbol \n"
    );
    TestIsSymbol(
     INI_IsNotreservedSymbol,
     "=",
     "'%s' is notreserved symbol \n",
     "'%s' is reserved symbol \n"
    );

    /* INI_IsNoncommentSymbol */
    TestIsSymbol(
     INI_IsNoncommentSymbol,
     "a",
     "'%s' is noncomment symbol \n",
     "'%s' is comment symbol \n"
    );
    TestIsSymbol(
     INI_IsNoncommentSymbol,
     "ю",
     "'%s' is noncomment symbol \n",
     "'%s' is comment symbol \n"
    );
    TestIsSymbol(
     INI_IsNoncommentSymbol,
     ";",
     "'%s' is noncomment symbol \n",
     "'%s' is comment symbol \n"
    );

    /* INI_IsNonemptySymbol */
    TestIsSymbol(
     INI_IsNonemptySymbol,
     "\t",
     "'%s' is nonempty symbol \n",
     "'%s' is empty symbol \n"
    );
    TestIsSymbol(
     INI_IsNonemptySymbol,
     ";",
     "'%s' is nonempty symbol \n",
     "'%s' is empty symbol \n"
    );
    TestIsSymbol(
     INI_IsNonemptySymbol,
     "=",
     "'%s' is nonempty symbol \n",
     "'%s' is empty symbol \n"
    );

    /* INI_IsKey */
    printf("%s \n", "Skipping leading key with INI_IsKey function:");
    TestIsRange(
     INI_IsKey,
     "key=value",
     "'%s' has not leading key \n"
    );
    TestIsRange(
     INI_IsKey,
     "key = value",
     "'%s' has not leading key \n"
    );

    /* INI_IsValue */
    printf("%s \n", "Skipping leading value with INI_IsValue function:");
    TestIsRange(
     INI_IsValue,
     "value\n",
     "'%s' has not leading value \n"
    );
    TestIsRange(
     INI_IsValue,
     "value \n",
     "'%s' has not leading value \n"
    );
    TestIsRange(
     INI_IsValue,
     "value;comment\n",
     "'%s' has not leading value \n"
    );
    TestIsRange(
     INI_IsValue,
     "value ;comment\n",
     "'%s' has not leading value \n"
    );

    /* INI_IsKeyValue */
    printf("%s \n", "Skipping leading key-value with INI_IsKeyValue function:");
    TestIsRangeWithRecord(
     INI_IsKeyValue,
     "key=value\n",
     "'%s' has not leading key-value \n"
    );
    TestIsRangeWithRecord(
     INI_IsKeyValue,
     "key = value\n",
     "'%s' has not leading key-value \n"
    );
    TestIsRangeWithRecord(
     INI_IsKeyValue,
     "key=value;comment\n",
     "'%s' has not leading key-value \n"
    );
    TestIsRangeWithRecord(
     INI_IsKeyValue,
     "key=\n",
     "'%s' has not leading key-value \n"
    );
    TestIsRangeWithRecord(
     INI_IsKeyValue,
     "key=;comment\n",
     "'%s' has not leading key-value \n"
    );
    TestIsRangeWithRecord(
     INI_IsKeyValue,
     "key = ;comment\n",
     "'%s' has not leading key-value \n"
    );
    TestIsRangeWithRecord(
     INI_IsKeyValue,
     ";comment\n",
     "'%s' has not leading key-value \n"
    );

    /* INI_IsEmptyLine */
    printf("%s \n",
     "Skipping leading empty lines with INI_IsEmptyLine function:");
    TestIsRange(
     INI_IsEmptyLine,
     "\nkey=value",
     "'%s' has not leading empty line \n"
    );
    TestIsRange(
     INI_IsEmptyLine,
     "\n    key=value",
     "'%s' has not leading empty line \n"
    );
    TestIsRange(
     INI_IsEmptyLine,
     ";comment\nkey=value",
     "'%s' has not leading empty line \n"
    );
    TestIsRange(
     INI_IsEmptyLine,
     ";comment\n key=value",
     "'%s' has not leading empty line \n"
    );
    TestIsRange(
     INI_IsEmptyLine,
     "key=value",
     "'%s' has not leading empty line \n"
    );

    /* INI_IsSectionHeader */
    printf("%s \n",
     "Skipping leading section header with INI_IsSectionHeader function:");
    TestIsRange(
     INI_IsSectionHeader,
     "[header]\nkey=value",
     "'%s' has not leading section header \n"
    );
    TestIsRange(
     INI_IsSectionHeader,
     "[header];comment\nkey=value",
     "'%s' has not leading section header \n"
    );
    TestIsRange(
     INI_IsSectionHeader,
     "[header]\n;comment\nkey=value",
     "'%s' has not leading section header \n"
    );
    TestIsRange(
     INI_IsSectionHeader,
     "header\n;comment\nkey=value",
     "'%s' has not leading section header \n"
    );

    /* INI_IsSectionLine */
    printf("%s \n",
     "Skipping leading section line with INI_IsSectionLine function:");
    TestIsRangeWithRecord(
     INI_IsSectionLine,
     "\nkey=value",
     "'%s' has not leading section line \n"
    );
    TestIsRangeWithRecord(
     INI_IsSectionLine,
     ";comment\nkey=value",
     "'%s' has not leading section line \n"
    );
    TestIsRangeWithRecord(
     INI_IsSectionLine,
     "key=value;comment\nkey=value",
     "'%s' has not leading section line \n"
    );
    TestIsRangeWithRecord(
     INI_IsSectionLine,
     "key=value\nkey=value",
     "'%s' has not leading section line \n"
    );
    TestIsRangeWithRecord(
     INI_IsSectionLine,
     "=value\nkey=value",
     "'%s' has not leading section line \n"
    );

    /* INI_IsHeadlessSection */
    printf("%s \n",
     "Skipping leading headless section with INI_IsHeadlessSection function:");
    TestIsRangeWithSection(
     INI_IsHeadlessSection,
     "key=value\nkey2=value2\n[header]",
     "'%s' has not leading headless section \n"
    );
    TestIsRangeWithSection(
     INI_IsHeadlessSection,
     "key=value\n;comment\n[header]",
     "'%s' has not leading headless section \n"
    );
    TestIsRangeWithSection(
     INI_IsHeadlessSection,
     "[header]key=value\n;comment\n[header2]",
     "'%s' has not leading headless section \n"
    );

    /* INI_IsSection */
    printf("%s \n", "Skipping leading section with INI_IsSection function:");
    TestIsRangeWithSection(
     INI_IsSection,
     "[header]\nkey=value\nkey2=value2\n[header2]",
     "'%s' has not leading section \n"
    );
    TestIsRangeWithSection(
     INI_IsSection,
     "[header]\nkey=value\n;comment\n[header2]",
     "'%s' has not leading section \n"
    );
    TestIsRangeWithSection(
     INI_IsSection,
     "[Заголовок]\nКлюч=Значение\n;Комментарий\n[Заголовок2]",
     "'%s' has not leading section \n"
    );
    TestIsRangeWithSection(
     INI_IsSection,
     "key=value\n;comment\n[header2]",
     "'%s' has not leading section \n"
    );

    TestParser();

    return 0;
}
