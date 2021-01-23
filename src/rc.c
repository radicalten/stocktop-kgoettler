#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "rc.h"

const char *DEFAULT_SYMBOLS[] = {
    "AAPL",
    "GOOG",
    "TSLA",
    "WORK"
};

char *get_rcfile(void)
{
    const char *rcstem = "/.stockrc";
    /* Check home directory */
    const char *homedir;
    if ((homedir = getenv("XDG_CONFIG_HOME")) == NULL) {
        if ((homedir = getenv("HOME")) == NULL) {
            homedir = getpwuid(getuid())->pw_dir;
        }
    }
    char *rcfile = malloc(sizeof(char) * (strlen(homedir) + strlen(rcstem) + 1));
    strcpy(rcfile, homedir);
    strcat(rcfile, rcstem);
    return rcfile;
}

int create_rcfile(const char *rcfile)
{
    FILE *fp;
    fp = fopen(rcfile, "w");
    int num = sizeof(DEFAULT_SYMBOLS) / sizeof(DEFAULT_SYMBOLS[0]);
    for (int i = 0; i < num; i++)
    {
        fprintf(fp, "%s\n", DEFAULT_SYMBOLS[i]);
    }
    fclose(fp);
    return 0;
}

struct symbol_array *read_rcfile(void)
{
    FILE *fp;
    char *rcfile;
    rcfile = get_rcfile();
    /* Check if file exists */
    if (access(rcfile, F_OK) != 0) 
    {
        printf("Yes");
        create_rcfile(rcfile);
    }
    /* Try to open file */
    if ((fp = fopen(rcfile, "r")) == NULL)
    {
        fprintf(stderr, "Could not open rcfile (%s) for reading\n", rcfile);
        free(rcfile);
        exit(1);
    }
    /* Read file */
    int len = 5;
    int n = 0;
    char **symbols = malloc(sizeof(char *) * len);
    char buf[10];
    while (fgets(buf, 10, fp) != NULL)
    {
        buf[strcspn(buf, "\n")] = 0;
        if (n >= len)
        {
            len *= 2;
            symbols = realloc(symbols, sizeof(char *) * len);
        }
        symbols[n] = malloc(sizeof(char) * strlen(buf));
        strcpy(symbols[n], buf);
        n += 1;
    }
    fclose(fp);
    struct symbol_array *out = malloc(sizeof(struct symbol_array));
    out->symbols = symbols;
    out->len = n;
    /* Cleanup */
    free(rcfile);
    return out;
}

void delete_symbol_array(struct symbol_array *symbols)
{
    for (int i = 0; i < symbols->len; i++)
    {
        free(symbols->symbols[i]);
    }
    free(symbols->symbols);
    free(symbols);
    return;
}
