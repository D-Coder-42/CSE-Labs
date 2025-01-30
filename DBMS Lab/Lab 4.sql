select COURSE_ID, count(ID)
from TAKES
group by COURSE_ID;

select DEPT_NAME
from STUDENT
group by DEPT_NAME
having count(ID) > 10;

select DEPT_NAME, count(COURSE_ID)
from COURSE
group by DEPT_NAME;

select DEPT_NAME, avg(SALARY)
from INSTRUCTOR
group by DEPT_NAME
having avg(SALARY) > 42000;

select SEC_ID, count(ID)
from TAKES
where SEMESTER = 'Spring' and YEAR = 2009
group by SEC_ID;

select PREREQ.COURSE_ID, PREREQ.PREREQ_ID 
from PREREQ
order by COURSE_ID;

select *
from INSTRUCTOR
order by SALARY desc;

select max(TOT_SAL)
from (select sum(SALARY) as TOT_SAL
	from INSTRUCTOR
	group by DEPT_NAME);

select DEPT_NAME, AVG_SAL
from (select DEPT_NAME, avg(SALARY) as AVG_SAL
	from INSTRUCTOR
	group by DEPT_NAME 
	having avg(SALARY) > 42000);

select max(ENROLLMENT)
from (select SEC_ID, count(ID) as ENROLLMENT
	from TAKES
	where SEMESTER = 'Spring' and YEAR = 2010
	group by SEC_ID);

select NAME
from (select distinct NAME
	from TEACHES, INSTRUCTOR
	where TEACHES.ID = INSTRUCTOR.ID
	and COURSE_ID like 'CS%');

/*
SELECT DISTINCT I.NAME
FROM INSTRUCTOR I
WHERE NOT EXISTS (
    SELECT S.ID
    FROM STUDENT S
    WHERE S.DEPT_NAME = 'Comp. Sci.'
    AND NOT EXISTS (
        SELECT T.ID
        FROM TAKES T, SECTION Sec, TEACHES Te
        WHERE T.COURSE_ID = Sec.COURSE_ID 
          AND T.SEC_ID = Sec.SEC_ID 
          AND T.SEMESTER = Sec.SEMESTER 
          AND T.YEAR = Sec.YEAR
          AND Sec.COURSE_ID = Te.COURSE_ID 
          AND Sec.SEC_ID = Te.SEC_ID 
          AND Te.ID = I.ID
          AND T.ID = S.ID
    )
);
*/

select avg(AVG_SAL)
from (select count(ID) as NUM_INST, avg(SALARY) as AVG_SAL
	from INSTRUCTOR
	group by DEPT_NAME
	having avg(SALARY) > 50000
	and count(ID) > 5);

with MAX_BUDGET(MB) as
(select MAX(BUDGET) as MB
from DEPARTMENT)
select DEPARTMENT.DEPT_NAME, (select MB from MAX_BUDGET) as BUDGET
from DEPARTMENT
where BUDGET = (select MB from MAX_BUDGET);

with AVG_TOT(AVERAGE) as 
(select avg(TOT_SAL) as AVERAGE
from (select sum(SALARY) as TOT_SAL 
	from INSTRUCTOR 
	group by DEPT_NAME)) 
select DEPT_NAME 
from INSTRUCTOR 
group by DEPT_NAME 
having sum(SALARY) > (select AVERAGE from AVG_TOT);

savepoint Lab4Q14;

insert into DEPARTMENT values ('IT', 'Taylor', 95000);
update STUDENT set DEPT_NAME = 'IT'
where DEPT_NAME = 'Comp. Sci.';

update INSTRUCTOR set SALARY = SALARY * 1.03 where SALARY > 100000;
update INSTRUCTOR set SALARY = SALARY * 1.05 where SALARY <= 100000;

rollback;