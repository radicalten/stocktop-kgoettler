#ifndef RC_H_SEEN
#define RC_H_SEEN

typedef struct SymbolArray {
    char **symbols;
    int len;
} SymbolArray;


char *get_rcfile(void);
int create_rcfile(const char *rcfile);

SymbolArray *SymbolArray_FromFile(void);
void SymbolArray_Delete(SymbolArray *symbols);

#endif
