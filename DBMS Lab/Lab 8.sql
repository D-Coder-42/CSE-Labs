create table SALARY_RAISE (
	INSTRUCTOR_ID	varchar(5),
	RAISE_DATE	date,
	RAISE_AMT	numeric(8,2)
);

-- 1.
declare
	cursor CUR (DNAME INSTRUCTOR.DEPT_NAME %type) is select * from INSTRUCTOR where DEPT_NAME = DNAME;
	DNAME INSTRUCTOR.DEPT_NAME %type;
	INST INSTRUCTOR %rowtype;
begin
	DNAME := '&Department';
	open CUR(DNAME);
	loop
		fetch CUR into INST;
		exit when CUR %NOTFOUND;
		update INSTRUCTOR set SALARY = SALARY * 1.05 where ID = INST.ID;
		insert into SALARY_RAISE values (INST.ID, SYSDATE, INST.SALARY*0.05);
	end loop;
	close CUR;
	dbms_output.put_line('Salary updated for all instructors in department ' || DNAME);
end;
/

/*OUTPUT: 
Enter value for department: kl
old   6:        DNAME := '&Department';
new   6:        DNAME := 'Finance';
Salary updated for all instructors in department Finance

INSTR RAISE_DAT  RAISE_AMT
----- --------- ----------
12121 20-MAR-25       4725
76543 20-MAR-25       4200
*/

-- 2.
declare
	cursor CUR is select * from STUDENT order by TOT_CRED;
	STD STUDENT %rowtype;
begin
	open CUR;
	loop
		fetch CUR into STD;
		dbms_output.put_line('ID: ' || STD.ID || ' | Name: ' || STD.NAME || ' | Department Name: ' || STD.DEPT_NAME || ' | Total Credits: ' || STD.TOT_CRED);
		if CUR %rowcount = 10 then exit;
		end if;
	end loop;
end;
/

/*OUTPUT:
ID: 70557 | Name: Snow | Department Name: Physics | Total Credits: 0
ID: 12345 | Name: Shankar | Department Name: Comp. Sci. | Total Credits: 32
ID: 55739 | Name: Sanchez | Department Name: Music | Total Credits: 38
ID: 45678 | Name: Levy | Department Name: Physics | Total Credits: 46
ID: 54321 | Name: Williams | Department Name: Comp. Sci. | Total Credits: 54
ID: 44553 | Name: Peltier | Department Name: Physics | Total Credits: 56
ID: 76543 | Name: Brown | Department Name: Comp. Sci. | Total Credits: 58
ID: 76653 | Name: Aoi | Department Name: Elec. Eng. | Total Credits: 60
ID: 19991 | Name: Brandt | Department Name: History | Total Credits: 80
ID: 98765 | Name: Bourikas | Department Name: Elec. Eng. | Total Credits: 98
*/

-- 3.
declare
	cursor C is select * from TEACHES;
	N number;
	INST_NAME INSTRUCTOR.NAME %type;
	COURSE_ROW COURSE %rowtype;
	SECT_ROW SECTION % rowtype;
begin
	for T in C loop
		begin
			select count(*) into N from TAKES group by COURSE_ID, SEC_ID, SEMESTER, YEAR having COURSE_ID = T.COURSE_ID and SEC_ID = T.SEC_ID and SEMESTER = T.SEMESTER and YEAR = T.YEAR;
		exception
			when NO_DATA_FOUND then N := 0;
		end;

		select NAME into INST_NAME from INSTRUCTOR where ID = T.ID;
		select * into SECT_ROW from SECTION where COURSE_ID = T.COURSE_ID and SEC_ID = T.SEC_ID and SEMESTER = T.SEMESTER and YEAR = T.YEAR;
		select * into COURSE_ROW from COURSE where COURSE_ID = T.COURSE_ID;

		dbms_output.put_line('Course ID: ' || T.COURSE_ID || ' | Title: ' || COURSE_ROW.TITLE || ' | Department Name: ' || COURSE_ROW.DEPT_NAME || ' | Credits: ' || COURSE_ROW.CREDITS || ' | Instructor Name: ' || INST_NAME);
		dbms_output.put_line('Building: ' || SECT_ROW.BUILDING || ' | Room Number: ' || SECT_ROW.ROOM_NUMBER || ' Time Slot ID: ' || SECT_ROW.TIME_SLOT_ID || ' Total Students Enrolled: ' || N);
		dbms_output.put_line('_________________________________________________________________');
	end loop;
end;
/

