## 1. Write shell commands for the following.
# i) To create a directory in your home directory having 2 subdirectories.
mkdir lab1 ; cd lab1; mkdir sub1 sub2
# ii) In the first subdirectory, create 3 different files with different content in each of them.
cd sub1
touch file1 file2 file3
# iii) Copy the first file from the first subdirectory to the second subdirectory.
cp file1 ../sub2
# iv) Create one more file in the second subdirectory, which has the output of the number of users and number of files.
cd ../sub2
(awk -F: '$3 >= 1000 && $3 <= 60000' /etc/passwd | wc -l && ls | wc -l) > acc
# v) To list all the files which starts with either a or A.
ls [aA]*
# vi)To count the number of files in the current directory
ls | wc -l
# vii) Display the output if the compilation of a program succeeds.
echo "#include <stdio.h>
int main() {
	printf(\"Hello, World\n\");
	return 0;
}" > hello.c
cc -o hello hello.c && ./hello
## 2. Execute the following commands in sequence: i) date ii) ls iii) pwd
date; ls; pwd

### OUTPUT

# acc
# 2
# Hello, World
# Wednesday 23 July 2025 03:44:21 PM IST
# acc  file1  hello  hello.c
# /home/CSE_5C2/Documents/230905252/lab1/sub2