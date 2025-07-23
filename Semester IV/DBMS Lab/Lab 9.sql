--1. Write a procedure to display a message “Good Day to You”.
create or replace procedure PRINT_MESSAGE is
begin
	dbms_output.put_line('Good Day to You');
end;
/

declare
begin
	PRINT_MESSAGE;
end;
/

/*
OUTPUT:
Good Day to You
*/

--2. Based on the University Database Schema in Lab 2, write a procedure which takes the dept_name as input parameter and lists all the instructors associated with the department as well as list all the courses offered by the department. Also, write an anonymous block with the procedure call. 
create or replace procedure LIST_INSTRUCTORS (DNAME INSTRUCTOR.DEPT_NAME %type) is
	cursor C(DEPT INSTRUCTOR.DEPT_NAME %type) is select * from INSTRUCTOR where DEPT_NAME = DEPT;
begin
	for INST in C(DNAME) loop
		dbms_output.put_line('ID: ' || INST.ID || ' | Name: ' || INST.NAME || ' | Department Name: ' || INST.DEPT_NAME || ' | Salary: ' || INST.SALARY);
	end loop;
end;
/

declare
	DNAME INSTRUCTOR.DEPT_NAME %type;
begin
	DNAME := '&Department';
	LIST_INSTRUCTORS(DNAME);
end;
/

/*
OUTPUT:
Enter value for department: Comp. Sci.
old   4:        DNAME := '&Department';
new   4:        DNAME := 'Comp. Sci.';
ID: 10101 | Name: Srinivasan | Department Name: Comp. Sci. | Salary: 68250
ID: 45565 | Name: Katz | Department Name: Comp. Sci. | Salary: 78750
ID: 83821 | Name: Brandt | Department Name: Comp. Sci. | Salary: 96600
*/

-- 3. Based on the University Database Schema in Lab 2, write a Pl/Sql block of code that lists the most popular course (highest number of students take it) for each of the departments. It should make use of a procedure course_popular which finds the most popular course in the given department.
create or replace procedure COURSE_POPULAR is
	cursor D is select distinct DEPT_NAME from COURSE;
	cursor C(DEPT COURSE.DEPT_NAME %type) is select * from COURSE where DEPT_NAME = DEPT;
	POPULAR COURSE %rowtype;
	MAXIMUM number := 0;
	N number;
begin
	for J in D loop
		open C(J.DEPT_NAME);
		fetch C into POPULAR;
		if C %notfound then
			dbms_output.put_line('No course offered by department');
			close C;
			return;
		end if;
		close C;

		for I in C(J.DEPT_NAME) loop
			N := 0;
			begin
				select count(*) into N from TAKES where COURSE_ID = I.COURSE_ID;
			exception
				when NO_DATA_FOUND then 
					N:=0;
			end;	
	
			if MAXIMUM < N then
				POPULAR := I;
				MAXIMUM := N;
			end if;
		end loop;

		dbms_output.put_line('Most popular course in ' || J.DEPT_NAME || ': ');
		dbms_output.put_line('Course ID: ' || POPULAR.COURSE_ID || ' | Course Title: ' || POPULAR.TITLE);
		dbms_output.put_line('-------------------------------------------------------');
	end loop;
end;
/

declare
begin
	COURSE_POPULAR;
end;
/
\

/*
OUTPUT:
Most popular course in Elec. Eng.:
Course ID: EE-181 | Course Title: Intro. to Digital Systems
-------------------------------------------------------
Most popular course in Physics:
Course ID: PHY-101 | Course Title: Physical Principles
-------------------------------------------------------
Most popular course in Comp. Sci.:
Course ID: CS-101 | Course Title: Intro. to Computer Science
-------------------------------------------------------
Most popular course in Finance:
Course ID: FIN-201 | Course Title: Investment Banking
-------------------------------------------------------
Most popular course in Biology:
Course ID: BIO-101 | Course Title: Intro. to Biology
-------------------------------------------------------
Most popular course in History:
Course ID: HIS-351 | Course Title: World History
-------------------------------------------------------
Most popular course in Music:
Course ID: MU-199 | Course Title: Music Video Production
-------------------------------------------------------
*/
-- 4. Based on the University Database Schema in Lab 2, write a procedure which takes the dept-name as input parameter and lists all the students associated with the department as well as list all the courses offered by the department. Also, write an anonymous block with the procedure call.
create or replace procedure STUDENT_COURSE_LIST (DNAME UNIDEPARTMENT.DEPT_NAME %type) is
	cursor S is select * from STUDENT where DEPT_NAME = DNAME;
	cursor C is select * from COURSE where DEPT_NAME = DNAME;
	COU COURSE %rowtype;
	STD STUDENT %rowtype;
