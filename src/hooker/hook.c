#include <hooker/hook.h>

/////////////////////////// CACHE API ////////////////////////
typedef struct {
    unsigned int rva;
} symbol_cache_entry_t;

static symbol_cache_entry_t symbol_cache[SYM_CACHE_SIZE] = {0};

void save_sym_cache()
{
    FILE *fp = fopen(SYM_CACHE_FILE, "wb");
    if (!fp)
    {
        perror("Unable to open symbol cache file for writing");
        return;
    }
    fwrite(symbol_cache, sizeof(symbol_cache_entry_t), SYM_CACHE_SIZE, fp);
    fclose(fp);
}

void load_sym_cache()
{
    FILE *fp = fopen(SYM_CACHE_FILE, "rb");
    if (!fp)
        return;

    fread(symbol_cache, sizeof(symbol_cache_entry_t), SYM_CACHE_SIZE, fp);
    fclose(fp);
}

//////////////////////// HASH SYM /////////////////////
bool is_prime(int n)
{
    if (n <= 1)
        return false;

    for (int i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}

int find_closest_prime(int n)
{
    if (n <= 2)
        return 2;

    if (is_prime(n))
        return n;

    int lower = n - 1;
    while (true)
    {
        if (is_prime(lower))
            return lower;

        lower--;
    }
}

static unsigned short hash_str(const char* str)
{
    unsigned int hash = 0;
    int c;
    static unsigned int prime = 0;
    
    if (!prime)
        prime = find_closest_prime(SYM_CACHE_SIZE);

    while ((c = *str++))
        hash = ((hash << 5) + hash) ^ c;

    return hash % prime;
}

//////////////////////////// HOOK API /////////////////////////
bool hook_func(void *hook_func, void *detour_func, void *original_func)
{
    if (MH_CreateHook(hook_func, detour_func, (LPVOID *)original_func) != MH_OK)
    {
        printf("Failed to create func hook, RVA: %llu \n", (uintptr_t)hook_func);
        return false;
    }
    return true;
}

//////////////////////////// SYM API /////////////////////////
void *get_rva_func(unsigned int rva)
{
    uintptr_t base_addr = (uintptr_t)GetModuleHandle(NULL);
    return (void *)(base_addr + rva + 4096);
}

void split_rva_val_line(const char *line, const char *sym, unsigned int *rva_val)
{
    char *split_str = malloc(strlen(line));
    strncpy(split_str, line, strlen(line));

    char *rva_val_str = strtok(split_str, ":");
    rva_val_str = strtok(NULL, ", ");
    sscanf(rva_val_str, "[%*x:%x]", rva_val);
    free(split_str);
}

void *dlsym(const char *sym)
{
    static bool is_sym_file_generated = false;
    unsigned int rva_val = 0;

    unsigned int sym_hash = hash_str(sym);
    symbol_cache_entry_t *cache_entry = &symbol_cache[sym_hash];
    if (cache_entry->rva != 0)
        return get_rva_func(cache_entry->rva);

    load_sym_cache();

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
            split_rva_val_line(line, sym, &rva_val);
            break;
        }
    }
    
    cache_entry->rva = rva_val;

    save_sym_cache();

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

//////////////////// MinHook ////////////////////

bool hooker_init(void)
{
    if (MH_Initialize() != MH_OK)
        return false;
        
    return true;
}

bool hooker_uninit(void)
{
    if (MH_Uninitialize() != MH_OK)
        return false;
        
    return true;
}

bool hooker_enable_all_hook(void)
{
    if (MH_QueueEnableHook(MH_ALL_HOOKS) != MH_OK)
        return false;
    if (MH_ApplyQueued() != MH_OK)
        return false;

    return true;
}

bool hooker_disable_all_hook(void)
{
    if (MH_QueueDisableHook(MH_ALL_HOOKS)!= MH_OK)
        return false;
    if (MH_ApplyQueued()!= MH_OK)
        return false;

    return true;
}
