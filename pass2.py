import os

# Read input files
ic_file = open('inter_code.txt', 'r')
symtab_file = open('SymTab.txt', 'r')
lit_file = open('literals.txt', 'r')
output_file = open('pass2_output.txt', 'w')

# Create dictionaries for symbol table and literal table
symtab = {}
for line in symtab_file:
    if line.strip():
        symbol, address = line.strip().split()
        symtab[symbol] = address

littab = {}
lit_count = 1
for line in lit_file:
    if line.strip():
        parts = line.strip().split()
        if len(parts) >= 2 and '**' not in parts[1]:
            value = parts[0].replace("='", "").replace("'", "")
            littab[str(lit_count)] = (value, parts[1])
            lit_count += 1

# Process each line of intermediate code
for line in ic_file:
    if not line.strip():
        continue

    parts = line.strip().split()
    if len(parts) < 2:
        continue

    lc = parts[0] if parts[0].isdigit() else ""
    output_line = ""

    # Process different parts of the instruction
    if "(IS" in line:
        # Get opcode
        opcode = parts[1].split(",")[1].replace(")", "")
        op1 = ""
        op2 = ""

        # Process operands
        for part in parts[2:]:
            if "(RG," in part:
                reg_num = part.split(",")[1].replace(")", "")
                if not op1:
                    op1 = reg_num
                else:
                    op2 = reg_num
            elif "(S," in part:
                sym_idx = part.split(",")[1].replace(")", "")
                for symbol, addr in symtab.items():
                    if sym_idx == symbol:
                        op2 = addr
            elif "(L," in part:
                lit_idx = part.split(",")[1].replace(")", "")
                if lit_idx in littab:
                    op2 = littab[lit_idx][1]

        output_line = f"{lc}\t{opcode}\t{op1}\t{op2}"

    elif "(DL" in line:
        opcode = parts[1].split(",")[1].replace(")", "")
        if "(C," in parts[2]:
            value = parts[2].split(",")[1].replace(")", "")
            output_line = f"{lc}\t{opcode}\t{value}\t"

    # Write to output file if we have a valid instruction
    if output_line:
        output_file.write(output_line + "\n")

# Close all files
ic_file.close()
symtab_file.close()
lit_file.close()
output_file.close()