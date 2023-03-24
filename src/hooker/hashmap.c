#include <hooker/hashmap.h>


struct entry {
    char sym[SYM_MAX_LEN];
    unsigned long rva;
    UT_hash_handle hh;
};

struct entry *table = NULL;


void load_hashmap_from_file(const char *filename)
{
    FILE *file = fopen(filename, "rb");
    if (!file) {
        file = fopen(filename, "wb");
        if (!file) {
            fprintf(stderr, "Failed to create file %s\n", filename);
            return;
        }
        fclose(file);
        return;
    }

    free_hashmap();

    struct entry entry;
    while (fread(&entry, sizeof(entry), 1, file) == 1) {
        struct entry *e = (struct entry *) malloc(sizeof(struct entry));
        strcpy(e->sym, entry.sym);
        e->rva = entry.rva;
        HASH_ADD_STR(table, sym, e);
    }

    fclose(file);
}

void save_hashmap_to_file(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file)
        return;

    struct entry *e;
    for (e = table; e != NULL; e = (struct entry *) e->hh.next)
        fwrite(e, sizeof(struct entry), 1, file);

    fclose(file);
}

void add_entry(const char *sym, unsigned int rva)
{
    struct entry *e = (struct entry *) malloc(sizeof(struct entry));
    strcpy(e->sym, sym);
    e->rva = rva;
    HASH_ADD_STR(table, sym, e);
}

unsigned int get_rva_from_hashmap(const char *sym)
{
    struct entry *e;
    HASH_FIND_STR(table, sym, e);
    if (e)
        return e->rva;
    else
        return -1;
}

void free_hashmap(void)
{
    struct entry *e, *tmp;
    HASH_ITER(hh, table, e, tmp)
    {
        HASH_DEL(table, e);
        free(e);
    }
}
