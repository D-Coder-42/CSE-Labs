select NAME,DEPT_NAME
from STUDENT;

select *
from INSTRUCTOR
where DEPT_NAME = 'Comp. Sci.';

select TITLE
from COURSE
where DEPT_NAME = 'Comp. Sci.' and CREDITS = 3;

select TAKES.COURSE_ID, TITLE
from TAKES, COURSE
where TAKES.COURSE_ID = COURSE.COURSE_ID and ID = 12345;

select *
from INSTRUCTOR
where SALARY>=40000 and SALARY<=90000;

select ID from INSTRUCTOR
where ID not in (select ID from TEACHES);

select S.NAME, C.TITLE, T.YEAR
from STUDENT S, COURSE C, TAKES T, SECTION SEC
where S.ID=T.ID 
and C.COURSE_ID=T.COURSE_ID 
and T.COURSE_ID=SEC.COURSE_ID 
and T.SEC_ID=SEC.SEC_ID
and T.YEAR=SEC.YEAR 
and T.SEMESTER=SEC.SEMESTER 
and SEC.ROOM_NUMBER=101;

select S.NAME, C.COURSE_ID, C.TITLE as C_NAME
from STUDENT S, COURSE C, TAKES T
where S.ID=T.ID
and T.COURSE_ID=C.COURSE_ID
and T.YEAR=2010;

select distinct A.NAME, A.SALARY as INST_SALARY
from INSTRUCTOR A,INSTRUCTOR B
where A.SALARY>B.SALARY
and B.DEPT_NAME='Comp. Sci.';

select NAME
from INSTRUCTOR
where DEPT_NAME like '%ch%';

select NAME, LENGTH(NAME)
from STUDENT;

select DEPT_NAME, SUBSTR (DEPT_NAME,3,3)
from DEPARTMENT;

select UPPER (NAME)
from INSTRUCTOR;

select NAME, NVL (SALARY,0)
from INSTRUCTOR;

select SALARY, ROUND (SALARY/3,-2)
from INSTRUCTOR;