-- 1. Based on the University database Schema in Lab 2, write a row trigger that records along with the time any change made in the Takes (ID, course-id, sec-id, semester, year, grade) table in log_change_Takes (Time_Of_Change, ID, courseid,sec-id, semester, year, grade).

create table LOG_CHANGE_TAKES (
	TIME_OF_CHANGE	timestamp,
	ID		varchar(5),
	COURSE_ID	varchar(8),
	SEC_ID		varchar(8),
	SEMESTER	varchar(6),
	YEAR		number(4),
	GRADE		varchar(2)
);

create or replace trigger TAKES_TRIGGER
before update on TAKES
for each row
begin
	insert into LOG_CHANGE_TAKES values (
		SYSTIMESTAMP,
		:old.ID,
		:old.COURSE_ID,
		:old.SEC_ID,
		:old.SEMESTER,
		:old.YEAR,
		:old.GRADE
	);
end;
/

update TAKES set GRADE = 'F' where GRADE is null;
select * from LOG_CHANGE_TAKES;

/*
OUTPUT:
TIME_OF_CHANGE
---------------------------------------------------------------------------
ID    COURSE_I SEC_ID   SEMEST       YEAR GR
----- -------- -------- ------ ---------- --
03-APR-25 02.16.56.330000 PM
98988 BIO-301  1        Summer       2010
*/

-- 2. Based on the University database schema in Lab: 2, write a row trigger to insert the existing values of the Instructor (ID, name, dept-name, salary) table into a new table Old_Data_Instructor (ID, name, dept-name, salary) when the salary table is updated.

create table OLD_DATA_INSTRUCTOR (
	ID		varchar(5),
	NAME		varchar(20),
	DEPT_NAME	varchar(20),
	SALARY		number(8,2)
);

create or replace trigger INST_TRIGGER
before update of SALARY on INSTRUCTOR
for each row
begin
	insert into OLD_DATA_INSTRUCTOR values (
		:old.ID,
		:old.NAME,
		:old.DEPT_NAME,
		:old.SALARY
	);
end;
/

update INSTRUCTOR set SALARY = SALARY*1.10 where DEPT_NAME = 'Music';
select * from OLD_DATA_INSTRUCTOR;

/*
OUTPUT:
ID    NAME                 DEPT_NAME                SALARY
----- -------------------- -------------------- ----------
15151 Mozart               Music                     44100
*/

-- 3. Based on the University Schema, write a database trigger on Instructor that checks the following: 
--  The name of the instructor is a valid name containing only alphabets. 
--  The salary of an instructor is not zero and is positive 
--  The salary does not exceed the budget of the department to which the instructor belongs. 

create or replace trigger CHECK_INST
before insert or update on INSTRUCTOR
for each row
declare
	DEPT_BUDGET UNIDEPARTMENT.BUDGET %type;
begin
	if not regexp_like (:new.NAME, '^[A-Za-z ]+$') then
		raise_application_error(-20001, 'Instructor name must contain only alphabets.');
	end if;
	select BUDGET into DEPT_BUDGET from UNIDEPARTMENT where DEPT_NAME = :new.DEPT_NAME;
	if :new.SALARY < 0 then
		raise_application_error(-20002, 'Salary cannot be a negative number');
	end if;
	if :new.SALARY > DEPT_BUDGET then
		raise_application_error(-20003, 'Salary exceeds department budget');
	end if;
end;
/

/*
OUTPUT:
insert into INSTRUCTOR values (05252, 'Dhruv', 'Physics', 80000)
            *
ERROR at line 1:
ORA-20003: Salary exceeds department budget
ORA-06512: at "C41.CHECK_INST", line 12
ORA-04088: error during execution of trigger 'C41.CHECK_INST'
*/

-- 4. Create a transparent audit system for a table Client_master (client_no, name, address, Bal_due). The system must keep track of the records that are being deleted or updated. The functionality being when a record is deleted or modified the original record details and the date of operation are stored in the auditclient (client_no, name, bal_due, operation, userid, opdate) table, then the delete or update is allowed to go through. 

create table CLIENT_MASTER (
	CLIENT_NO	number(5)	primary key,
	NAME		varchar(20),
	ADDRESS		varchar(20),
	BAL_DUE		number
);

create table AUDIT_CLIENT (
	CLIENT_NO	number(5),
	NAME		varchar(20),
	BAL_DUE		number,
	OPERATION	varchar(6),
	USERID		varchar(3),
	OPDATE		date
);

create or replace trigger AUDIT_TRIGGER
before update or delete on CLIENT_MASTER
for each row
declare
	OPER	AUDIT_CLIENT.OPERATION %type;
begin
	case
	when updating then
		OPER := 'UPDATE';
	when deleting then
		OPER := 'DELETE';
	end case;
	insert into AUDIT_CLIENT values (
		:old.CLIENT_NO,
		:old.NAME,
		:old.BAL_DUE,
		OPER,
		USER,
		SYSDATE
	);
end;
/

insert into CLIENT_MASTER values (05252, 'Dhruv', 'Mumbai', 42000);
update CLIENT_MASTER set BAL_DUE = 37000 where CLIENT_NO = 05252;
delete from CLIENT_MASTER where CLIENT_NO = 05252;
select * from AUDIT_CLIENT;

/*
OUTPUT:
 CLIENT_NO NAME                    BAL_DUE OPERAT USE OPDATE
---------- -------------------- ---------- ------ --- ---------
      5252 Dhruv                     42000 UPDATE C41 03-APR-25
      5252 Dhruv                     37000 DELETE C41 03-APR-25
*/

-- 5. Based on the University database Schema in Lab 2, create a view Advisor_Student Which is a natural join on Advisor, Student and Instructor tables. Create an INSTEAD OF trigger on Advisor Student to enable the user to delete the corresponding entries in Advisor table.

create view ADVISOR_STUDENT as (
	select S_ID, STUDENT.NAME S_NAME, TOT_CRED, I_ID, INSTRUCTOR.NAME I_NAME, SALARY, STUDENT.DEPT_NAME DEPT_NAME
	from ADVISOR
	join STUDENT on S_ID = STUDENT.ID
	join INSTRUCTOR on I_ID = INSTRUCTOR.ID
);

create or replace trigger DELETE_ADVISOR
instead of delete on ADVISOR_STUDENT
for each row
begin
	delete from ADVISOR where S_ID = :old.S_ID;
end;
/

delete from ADVISOR_STUDENT where S_ID = '76543';
select * from ADVISOR;

/*
OUTPUT:
S_ID  I_ID
----- -----
00128 45565
12345 10101
23121 76543
44553 22222
45678 22222
76653 98345
98765 98345
98988 76766
*/
