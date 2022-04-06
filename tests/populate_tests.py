import sys
import string

PROJECT_NAME = sys.argv[1]
STDT_NUM = sys.argv[2]

temp_identifiers = {}
temp_identifiers["elf_binary_loc"] = f"../dist/default/debug/{PROJECT_NAME}.debug.elf"

ms500_passed = None
ms1000_passed = None

def populate_test(temp_file_str, temp_identifiers):
    with open(temp_file_str) as f:
        render = string.Template(f.read()).substitute(**temp_identifiers)
    with open(temp_file_str+".render", "w") as f:
        f.write(render)

with open(f"../the_1_{STDT_NUM}.s","r") as f:
    for lineno, line in enumerate(f.readlines()):
        if line.find("ms500_passed:") != -1:
            if ms500_passed:
                print("Multiple labels for ms500_passed, remove extra strings (in comments etc. containing 'ms500_passed:' (with double colon)")
                exit(1)
            else:
                ms500_passed = lineno+2
        elif line.find("ms1000_passed:") != -1:
            if ms1000_passed:
                print("Multiple labels for ms1000_passed, remove extra strings (in comments etc. containing 'ms1000_passed:' (with double colon)")
                exit(1)
            else:
                ms1000_passed = lineno+2



if not (ms500_passed and ms1000_passed):
    print("One of the debug labels are missing.")
    exit(1)


with open(f"../dist/default/debug/{PROJECT_NAME}.debug.cmf","r") as f:
    for line in f.readlines():
        if line.find(f">{ms500_passed}:") != -1:
            ms500_passed = line.strip().split()[0]
        elif line.find(f">{ms1000_passed}:") != -1:
            ms1000_passed = line.strip().split()[0]

temp_identifiers["msec500_passed"] = ms500_passed
temp_identifiers["msec1000_passed"] = ms1000_passed


populate_test("msec1000_passed_test.dbg", temp_identifiers)
populate_test("msec500_passed_test.dbg", temp_identifiers)
populate_test("msec500_passed_test_ra4_hold.dbg", temp_identifiers)
populate_test("msec500_passed_test_re4_hold.dbg", temp_identifiers)
            


    
