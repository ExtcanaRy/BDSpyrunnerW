import os
import re
import time
import difflib
import argparse
import multiprocessing as mp

try:
    import pefile
    import chardet
    import requests
    import colorama

    from fastread import Fastread
    from tqdm import tqdm
    from colorama import Fore
except ModuleNotFoundError:
    if input("Install requirements with: pip install pefile chardet requests colorama fastread tqdm ? [y/n]") == "y":
        os.system("pip install pefile chardet requests colorama fastread tqdm")
    os._exit(1)

def is_executable(filename) -> bool:
    try:
        pe = pefile.PE(filename)

        # Check for executable code segments
        if not any(s.SizeOfRawData > 0 for s in pe.sections):
            return False

        # Check if there is an import table
        if not hasattr(pe, "DIRECTORY_ENTRY_IMPORT"):
            return False

        # Check if there is an export table
        # if not hasattr(pe, "DIRECTORY_ENTRY_EXPORT"):
        #    return False

        # Check if there is an res table
        if not hasattr(pe, "DIRECTORY_ENTRY_RESOURCE"):
            return False

        # The file is complete
        return True
    except pefile.PEFormatError:
        # The file is incomplete
        return False
    except OSError:
        # File not found
        return False


colorama.init()  # init colorama


def printc(text, color=""):
    """print colorful text"""
    print(color + text + Fore.RESET)


def downloadCVDUMP():
    url = "https://raw.fgit.ml/microsoft/microsoft-pdb/master/cvdump/cvdump.exe"
    if not is_executable("cvdump.exe"):
        for retries in range(3):
            print()
            printc("Downloading cvdump.exe", Fore.CYAN)
            response = requests.get(url)
            if response.status_code == 200:
                printc("Connected!", Fore.GREEN)
                # write to file
                with open("cvdump.exe", "wb") as file:
                    file.write(response.content)
            if is_executable("cvdump.exe"):
                printc("Downloaded cvdump.exe", Fore.GREEN)
                print()
                break
            else:
                printc(
                    f"Failed to download cvdump.exe, retrying...({retries+1}/3)", Fore.RED)
        if not is_executable("cvdump.exe"):
            printc("Failed to download cvdump.exe, please try again or download it manually and put it in the same folder as this script", Fore.RED)
            printc("Url: " + url, Fore.CYAN)
            os._exit(1)


def read_pdb_symbols(file_path) -> list:
    start_time = time.time()

    pdb_symbols = []
    lines = Fastread(file_path).lines()
    if linux:
        for index, line in enumerate(lines):
            if "_Z" in line:
                pdb_symbols.append(line[line.find("_Z"):-1])
    else:
        for index, line in enumerate(lines):
            if "?" in line and "Z" in line:
                pdb_symbols.append(line[43:-1])
            elif "?" in line and "B" in line:
                pdb_symbols.append(line[43:-1])
    printc(
        f"Loaded {len(pdb_symbols)} symbols from pdb file ({str(time.time()-start_time)[:5]})s!", Fore.GREEN)
    return pdb_symbols


def read_plugin_symbols(source_code_path) -> list:
    start_time = time.time()
    file_list = get_file_list(source_code_path)
    plugin_symbols = {}
    str_begin = None
    str_end = None
    for file_path in file_list:
        with open(file_path, "r", encoding=get_file_encoding(file_path)) as source_file:
            lines = (line for line in source_file)
            for index, line in enumerate(lines):
                if "//" in line:
                    continue
                if r'"?' in line and r'Z"' in line:
                    str_begin = line.find(r'"?') + 1
                    str_end = line.find(r'Z"') + 1
                    # print(line[str_begin:str_end])
                elif r'"?' in line and r'B"' in line:
                    str_begin = line.find(r'"?') + 1
                    str_end = line.find(r'B"') + 1
                else:
                    continue
                if not re.search(r'[^a-zA-Z0-9@?_$]', line[str_begin:str_end]):
                    plugin_symbols[line[str_begin:str_end]
                                    ] = f"{file_path}:{index+1}"
                    
        if verbose_mode:
            print(f"Processed file: {file_path}")
    printc(
        f"Loaded {len(plugin_symbols)} symbols from source code files {timer(start_time, time.time())}!", Fore.GREEN)
    return plugin_symbols


def timer(start_time: float, end_time: float) -> str:
    return f"({str(time.time()-start_time)[:5]}s)"


def get_file_list(source_code_path) -> list:
    file_list = []
    for root, dirs, files in os.walk(source_code_path):
        for file in files:
            if ".h" in file or ".cpp" in file:
                file_list.append(os.path.join(root, file))
    return file_list


def search_str_in_file(file_path, str) -> int:
    with open(file_path, "r", encoding=get_file_encoding(file_path)) as source_file:
        lines = (line for line in source_file)
        for index, line in enumerate(lines):
            if "//" in line:  # Does not match the symbol in the comment
                # pass
                continue
            if str in line:
                return index + 1
    return 0


def get_file_encoding(filePath) -> str:
    with open(filePath, "rb") as f:
        data = f.read()
        result = chardet.detect(data)
        return result["encoding"]


def dict_split(d, chunk_size):
    keys = list(d.keys())
    chunks = [dict(zip(keys[i::chunk_size], (d[k]
                   for k in keys[i::chunk_size]))) for i in range(chunk_size)]
    return chunks


