SET SERVEROUTPUT ON

create table STUDENT (
	ROLLNO	number	primary key,
	GPA	numeric(4,2)
);

insert into STUDENT values (1, 5.8);
insert into STUDENT values (2, 6.5);
insert into STUDENT values (3, 3.4);
insert into STUDENT values (4, 7.8);
insert into STUDENT values (5, 9.5);

-- 1.
declare
	ROLL	STUDENT.ROLLNO %type;
	GRADE	float;
begin
	ROLL := &RollNo;
	select GPA into GRADE from STUDENT where ROLLNO = ROLL;
	dbms_output.put_line('GPA of student is: ' || GRADE);
exception
	when NO_DATA_FOUND then
	dbms_output.put_line('No student found');
end;
/

-- 2.
declare
	ROLL	STUDENT.ROLLNO %type;
	GRADE	float;
begin
	ROLL := &RollNo;
	select GPA into GRADE from STUDENT where ROLLNO = ROLL;
	dbms_output.put_line('GPA of student is: ' || GRADE);
	if GRADE<=4 then
		dbms_output.put_line('Grade of student is: F');
	elsif GRADE<=5 then
		dbms_output.put_line('Grade of student is: E');
	elsif GRADE<=6 then
		dbms_output.put_line('Grade of student is: D');
	elsif GRADE<=7 then
		dbms_output.put_line('Grade of student is: C');
	elsif GRADE<=8 then
		dbms_output.put_line('Grade of student is: B');
	elsif GRADE<=9 then
		dbms_output.put_line('Grade of student is: A');
	else
		dbms_output.put_line('Grade of student is: A+');
	end if;
exception
	when NO_DATA_FOUND then
	dbms_output.put_line('No student found');
end;
/

-- 3.
declare
	DAYS	number;
	FINE	number;
begin
	DAYS := &days;
	if DAYS <= 7 then
		FINE := 0;
	elsif DAYS <=15 then
		FINE := (DAYS-7)*1;
	elsif DAYS <=30 then
		FINE :=  8*1 + (DAYS-15)*2;
	else
		FINE := 8*1 + 15*2 + 5;
	end if;

	dbms_output.put_line('Fine for late return: Rs. ' || FINE);
end;
/

--4.
declare
	ROLL	STUDENT.ROLLNO %type;
	GRADE	STUDENT.GPA %type;
	i	number := 1;
begin
	loop
		select ROLLNO,GPA into ROLL,GRADE from STUDENT where ROLLNO = i;
		if GRADE<=4 then
			dbms_output.put_line('Grade of student ' || ROLL || ' is: F');
		elsif GRADE<=5 then
			dbms_output.put_line('Grade of student ' || ROLL || ' is: E');
		elsif GRADE<=6 then
			dbms_output.put_line('Grade of student ' || ROLL || ' is: D');
		elsif GRADE<=7 then
			dbms_output.put_line('Grade of student ' || ROLL || ' is: C');
		elsif GRADE<=8 then
			dbms_output.put_line('Grade of student ' || ROLL || ' is: B');
		elsif GRADE<=9 then
			dbms_output.put_line('Grade of student ' || ROLL || ' is: A');
		else
			dbms_output.put_line('Grade of student ' || ROLL || ' is: A+');
		end if;
		i := i + 1;
		if i > 5 then
			exit;
		end if;
	end loop;
end;
/

-- 5.
alter table STUDENT add LETTERGRADE varchar2(2);

declare
	GRADE 	STUDENT.GPA %type;
	i	number := 0;
begin
	while i <= 5
	loop
		select GPA into GRADE from STUDENT where ROLLNO = i;
		if GRADE <= 4 then
			update STUDENT set LETTERGRADE = 'F' where ROLLNO = i;
		elsif GRADE <= 5 then
			update STUDENT set LETTERGRADE = 'E' where ROLLNO = i;
		elsif GRADE <= 6 then
			update STUDENT set LETTERGRADE = 'D' where ROLLNO = i;
		elsif GRADE <= 7 then
			update STUDENT set LETTERGRADE = 'C' where ROLLNO = i;
		elsif GRADE <= 8 then
			update STUDENT set LETTERGRADE = 'B' where ROLLNO = i;
		elsif GRADE <= 9 then
			update STUDENT set LETTERGRADE = 'A' where ROLLNO = i;
		else
			update STUDENT set LETTERGRADE = 'A+' where ROLLNO = i;
		end if;
		i := i + 1;
	end loop;
end;
/

-- 6.
declare
	GRADE	STUDENT.GPA %type;
	MAXIMUM	STUDENT.GPA %type := 0;
begin
	for i in 1..5 loop
		select GPA into GRADE from STUDENT where ROLLNO = i;
		if GRADE > MAXIMUM then
			MAXIMUM := GRADE;
		end if;
	end loop;
	dbms_output.put_line('Maximum GPA is: ' || MAXIMUM);
end;
/

-- 7.
declare
	ROLL	STUDENT.ROLLNO %type;
	GRADE	STUDENT.GPA %type;
	i	number := 1;
begin
<<LOOP_BACK>>
	select ROLLNO,GPA into ROLL,GRADE from STUDENT where ROLLNO = i;
	if GRADE<=4 then
		dbms_output.put_line('Grade of student ' || ROLL || ' is: F');
	elsif GRADE<=5 then
		dbms_output.put_line('Grade of student ' || ROLL || ' is: E');
	elsif GRADE<=6 then
		dbms_output.put_line('Grade of student ' || ROLL || ' is: D');
	elsif GRADE<=7 then
		dbms_output.put_line('Grade of student ' || ROLL || ' is: C');
	elsif GRADE<=8 then
		dbms_output.put_line('Grade of student ' || ROLL || ' is: B');
	elsif GRADE<=9 then
		dbms_output.put_line('Grade of student ' || ROLL || ' is: A');
	else
		dbms_output.put_line('Grade of student ' || ROLL || ' is: A+');
	end if;
	i := i + 1;
	if i <= 5 then
		goto	LOOP_BACK;
	end if;
end;
/

-- 8.
declare
	INAME	INSTRUCTOR.NAME %type;
	RECORD	INSTRUCTOR %rowtype;
begin
	INAME := '&Name';
	select * into RECORD from INSTRUCTOR where NAME = INAME;
	dbms_output.put_line('ID: ' || RECORD.ID);
	dbms_output.put_line('Name: ' || RECORD.NAME);
	dbms_output.put_line('Department: ' || RECORD.DEPT_NAME);
	dbms_output.put_line('Salary: ' || RECORD.SALARY);
exception
	when NO_DATA_FOUND then
		dbms_output.put_line('No instructor with name ' || INAME || ' found');
	when TOO_MANY_ROWS then
		dbms_output.put_line('Multiple instructors with name ' || INAME || ' found');
end;
/