/*OUTPUT:
Course ID: CS-101 | Title: Intro. to Computer Science | Department Name: Comp.
Sci. | Credits: 4 | Instructor Name: Srinivasan
Building: Packard | Room Number: 101 Time Slot ID: H Total Students Enrolled: 5
_________________________________________________________________
Course ID: CS-315 | Title: Robotics | Department Name: Comp. Sci. | Credits: 3 |
Instructor Name: Srinivasan
Building: Watson | Room Number: 120 Time Slot ID: D Total Students Enrolled: 2
_________________________________________________________________
Course ID: CS-347 | Title: Database System Concepts | Department Name: Comp.
Sci. | Credits: 3 | Instructor Name: Srinivasan
Building: Taylor | Room Number: 3128 Time Slot ID: A Total Students Enrolled: 2
_________________________________________________________________
Course ID: FIN-201 | Title: Investment Banking | Department Name: Finance |
Credits: 3 | Instructor Name: Wu
Building: Packard | Room Number: 101 Time Slot ID: B Total Students Enrolled: 1
_________________________________________________________________
Course ID: MU-199 | Title: Music Video Production | Department Name: Music |
Credits: 3 | Instructor Name: Mozart
Building: Packard | Room Number: 101 Time Slot ID: D Total Students Enrolled: 1
_________________________________________________________________
Course ID: PHY-101 | Title: Physical Principles | Department Name: Physics |
Credits: 4 | Instructor Name: Einstein
Building: Watson | Room Number: 100 Time Slot ID: A Total Students Enrolled: 1
_________________________________________________________________
Course ID: HIS-351 | Title: World History | Department Name: History | Credits:
3 | Instructor Name: El Said
Building: Painter | Room Number: 514 Time Slot ID: C Total Students Enrolled: 1
_________________________________________________________________
Course ID: CS-101 | Title: Intro. to Computer Science | Department Name: Comp.
Sci. | Credits: 4 | Instructor Name: Katz
Building: Packard | Room Number: 101 Time Slot ID: F Total Students Enrolled: 1
_________________________________________________________________
Course ID: CS-319 | Title: Image Processing | Department Name: Comp. Sci. |
Credits: 3 | Instructor Name: Katz
Building: Watson | Room Number: 100 Time Slot ID: B Total Students Enrolled: 1
_________________________________________________________________
Course ID: BIO-101 | Title: Intro. to Biology | Department Name: Biology |
Credits: 4 | Instructor Name: Crick
Building: Painter | Room Number: 514 Time Slot ID: B Total Students Enrolled: 1
_________________________________________________________________
Course ID: BIO-301 | Title: Genetics | Department Name: Biology | Credits: 4 |
Instructor Name: Crick
Building: Painter | Room Number: 514 Time Slot ID: A Total Students Enrolled: 1
_________________________________________________________________
Course ID: CS-190 | Title: Game Design | Department Name: Comp. Sci. | Credits:
4 | Instructor Name: Brandt
Building: Taylor | Room Number: 3128 Time Slot ID: E Total Students Enrolled: 0
_________________________________________________________________
Course ID: CS-190 | Title: Game Design | Department Name: Comp. Sci. | Credits:
4 | Instructor Name: Brandt
Building: Taylor | Room Number: 3128 Time Slot ID: A Total Students Enrolled: 2
_________________________________________________________________
Course ID: CS-319 | Title: Image Processing | Department Name: Comp. Sci. |
Credits: 3 | Instructor Name: Brandt
Building: Taylor | Room Number: 3128 Time Slot ID: C Total Students Enrolled: 1
_________________________________________________________________
Course ID: EE-181 | Title: Intro. to Digital Systems | Department Name: Elec.
Eng. | Credits: 3 | Instructor Name: Kim
Building: Taylor | Room Number: 3128 Time Slot ID: C Total Students Enrolled: 1
_________________________________________________________________
*/
-- 4.
declare
	cursor C is select * from TAKES where COURSE_ID = 'CS-101';
	CREDS STUDENT.TOT_CRED %type;
BEGIN
	for T in C loop
		select TOT_CRED into CREDS from STUDENT where ID = T.ID;
		if CREDS < 40 then
			delete from TAKES where ID = T.ID and COURSE_ID = T.COURSE_ID;
		end if;
	end loop;
end;
/

-- 5.
update STUDENT_TABLE set LETTERGRADE = 'F';

declare
	cursor C is select * from STUDENT_TABLE for update;
	GPA STUDENT_TABLE.GPA %type;
begin
	for STD in C loop
		GPA := STD.GPA;
		if GPA <= 4 then
			update STUDENT_TABLE set LETTERGRADE = 'F' where current of C;	
		elsif GPA <= 5 then
			update STUDENT_TABLE set LETTERGRADE = 'E' where current of C;
		elsif GPA <= 6 then
			update STUDENT_TABLE set LETTERGRADE = 'D' where current of C;
		elsif GPA <= 7 then
			update STUDENT_TABLE set LETTERGRADE = 'C' where current of C;
		elsif GPA <= 8 then
			update STUDENT_TABLE set LETTERGRADE = 'B' where current of C;
		elsif GPA <= 9 then
			update STUDENT_TABLE set LETTERGRADE = 'A' where current of C;
		else
			update STUDENT_TABLE set LETTERGRADE = 'A+' where current of C;
		end if;
	end loop;
