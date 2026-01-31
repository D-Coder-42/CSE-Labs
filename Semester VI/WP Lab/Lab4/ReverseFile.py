def reverseFile(inpf, outf):
    with open(inpf, 'r') as f:
        content = f.read()
    
    reversed = content[::-1]

    with open(outf, 'w') as f:
        f.write(reversed) 

inpfile = "files/source.txt"
outfile = "files/reversed.txt"

reverseFile(inpfile, outfile);
print(f"Reversed content from '{inpfile}' has been written to '{outfile}'")