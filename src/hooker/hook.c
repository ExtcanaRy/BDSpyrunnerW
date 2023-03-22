#include <hooker/hook.h>

enum section_type
{
    SECTION_TEXT,
    SECTION_RDATA,
    SECTION_DATA,
    SECTION_PDATA,
    SECTION__RDATA,
    SECTION_RSRC,
    SECTION_RELOC,
};

typedef enum {
    VIRTUAL_ADDRESS,
    MISC_VIRTUAL_SIZE,
    SIZE_OF_RAW_DATA,
    POINTER_TO_RAW_DATA,
    CHARACTERISTICS,
} section_property;

struct section_info
{
    DWORD virtual_address;
    DWORD misc_virtual_size;
    DWORD size_of_raw_data;
    DWORD pointer_to_raw_data;
    DWORD characteristics;
} section_infos[7];

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
inline void *rva2va(unsigned int rva)
{
    uintptr_t base_addr = (uintptr_t)GetModuleHandle(NULL);
    return (void *)(base_addr + rva);
}

void split_sym_line(const char *line, unsigned short *type, unsigned int *rva_val)
{
    *type = (unsigned short)strtol(line + 10, NULL, 16);
    *rva_val = (unsigned int)strtol(line + 15, NULL, 16);
}

void read_static_data(long offset, void *data, size_t size) {
    FILE *fp = fopen(BDS_EXE_PATH, "rb");
    if (!fp)
        return;
    fseek(fp, offset, SEEK_SET);
    fread(data, size, 1, fp);
    fclose(fp);
}

void *dlsym(const char *sym)
{
    static bool is_sym_file_generated = false;
    unsigned short rva_type = 0;
    static long rva_val = 0;


    rva_val = get_rva_from_hashmap(sym);
    if (rva_val == -1)
    {
        load_hashmap_from_file(SYM_CACHE_FILE);
        rva_val = get_rva_from_hashmap(sym);
        if (rva_val != -1)
            return rva2va(rva_val);
    }
    else
    {
        return rva2va(rva_val);
    }
    FILE *fp = fopen(SYM_FILE, "r");
    if (!fp)
    {
        if (!release_cvdump_exe())
            return NULL;

        printf("Symbol file " SYM_FILE " not found, trying to generate.\n");

        gen_sym_file();

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

    const size_t MAX_LINE_LENGTH = 8192;
    char *line = (char*)malloc(MAX_LINE_LENGTH);

    if (!line)
        return NULL;

    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL)
    {
        if (strstr(line, sym))
        {
            split_sym_line(line, &rva_type, &rva_val);
            switch (rva_type - 1)
            {
            case SECTION_TEXT:
                rva_val += section_infos[SECTION_TEXT].virtual_address;
                break;
            case SECTION_RDATA:
                free(line);
                fclose(fp);
                rva_val += section_infos[SECTION_RDATA].pointer_to_raw_data;
                return (void *)&rva_val;
                break;
            case SECTION_DATA:
                rva_val += section_infos[SECTION_DATA].virtual_address;
                break;
            }
            break;
        }
    }
    
    free(line);
    fclose(fp);

    add_entry(sym, rva_val);
    save_hashmap_to_file(SYM_CACHE_FILE);

    return rva2va(rva_val);
}

bool release_cvdump_exe(void)
{
    FILE *fp = fopen(CVDUMP_EXE_PATH, "wb");
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

inline int gen_sym_file(void)
{
    return system(CVDUMP_EXE_PATH CVDUMP_EXEC_ARGS BDS_PDB_PATH " > " SYM_FILE );
}

//////////////////// SECTION ////////////////////
bool init_section_infos(void)
{
    IMAGE_DOS_HEADER dos_header;
    IMAGE_NT_HEADERS64 nt_header;
    IMAGE_SECTION_HEADER section_header;

    HANDLE h_file = CreateFileA(BDS_EXE_PATH, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h_file == INVALID_HANDLE_VALUE)
    {
        printf("Error: cannot open file %s.\n", BDS_EXE_PATH);
        return false;
    }

    DWORD bytes_read = 0;
    if (!ReadFile(h_file, &dos_header, sizeof(dos_header), &bytes_read, NULL))
    {
        printf("Error: cannot read DOS header.\n");
        CloseHandle(h_file);
        return false;
    }

    if (SetFilePointer(h_file, dos_header.e_lfanew, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        printf("Error: cannot locate NT header.\n");
        CloseHandle(h_file);
        return false;
    }
    if (!ReadFile(h_file, &nt_header, sizeof(nt_header), &bytes_read, NULL))
    {
        printf("Error: cannot read NT header.\n");
        CloseHandle(h_file);
        return false;
    }

    DWORD sectionHeaderOffset = dos_header.e_lfanew + sizeof(nt_header.Signature) + sizeof(nt_header.FileHeader) + nt_header.FileHeader.SizeOfOptionalHeader;
    if (SetFilePointer(h_file, sectionHeaderOffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {
        printf("Error: cannot locate section header.\n");
        CloseHandle(h_file);
        return false;
    }

    for (int i = 0; i < nt_header.FileHeader.NumberOfSections; i++)
    {
        if (!ReadFile(h_file, &section_header, sizeof(section_header), &bytes_read, NULL))
        {
            printf("Error: cannot read section header.\n");
            CloseHandle(h_file);
            return false;
        }

        section_infos[i].virtual_address     = section_header.VirtualAddress;
        section_infos[i].misc_virtual_size   = section_header.Misc.VirtualSize;
        section_infos[i].size_of_raw_data    = section_header.SizeOfRawData;
        section_infos[i].pointer_to_raw_data = section_header.PointerToRawData;
        section_infos[i].characteristics     = section_header.Characteristics;
    }

    CloseHandle(h_file);
    return true;
}

//////////////////// MinHook ////////////////////

bool hooker_init(void)
{
    load_hashmap_from_file(SYM_CACHE_FILE);
    if (!init_section_infos())
        return false;
    if (MH_Initialize() != MH_OK)
        return false;
        
    return true;
}

bool hooker_uninit(void)
{
    save_hashmap_to_file(SYM_CACHE_FILE);
    free_hashmap();
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