def compare():
    pdb_symbols = read_pdb_symbols("symbols.txt")
    # exit()
    plugin_symbols = read_plugin_symbols(source_code_path)
    print()
    pbar = tqdm(total=len(plugin_symbols), desc="Progress",
                leave=True, unit="sym", unit_scale=False)
    plugin_symbols_tasks = dict_split(plugin_symbols, mp.cpu_count())
    parent_conn, child_conn = mp.Pipe()
    for plugin_symbols_task in plugin_symbols_tasks:
        mp.Process(target=compare_tsk, args=(
            pdb_symbols, plugin_symbols_task, verbose_mode, similar_ratio, child_conn), daemon=True).start()

    missing_count = 0
    for index in range(len(plugin_symbols)):
        result = parent_conn.recv()
        if len(result) == 4:
            pbar.write(
                f"{Fore.GREEN}Found symbol at {result['pos']} {result['time']}:")
            pbar.write(f"{result['sym']}{Fore.RESET}")
            pbar.write(f"{Fore.MAGENTA}Key words: {', '.join(result['keywords'])}{Fore.RESET}")
        elif len(result) == 5:
            similar_syms = '\n'.join(result['similar_syms'])
            pbar.write(
                f"{Fore.RED}Missing symbol at file {result['pos']} {result['time']}{Fore.RESET}")
            pbar.write(f"{Fore.YELLOW}{result['sym']}{Fore.RESET}")
            pbar.write(f"{Fore.MAGENTA}Key words: {', '.join(result['keywords'])}{Fore.RESET}")
            pbar.write(
                f"{Fore.GREEN}Similar symbols in symbol file:{Fore.RESET}")
            pbar.write(f"{Fore.LIGHTBLUE_EX}{similar_syms}{Fore.RESET}\n")
            missing_count += 1
        if index == len(plugin_symbols) - 1:
            color = Fore.LIGHTRED_EX
            if missing_count == 0:
                color = Fore.LIGHTGREEN_EX
            pbar.write(f"{color}Missing {missing_count} symbols{Fore.RESET}")
        pbar.update(1)


def compare_tsk(pdb_symbols, plugin_symbols, verbose_mode, similar_ratio, conn):
    time.sleep(1)
    for symbol in plugin_symbols:
        start_time = time.time()
        if symbol not in pdb_symbols:
            similar_symbols = {}
            keywords = []
            first_keyword = re.search(r'[a-z]+[^@]*', symbol).group()
            keywords = re.findall(r'[A-Z][a-z]+(?:[A-Z][a-z]+)*', symbol)[:5]
            if keywords[0] in first_keyword:
                keywords[0] = first_keyword
            keywords = list(set([keyword for keyword in keywords if len(keyword) >= 5]))

            for pdb_symbol in pdb_symbols:
                if first_keyword in pdb_symbol:
                    similarity = difflib.SequenceMatcher(
                        None, symbol, pdb_symbol).quick_ratio()
                    similar_symbols[pdb_symbol] = similarity
            if len(similar_symbols) > 5 or len(similar_symbols) == 0:
                similar_symbols = {}
                for pdb_symbol in pdb_symbols:
                    if not all(keyword in pdb_symbol for keyword in keywords):
                        continue
                    similarity = difflib.SequenceMatcher(
                        None, symbol, pdb_symbol).quick_ratio()
                    if similarity >= similar_ratio:
                        similar_symbols[pdb_symbol] = similarity

            similar_symbols = sorted(similar_symbols.items(), key=lambda x: x[1], reverse=True)
            similar_symbols = [f"[{str(value*100)[:5]}%] {key}" for key, value in similar_symbols]
            conn.send({'pos': plugin_symbols[symbol], 'sym': symbol, 'keywords': keywords, 
                 'similar_syms': similar_symbols[:10], 'time': f"{timer(start_time, time.time())}"})
        elif verbose_mode:
            conn.send({'pos': plugin_symbols[symbol], 'sym': symbol, 'keywords': keywords,
                      'time': f"{timer(start_time, time.time())}"})
        else:
            conn.send({'time': f"{timer(start_time, time.time())}"})


def compare_tsk_linux():
    pass


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("pdb_file_path", help="the path to the pdb file")
    parser.add_argument("source_code_path", help="the path to the source code")
    parser.add_argument("-s", "--similar-ratio", type=float, default=0.8,
                        help="After the symbol is not found," +
                        " a lookup is attempted using the string similarity algorithm," +
                        " where a smaller value may provide more results, possible values: 0 to 1")
    parser.add_argument("-v", "--verbose", action="store_true",
                        help="enable verbose mode")
    parser.add_argument("-o", "--overwrite", action="store_true",
                        help="overwrite the cache")
    parser.add_argument("-l", "--linux", action="store_true",
                        help="using linux edition")

    args = parser.parse_args()

    pdb_file_path = args.pdb_file_path
    source_code_path = args.source_code_path
    verbose_mode = args.verbose
    overwrite = args.overwrite
    similar_ratio = args.similar_ratio
    linux = args.linux

    #os.system("del /f /q symbols.txt")
    printc("\nSymbol Checker For BDS Projects\n", Fore.LIGHTCYAN_EX)
    downloadCVDUMP()
    if overwrite or not os.path.exists("symbols.txt"):
        printc("Generating symbols.txt...", Fore.CYAN)
        start_time = time.time()
        os.system(f".\cvdump.exe -headers -p {pdb_file_path} > symbols.txt")
        printc(f"Generated{timer(start_time, time.time())}!\n", Fore.GREEN)
    compare()