begin
	open C;
	fetch C into COU;
	if C %notfound then
		dbms_output.put_line('No courses offered by department');
		return;
		close C;
	end if;
	close C;
	
	dbms_output.put_line('Courses offered by ' || DNAME || ': ');
	for I in C loop
		dbms_output.put_line(I.COURSE_ID || ' - ' || I.TITLE);
	end loop;

	dbms_output.put_line('-----------------------------------------');	

	open S;
	fetch S into STD;
	if S %notfound then
		dbms_output.put_line('No students enrolled under department');
		return;
		close S;
	end if;
	close S;
	
	dbms_output.put_line('Students enrolled under ' || DNAME || ': ');
	for I in S loop
		dbms_output.put_line(I.ID || ' - ' || I.NAME);
	end loop;
end;
/

declare
	DNAME INSTRUCTOR.DEPT_NAME %type;
begin
	DNAME := '&Department';
	STUDENT_COURSE_LIST(DNAME);
end;
/

/*
OUTPUT:
Enter value for department: Comp. Sci.
old   4:        DNAME := '&Department';
new   4:        DNAME := 'Comp. Sci.';
Courses offered by Comp. Sci.:
CS-101 - Intro. to Computer Science
CS-190 - Game Design
CS-315 - Robotics
CS-319 - Image Processing
CS-347 - Database System Concepts
-----------------------------------------
Students enrolled under Comp. Sci.:
00128 - Zhang
12345 - Shankar
54321 - Williams
76543 - Brown
*/

-- 5. Write a function to return the Square of a given number and call it from an anonymous block.
create or replace function SQUARE(N number)
	return number as
	SQ number;
begin
	SQ := N*N;
	return SQ;
end;
/

declare
	N number;
begin
	N := &Number;
	dbms_output.put_line('Square of ' || N || ' is: ' || SQUARE(N));
end;
/

/*
OUTPUT:
Enter value for number: 27
old   4:        N := &Number;
new   4:        N := 27;
Square of 27 is: 729
*/

-- 6. Based on the University Database Schema in Lab 2, write a Pl/Sql block of code that lists the highest paid Instructor in each of the Department. It should make use of a function department_highest which returns the highest paid Instructor for the given branch.
create or replace function DEPARTMENT_HIGHEST (DNAME UNIDEPARTMENT.DEPT_NAME %type)
	return INSTRUCTOR.NAME %type as
	HIGHEST INSTRUCTOR.NAME %type;
	MAXIMUM INSTRUCTOR.SALARY %type := 0;
	cursor C is select * from INSTRUCTOR where DEPT_NAME = DNAME;
begin
	for INST in C loop
		if INST.SALARY > MAXIMUM then
			HIGHEST := INST.NAME;
			MAXIMUM := INST.SALARY;
		end if;
	end loop;
	return HIGHEST;
end;
/

declare
	cursor C is select * from UNIDEPARTMENT;
	HIGHEST INSTRUCTOR.NAME %type;
begin
	for DEPT in C loop
		HIGHEST := DEPARTMENT_HIGHEST(DEPT.DEPT_NAME);
		dbms_output.put_line('Highest paid instructor of department ' || DEPT.DEPT_NAME || ': ' || HIGHEST);
	end loop;
end;
/

/*
OUTPUT:
Highest paid instructor of department Biology: Crick
Highest paid instructor of department Comp. Sci.: Brandt
Highest paid instructor of department Elec. Eng.: Kim
Highest paid instructor of department Finance: Wu
Highest paid instructor of department History: Califieri
Highest paid instructor of department Music: Mozart
Highest paid instructor of department Physics: Einstein
*/

-- 7. Based on the University Database Schema in Lab 2, create a package to include the following: 
-- a) A named procedure to list the instructor_names of given department 
-- b) A function which returns the max salary for the given department 
-- c) Write a PL/SQL block to demonstrate the usage of above package components

create or replace package DEPT_INFO as
	procedure LIST_INST(DEPT UNIDEPARTMENT.DEPT_NAME %type);
	function HIGH_SAL(DEPT UNIDEPARTMENT.DEPT_NAME %type) return number;
end DEPT_INFO;
/

create or replace package body DEPT_INFO as
procedure LIST_INST (DEPT UNIDEPARTMENT.DEPT_NAME %type) is
	cursor C is select * from INSTRUCTOR where DEPT_NAME = DEPT;
begin
	for INST in C loop
		dbms_output.put_line('ID: ' || INST.ID || ' | Name: ' || INST.NAME || ' | Department Name: ' || INST.DEPT_NAME || ' | Salary: ' || INST.SALARY);
	end loop;
end;
function HIGH_SAL (DEPT UNIDEPARTMENT.DEPT_NAME %type)
	return number as
	MAXIMUM number := 0;
	cursor C is select * from INSTRUCTOR where DEPT_NAME = DEPT;
begin
	for INST in C loop
		if INST.SALARY > MAXIMUM then
			MAXIMUM := INST.SALARY;
		end if;
	end loop;
	return MAXIMUM;
end;
end DEPT_INFO;
/
