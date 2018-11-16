#include <stdio.h>

#include "ebnf_ini_internal.h"

int main(int argc, char **argv) {
    const char *p;

    if (INI_IsNonnewlineSymbol("a"))
        printf("%s \n", "a is nonnewline symbol");
    else
        printf("%s \n", "a is newline symbol");

    if (INI_IsNonnewlineSymbol("ю"))
        printf("%s \n", "ю is nonnewline symbol");
    else
        printf("%s \n", "ю is newline symbol");

    if (INI_IsNonnewlineSymbol("\n"))
        printf("%s \n", "newline is nonnewline symbol");
    else
        printf("%s \n", "newline is newline symbol");

    if (INI_IsWhitespaceSymbol(" "))
        printf("%s \n", "' ' is whitespace symbol");
    else
        printf("%s \n", "' ' is not whitespace symbol");

    if (INI_IsWhitespaceSymbol("\t"))
        printf("%s \n", "'\t' is whitespace symbol");
    else
        printf("%s \n", "'\t' is not whitespace symbol");

    if (INI_IsWhitespaceSymbol("a"))
        printf("%s \n", "'a' is whitespace symbol");
    else
        printf("%s \n", "'a' is not whitespace symbol");

    if (INI_IsWhitespaceSymbol("ю"))
        printf("%s \n", "'ю' is whitespace symbol");
    else
        printf("%s \n", "'ю' is not whitespace symbol");

    printf(
     "%s \n", "Skipping leading whitespaces with INI_IsWhitespace function:");
    if (INI_IsWhitespace("    hello", &p))
        printf("'%s' -> '%s' \n", "    hello", p);
    else
        printf("'%s' has not leading whitespaces \n", "    thello");
    if (INI_IsWhitespace("\t\t\t\thello", &p))
        printf("'%s' -> '%s' \n", "\t\t\t\thello", p);
    else
        printf("'%s' has not leading whitespaces \n", "\t\t\t\thello");
    if (INI_IsWhitespace("  \t\thello", &p))
        printf("'%s' -> '%s' \n", "  \t\thello", p);
    else
        printf("'%s' has not leading whitespaces \n", "  \t\thello");
    if (INI_IsWhitespace("thello", &p))
        printf("'%s' -> '%s' \n", "hello", p);
    else
        printf("'%s' has not leading whitespaces \n", "hello");

    printf("%s \n", "Skipping leading comment with INI_IsComment function:");
    if (INI_IsComment("; comment\nhello", &p))
        printf("'%s'\n->\n'%s' \n", "; comment\nhello", p);
    else
        printf("'%s' has not leading comment \n", "; comment\nhello");
    if (INI_IsComment("    ; comment\nhello", &p))
        printf("'%s'\n->\n'%s' \n", "    ; comment\nhello", p);
    else
        printf("'%s' has not leading comment \n", "    ; comment\nhello");
    if (INI_IsComment("comment\nhello", &p))
        printf("'%s'\n->\n'%s' \n", "comment\nhello", p);
    else
        printf("'%s' has not leading comment \n", "comment\nhello");

    printf("%s \n", "Skipping leading newline with INI_IsNewline function:");
    if (INI_IsNewline("\nhello", &p))
        printf("'%s'\n->\n'%s' \n", "\nhello", p);
    else
        printf("'%s' has not leading newline \n", "\nhello");
    if (INI_IsNewline("    \nhello", &p))
        printf("'%s'\n->\n'%s' \n", "    \nhello", p);
    else
        printf("'%s' has not leading newline \n", "    \nhello");
    if (INI_IsNewline("hello", &p))
        printf("'%s'\n->\n'%s' \n", "hello", p);
    else
        printf("'%s' has not leading newline \n", "hello");

    printf("%s \n", "Skipping leading comment or newline with INI_IsCommentOrNewline function:");
    if (INI_IsCommentOrNewline("; comment\nhello", &p))
        printf("'%s'\n->\n'%s' \n", "; comment\nhello", p);
    else
        printf("'%s' has not leading comment or newline \n", "; comment\nhello");
    if (INI_IsCommentOrNewline("    ; comment\nhello", &p))
        printf("'%s'\n->\n'%s' \n", "    ; comment\nhello", p);
    else
        printf("'%s' has not leading comment or newline \n", "    ; comment\nhello");
    if (INI_IsCommentOrNewline("\nhello", &p))
        printf("'%s'\n->\n'%s' \n", "\nhello", p);
    else
        printf("'%s' has not leading comment or newline \n", "\nhello");
    if (INI_IsCommentOrNewline("comment\nhello", &p))
        printf("'%s'\n->\n'%s' \n", "comment\nhello", p);
    else
        printf("'%s' has not leading comment or newline \n", "comment\nhello");

    if (INI_IsNotreservedSymbol("a"))
        printf("%s \n", "'a' is notreserved symbol");
    else
        printf("%s \n", "'a' is reserved symbol");

    if (INI_IsNotreservedSymbol("ю"))
        printf("%s \n", "'ю' is notreserved symbol");
    else
        printf("%s \n", "'ю' is reserved symbol");

    if (INI_IsNotreservedSymbol(" "))
        printf("%s \n", "' ' is notreserved symbol");
    else
        printf("%s \n", "' ' is reserved symbol");

    if (INI_IsNotreservedSymbol("\t"))
        printf("%s \n", "'\t' is notreserved symbol");
    else
        printf("%s \n", "'\t' is reserved symbol");

    if (INI_IsNotreservedSymbol(";"))
        printf("%s \n", "';' is notreserved symbol");
    else
        printf("%s \n", "';' is reserved symbol");

    if (INI_IsNotreservedSymbol("="))
        printf("%s \n", "'=' is notreserved symbol");
    else
        printf("%s \n", "'=' is reserved symbol");

    if (INI_IsNoncommentSymbol("a"))
        printf("%s \n", "'a' is noncomment symbol");
    else
        printf("%s \n", "'a' is comment symbol");

    if (INI_IsNoncommentSymbol("ю"))
        printf("%s \n", "'ю' is noncomment symbol");
    else
        printf("%s \n", "'ю' is comment symbol");

    if (INI_IsNoncommentSymbol(";"))
        printf("%s \n", "';' is noncomment symbol");
    else
        printf("%s \n", "';' is comment symbol");

    if (INI_IsNonemptySymbol("\t"))
        printf("%s \n", "'\t' is nonempty symbol");
    else
        printf("%s \n", "'\t' is empty symbol");

    if (INI_IsNonemptySymbol(";"))
        printf("%s \n", "';' is nonempty symbol");
    else
        printf("%s \n", "';' is empty symbol");

    if (INI_IsNonemptySymbol("="))
        printf("%s \n", "'=' is nonempty symbol");
    else
        printf("%s \n", "'=' is empty symbol");

    printf("%s \n", "Skipping leading key with INI_IsKey function:");
    if (INI_IsKey("key=value", &p))
        printf("'%s' -> '%s' \n", "key=value", p);
    else
        printf("'%s' has not leading key \n", "key=value");
    if (INI_IsKey("key = value", &p))
        printf("'%s' -> '%s' \n", "key = value", p);
    else
        printf("'%s' has not leading key \n", "key = value");

    printf("%s \n", "Skipping leading value with INI_IsValue function:");
    if (INI_IsValue("value\n", &p))
        printf("'%s' -> '%s' \n", "value\n", p);
    else
        printf("'%s' has not leading key \n", "value\n");
    if (INI_IsValue("value \n", &p))
        printf("'%s' -> '%s' \n", "value \n", p);
    else
        printf("'%s' has not leading key \n", "value \n");
    if (INI_IsValue("value;comment\n", &p))
        printf("'%s' -> '%s' \n", "value;comment\n", p);
    else
        printf("'%s' has not leading key \n", "value;comment\n");
    if (INI_IsValue("value ;comment\n", &p))
        printf("'%s' -> '%s' \n", "value ;comment\n", p);
    else
        printf("'%s' has not leading key \n", "value ;comment\n");

    printf("%s \n", "Skipping leading key-value with INI_IsKeyValue function:");
    if (INI_IsKeyValue("key=value\n", &p))
        printf("'%s' -> '%s' \n", "key=value\n", p);
    else
        printf("'%s' has not leading key-value \n", "key=value\n");
    if (INI_IsKeyValue("key = value\n", &p))
        printf("'%s' -> '%s' \n", "key = value\n", p);
    else
        printf("'%s' has not leading key-value \n", "key = value\n");
    if (INI_IsKeyValue("key=value;comment\n", &p))
        printf("'%s' -> '%s' \n", "key=value;comment\n", p);
    else
        printf("'%s' has not leading key-value \n", "key=value;comment\n");
    if (INI_IsKeyValue("key=\n", &p))
        printf("'%s' -> '%s' \n", "key=\n", p);
    else
        printf("'%s' has not leading key-value \n", "key=\n");
    if (INI_IsKeyValue("key=;comment\n", &p))
        printf("'%s' -> '%s' \n", "key=;comment\n", p);
    else
        printf("'%s' has not leading key-value \n", "key=;comment\n");
    if (INI_IsKeyValue("key = ;comment\n", &p))
        printf("'%s' -> '%s' \n", "key = ;comment\n", p);
    else
        printf("'%s' has not leading key-value \n", "key = ;comment\n");
    if (INI_IsKeyValue(";comment\n", &p))
        printf("'%s' -> '%s' \n", ";comment\n", p);
    else
        printf("'%s' has not leading key-value \n", ";comment\n");

    printf("%s \n", "Skipping leading empty lines with INI_IsEmptyLine function:");
    if (INI_IsEmptyLine("\nkey=value", &p))
        printf("'%s' -> '%s' \n", "\nkey=value", p);
    else
        printf("'%s' has not leading key-value \n", "\nkey=value");
    if (INI_IsEmptyLine("\n    key=value", &p))
        printf("'%s' -> '%s' \n", "\n    key=value", p);
    else
        printf("'%s' has not leading key-value \n", "\n    key=value");
    if (INI_IsEmptyLine(";comment\nkey=value", &p))
        printf("'%s' -> '%s' \n", ";comment\nkey=value", p);
    else
        printf("'%s' has not leading key-value \n", ";comment\nkey=value");
    if (INI_IsEmptyLine(";comment\n key=value", &p))
        printf("'%s' -> '%s' \n", ";comment\n key=value", p);
    else
        printf("'%s' has not leading key-value \n", ";comment\n key=value");
    if (INI_IsEmptyLine("key=value", &p))
        printf("'%s' -> '%s' \n", "key=value", p);
    else
        printf("'%s' has not leading key-value \n", "key=value");

    printf("%s \n", "Skipping leading section header with INI_IsSectionHeader function:");
    if (INI_IsSectionHeader("[header]\nkey=value", &p))
        printf("'%s' -> '%s' \n", "[header]\nkey=value", p);
    else
        printf("'%s' has not leading section header \n", "[header]\nkey=value");
    if (INI_IsSectionHeader("[header];comment\nkey=value", &p))
        printf("'%s' -> '%s' \n", "[header];comment\nkey=value", p);
    else
        printf("'%s' has not leading section header \n", "[header];comment\nkey=value");
    if (INI_IsSectionHeader("[header]\n;comment\nkey=value", &p))
        printf("'%s' -> '%s' \n", "[header]\n;comment\nkey=value", p);
    else
        printf("'%s' has not leading section header \n", "[header]\n;comment\nkey=value");
    if (INI_IsSectionHeader("header\n;comment\nkey=value", &p))
        printf("'%s' -> '%s' \n", "header\n;comment\nkey=value", p);
    else
        printf("'%s' has not leading section header \n", "header\n;comment\nkey=value");

    printf("%s \n", "Skipping leading section line with INI_IsSectionLine function:");
    if (INI_IsSectionLine("\nkey=value", &p))
        printf("'%s' -> '%s' \n", "\nkey=value", p);
    else
        printf("'%s' has not leading section line \n", "\nkey=value");
    if (INI_IsSectionLine(";comment\nkey=value", &p))
        printf("'%s' -> '%s' \n", ";comment\nkey=value", p);
    else
        printf("'%s' has not leading section line \n", ";comment\nkey=value");
    if (INI_IsSectionLine("key=value;comment\nkey=value", &p))
        printf("'%s' -> '%s' \n", "key=value;comment\nkey=value", p);
    else
        printf("'%s' has not leading section line \n", "key=value;comment\nkey=value");
    if (INI_IsSectionLine("key=value\nkey=value", &p))
        printf("'%s' -> '%s' \n", "key=value\nkey=value", p);
    else
        printf("'%s' has not leading section line \n", "key=value\nkey=value");
    if (INI_IsSectionLine("=value\nkey=value", &p))
        printf("'%s' -> '%s' \n", "=value\nkey=value", p);
    else
        printf("'%s' has not leading section line \n", "=value\nkey=value");

    printf("%s \n", "Skipping leading headless section with INI_IsSectionLine function:");
    if (INI_IsHeadlessSection("key=value\nkey2=value2\n[header]", &p))
        printf("'%s' -> '%s' \n", "key=value\nkey2=value2\n[header]", p);
    else
        printf("'%s' has not leading headless section \n", "key=value\nkey2=value2\n[header]");
    if (INI_IsHeadlessSection("key=value\n;comment\n[header]", &p))
        printf("'%s' -> '%s' \n", "key=value\n;comment\n[header]", p);
    else
        printf("'%s' has not leading headless section \n", "key=value\n;comment\n[header]");
    if (INI_IsHeadlessSection("[header]key=value\n;comment\n[header2]", &p))
        printf("'%s' -> '%s' \n", "[header]key=value\n;comment\n[header2]", p);
    else
        printf("'%s' has not leading headless section \n", "[header]key=value\n;comment\n[header2]");

    printf("%s \n", "Skipping leading section with INI_IsSectionLine function:");
    if (INI_IsSection("[header]\nkey=value\nkey2=value2\n[header2]", &p))
        printf("'%s' -> '%s' \n", "[header]\nkey=value\nkey2=value2\n[header2]", p);
    else
        printf("'%s' has not leading section \n", "[header]\nkey=value\nkey2=value2\n[header2]");
    if (INI_IsSection("[header]\nkey=value\n;comment\n[header2]", &p))
        printf("'%s' -> '%s' \n", "[header]\nkey=value\n;comment\n[header2]", p);
    else
        printf("'%s' has not leading section \n", "[header]\nkey=value\n;comment\n[header2]");
    if (INI_IsSection("key=value\n;comment\n[header2]", &p))
        printf("'%s' -> '%s' \n", "key=value\n;comment\n[header2]", p);
    else
        printf("'%s' has not leading section \n", "key=value\n;comment\n[header2]");

    return 0;
}
