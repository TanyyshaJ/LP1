import re as regex
import itertools
import json
import os

# Clear the terminal before starting the program
os.system('cls' if os.name == 'nt' else 'clear')

# Clear the Expanded code file before running
with open("output/expcode.asm",'w') as file:
    pass
file.close()

def get_key(val: str, dict: dict) -> any:
    """
    Function to get key from a dictionary based on value
    """
    for key, value in dict.items():
        if val == value:
            return key
    return None

def process_params(param: str):
    """
    Function to process strings of the type "(P,1)", "(C,11)", etc.
    Returns a tuple of the character corresponding to the table
    and the position of the parameter
    """
    pattern = r'\(([^,]+),(\d+)\)'
    match = regex.match(pattern, param)
    if match:
        component1 = match.group(1)
        component2 = int(match.group(2))
        return component1, component2
    else:
        return None
    
def convert(lst):
    """
    Function to convert list of items into space separated values
    """
    return ' '.join(lst)

# Input File(s)
kpdtabFile = open('input/kpdtab.json', 'r')
mdtFile = open('input/mdt.asm', 'r')
mntFile = open('input/mnt.json', 'r')
pntabFile = open('input/pnt.json', 'r')
callsFile = open('input/calls.asm', 'r')

# Expanded Code (Output) File
expcodeFile = open('output/expcode.asm', 'a')

# Tables
kpdtab = json.load(kpdtabFile)
mnt = json.load(mntFile)
pntab = json.load(pntabFile)

# Regex pattern to split on occurrence of one or more spaces
spacePattern = r'\s+'

for line in callsFile:
    # Skip blank lines and remove beginning and trailing whitespace(s)
    if line == '\n': continue
    line = line.strip()

    # Split the line into words
    cmd = regex.split(spacePattern, line.rstrip())

    macroName = cmd[0]      # 0th item of call statement is macro name
    MPList = cmd[1::]       # Remaining items are parameters of macro

    # npp: number of positional parameters
    # nkp: number of keyword parameters
    mdtPointer, kpdtPointer, npp, nkp = "","","",""

    for key,value in mnt.items():
        if value['name'] == macroName:
            mdtPointer = int(value['mdtp'])  # Convert to integer
            kpdtPointer = value['kpdtp']
            npp = int(value['pp'])           # Convert to integer
            nkp = int(value['kp'])           # Convert to integer
            break
    
    # total number of parameters in the macro
    tot = npp + nkp

    # Create the skeleton of the Actual Parameter table based on the
    # Parameter name table of the corresponding macro
    APTAB = {}
    # Initialize APTAB with parameter positions
    for pos, param_name in pntab[macroName].items():
        APTAB[pos] = ""
    aptPointer = 1

    # Check to see if macro call corresponds to a valid macro definition
    if mdtPointer == "":
        print("No such macro exists")
        continue

    # Processing parameters, building APTAB
    for parameter in MPList:
        # Remove trailing commas
        if parameter[-1] == ',':
            parameter = parameter.replace(',','',1)

        # Keyword Parameter
        if '=' in parameter:
            eqIndex = parameter.index('=')
            paramName = parameter[0:eqIndex:]
            paramValue = parameter[eqIndex + 1::]
            for pos, name in pntab[macroName].items():
                if name == paramName:
                    APTAB[pos] = paramValue
                    break

        # Positional parameter
        else:
            if str(aptPointer) in APTAB:
                APTAB[str(aptPointer)] = parameter
                aptPointer += 1

    # Assign default values to parameters that haven't been
    # assigned a value in the call statement
    if nkp > 0:
        for entry in kpdtab.values():
            if entry["macro"] == macroName and entry["value"] != "---":
                for pos, name in pntab[macroName].items():
                    if name == entry["name"] and (pos not in APTAB or not APTAB[pos]):
                        APTAB[pos] = entry["value"]

    # List to keep track of expanded statements of corresponding macro call
    macroStmts = []

    # Read MDT and store the macro definition
    mdtFile.seek(0)
    lines = mdtFile.readlines()
    start_line = mdtPointer - 1  # Adjust for 0-based indexing
    for i in range(start_line, len(lines)):
        if 'MEND' in lines[i]:
            break
        if i != start_line:  # Skip the macro definition line
            macroStmts.append(lines[i].strip())

    # Replace parameter references with parameter values from APTAB
    for i in range(len(macroStmts)):
        line = macroStmts[i]
        words = line.split()
        for j in range(len(words)):
            if '(' in words[j]:
                tab, pos = process_params(words[j])
                if tab and str(pos) in APTAB:
                    words[j] = APTAB[str(pos)]
        macroStmts[i] = "+" + " ".join(words)
        expcodeFile.write(macroStmts[i] + '\n')

    expcodeFile.write('\n')

# Close all files
kpdtabFile.close()
mdtFile.close()
mntFile.close()
pntabFile.close()
callsFile.close()
expcodeFile.close()