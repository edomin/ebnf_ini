#ifndef TYPES_H
#define TYPES_H

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

#endif