end;
/

-- 6.
declare
	cursor C (CID TEACHES.COURSE_ID %type) is select distinct ID from TEACHES where COURSE_ID = CID;
	INST_NAME INSTRUCTOR.NAME %type;
	CID TEACHES.COURSE_ID %type;
begin
	CID := '&Course_ID';
	for I in C(CID) loop
		select NAME into INST_NAME from INSTRUCTOR where ID = I.ID;
		dbms_output.put_line('ID: ' || I.ID || ' | Name: ' || INST_NAME);
	end loop;
end;
/

/*OUTPUT:
Enter value for course_id: CS-101
old   6:        CID := '&Course_ID';
new   6:        CID := 'CS-101';
ID: 10101 | Name: Srinivasan
ID: 45565 | Name: Katz
*/

-- 7.
DECLARE
    CURSOR C1 IS SELECT * FROM advisor;
    CURSOR C2(i takes.ID%TYPE) IS SELECT * FROM takes WHERE ID = i;
    CURSOR C3(i teaches.ID%TYPE) IS SELECT * FROM teaches WHERE ID = i;
    stdName student.name%TYPE;
    flag NUMERIC(1);
BEGIN
    FOR I IN C1
    LOOP
        flag := 0;
        FOR J IN C2(I.s_ID)
        LOOP
            FOR Z IN C3(I.i_ID)
            LOOP
                IF J.course_id = Z.course_id AND J.sec_id = Z.sec_id AND J.semester = Z.semester AND J.year = Z.year THEN
                SELECT name INTO stdName FROM student WHERE ID = J.ID;
                DBMS_OUTPUT.PUT_LINE('ID: ' || J.ID || 'Name: ' || stdName);
                flag := 1;
                END IF;
            END LOOP;
            IF flag = 1 THEN EXIT; END IF;
        END LOOP;
    END LOOP;
END;
/

/* OUTPUT:
ID: 12345Name: Shankar
ID: 44553Name: Peltier
ID: 76653Name: Aoi
ID: 98988Name: Tanaka
*/

-- 7. (GPT)
DECLARE
    -- A cursor to get students who are taking courses taught by their advisors
    CURSOR C1 IS
        SELECT DISTINCT s.ID AS student_id, s.NAME AS student_name, t.COURSE_ID, t.SEC_ID, t.SEMESTER, t.YEAR
        FROM student s
        JOIN advisor a ON s.ID = a.S_ID  -- Join students with their advisor
        JOIN takes t ON s.ID = t.ID     -- Join students with the courses they are taking
        JOIN teaches te ON t.COURSE_ID = te.COURSE_ID AND t.SEC_ID = te.SEC_ID
        WHERE a.I_ID = te.ID;  -- Ensure the course is taught by the advisor (match advisor's ID)

    stdName student.NAME%TYPE;  -- Variable to store the student's name
BEGIN
    -- Loop through each student and their course
    FOR student_record IN C1 LOOP
        -- Output the student's ID, name, and the course they are taking
        DBMS_OUTPUT.PUT_LINE('Student ID: ' || student_record.student_id || 
                             ' | Name: ' || student_record.student_name ||
                             ' | Course ID: ' || student_record.COURSE_ID ||
                             ' | Section ID: ' || student_record.SEC_ID ||
                             ' | Semester: ' || student_record.SEMESTER ||
                             ' | Year: ' || student_record.YEAR);
    END LOOP;
END;
/

/*OUTPUT:
Student ID: 12345 | Name: Shankar | Course ID: CS-315 | Section ID: 1 |
Semester: Spring | Year: 2010
Student ID: 76653 | Name: Aoi | Course ID: EE-181 | Section ID: 1 | Semester:
Spring | Year: 2009
Student ID: 00128 | Name: Zhang | Course ID: CS-101 | Section ID: 1 | Semester:
Fall | Year: 2009
Student ID: 76543 | Name: Brown | Course ID: CS-101 | Section ID: 1 | Semester:
Fall | Year: 2009
Student ID: 44553 | Name: Peltier | Course ID: PHY-101 | Section ID: 1 |
Semester: Fall | Year: 2009
Student ID: 98988 | Name: Tanaka | Course ID: BIO-101 | Section ID: 1 |
Semester: Summer | Year: 2009
Student ID: 12345 | Name: Shankar | Course ID: CS-347 | Section ID: 1 |
Semester: Fall | Year: 2009
Student ID: 98988 | Name: Tanaka | Course ID: BIO-301 | Section ID: 1 |
Semester: Summer | Year: 2010
*/
