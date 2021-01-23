#ifndef RC_H_SEEN
#define RC_H_SEEN

struct symbol_array {
    char **symbols;
    int len;
};


char *get_rcfile(void);
int create_rcfile(const char *rcfile);
struct symbol_array *read_rcfile(void);
void delete_symbol_array(struct symbol_array *symbols);

#endif
