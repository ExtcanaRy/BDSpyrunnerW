#include <hooker/hook.h>

typedef struct {
    const char *sym;
    char *line;
} symbol_cache_entry_t;

static symbol_cache_entry_t symbol_cache[SYM_CACHE_SIZE] = {0};

static unsigned int hash_str(const char *str)
{
    unsigned int hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash % SYM_CACHE_SIZE;
}

bool hook_func(void *hook_func, void *detour_func, void *original_func)
{
    if (MH_CreateHook(hook_func, detour_func, (LPVOID *)original_func) != MH_OK)
    {
        printf("Failed to create func hook, RVA: %llu \n", (uintptr_t)hook_func);
        return false;
    }
    if (MH_EnableHook(hook_func) != MH_OK)
    {
        printf("Failed to enable func hook, RVA: %llu \n", (uintptr_t)hook_func);
        return false;
    }
    return true;
}

void *get_rva_func(unsigned int rva)
{
    uintptr_t base_addr = (uintptr_t)GetModuleHandle(NULL);
    return (void *)(base_addr + rva + 4096);
}

bool add_to_sym_cache_file(const char* line)
{
    FILE *fp = fopen(SYM_CACHE_FILE, "a+");
    if (!fp)
        return false;

    fprintf(fp, "%s", line);
    fclose(fp);
    return true;
}

bool search_sym_cache_file(const char* sym, char** line_out)
{
    FILE *fp = fopen(SYM_CACHE_FILE, "r");
    if (!fp)
        return false;

    const size_t MAX_LINE_LENGTH = 4096;
    char *line = malloc(MAX_LINE_LENGTH);
    if (!line)
    {
        fclose(fp);
        return false;
    }

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL)
    {
        if (strstr(line, sym))
        {
            *line_out = _strdup(line);
            fclose(fp);
            free(line);
            return true;
        }
    }

    fclose(fp);
    free(line);
    return false;
}

void *dlsym(const char *sym)
{
    static bool is_sym_file_generated = false;

    char *rva_val_str = NULL;
    unsigned int rva_val = 0;

    unsigned int sym_hash = hash_str(sym);
    symbol_cache_entry_t *cache_entry = &symbol_cache[sym_hash];
    if (cache_entry->sym && strcmp(cache_entry->sym, sym) == 0)
    {
        char *split_str = malloc(strlen(cache_entry->sym));
        strncpy(split_str, cache_entry->line, strlen(cache_entry->sym));
        rva_val_str = strtok(split_str, ":");
        rva_val_str = strtok(NULL, ", ");
        sscanf(rva_val_str, "[%*x:%x]", &rva_val);
        free(split_str);
        return get_rva_func(rva_val);
    }

    char* cached_line = NULL;
    if (search_sym_cache_file(sym, &cached_line))
    {
        rva_val_str = strtok(cached_line, ":");
        rva_val_str = strtok(NULL, ", ");
        sscanf(rva_val_str, "[%*x:%x]", &rva_val);
        free(cached_line);
        cache_entry->sym = sym;
        cache_entry->line = _strdup(cached_line);
        return get_rva_func(rva_val);
    }

    FILE *fp = fopen(SYM_FILE, "r");
    if (!fp)
    {
        if (!release_cvdump_exe())
            return NULL;

        printf("Symbol file " SYM_FILE " not found, trying to generate.\n");

        system(CVDUMP_EXE_PATH CVDUMP_EXEC_ARGS BDS_PDB_PATH " > " SYM_FILE );
        
        fp = fopen(SYM_FILE, "r");
        if (!fp)
        {
            if (!is_sym_file_generated)
            {
                printf("Failed to generate. \n");
                is_sym_file_generated = true;
            }
            printf("The symbol %s will not be parsed. \n", sym);
            return NULL;
        }
    }

    const size_t MAX_LINE_LENGTH = 4096;
    char *line = malloc(MAX_LINE_LENGTH * sizeof(char));

    if (!line)
        return NULL;

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL)
    {
        if (strstr(line, sym))
        {
            char *split_str = malloc(strlen(line));
            strncpy(split_str, line, strlen(line));

            rva_val_str = strtok(split_str, ":");
            rva_val_str = strtok(NULL, ", ");
            sscanf(rva_val_str, "[%*x:%x]", &rva_val);
            free(split_str);
            break;
        }
    }
    
    cache_entry->sym = sym;
    cache_entry->line = _strdup(line);

    add_to_sym_cache_file(line);

    free(line);
    fclose(fp);

    return get_rva_func(rva_val);
}

bool release_cvdump_exe(void)
{
    FILE* fp = fopen(CVDUMP_EXE_PATH, "wb");
    if (fp)
    {
        fwrite(CVDUMP_EXE_RES, sizeof(CVDUMP_EXE_RES), 1, fp);
        printf("Release resource cvdump.exe success.\n");
        fclose(fp);
        return true;
    }
    else
    {
        printf("Release resource cvdump.exe failed.\n");
        return false;
    }
}
