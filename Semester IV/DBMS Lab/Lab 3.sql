select COURSE_ID
from TAKES
where YEAR=2009 and SEMESTER='Fall'
union
select COURSE_ID
from TAKES
where YEAR=2010 and SEMESTER='Spring';

select COURSE_ID
from TAKES
where YEAR=2009 and SEMESTER='Fall'
intersect
select COURSE_ID
from TAKES
where YEAR=2010 and SEMESTER='Spring';

select COURSE_ID
from TAKES
where YEAR=2009 and SEMESTER='Fall'
minus
select COURSE_ID
from TAKES
where YEAR=2010 and SEMESTER='Spring';

select TITLE
from COURSE
minus
select COURSE.TITLE
from TAKES, COURSE
where TAKES.COURSE_ID = COURSE.COURSE_ID;

select COURSE_ID
from SECTION
where YEAR=2009 and SEMESTER='Fall' 
and COURSE_ID in (select COURSE_ID
		from SECTION
		where YEAR=2010 and SEMESTER='Spring');

select count(TAKES.ID)
from TAKES, TEACHES
where TAKES.COURSE_ID = TEACHES.COURSE_ID
and TEACHES.ID in (select ID
		from INSTRUCTOR
		where ID = 10101);

select COURSE_ID
from SECTION
where YEAR=2009 and SEMESTER='Fall' 
and COURSE_ID not in (select COURSE_ID
		from SECTION
		where YEAR=2010 and SEMESTER='Spring');

select NAME
from STUDENT
where NAME in (select NAME
		from INSTRUCTOR);

select NAME
from INSTRUCTOR
where SALARY > some (select SALARY
		from INSTRUCTOR
		where DEPT_NAME = 'Biology');

select NAME
from INSTRUCTOR
where SALARY > all (select SALARY
		from INSTRUCTOR
		where DEPT_NAME = 'Biology');

select DEPT_NAME
from INSTRUCTOR
group by DEPT_NAME
having avg(SALARY) >= all (select avg(SALARY)
		from INSTRUCTOR
		group by DEPT_NAME);

select DEPT_NAME
from DEPARTMENT
where BUDGET < all (select avg(SALARY)
		from INSTRUCTOR);

select COURSE_ID
from SECTION A
where YEAR=2009 and SEMESTER='Fall' 
and exists (select COURSE_ID
		from SECTION B
		where YEAR=2010 and SEMESTER='Spring' and A.COURSE_ID=B.COURSE_ID);

select S.ID, S.NAME
from STUDENT S
where not exists (select COURSE_ID
		from COURSE
		where DEPT_NAME = 'Physics'
		minus
		select T.COURSE_ID
		from TAKES T
		where S.ID=T.ID);

select COURSE_ID
from SECTION
where YEAR=2009
group by COURSE_ID
having count(*) <= 1;

select ID
from TAKES
where COURSE_ID like 'CS-%'
group by ID 
having count(*) >= 2;

select avg(AVG_SALARY)
from (select avg(SALARY) as AVG_SALARY
	from INSTRUCTOR
	group by DEPT_NAME
	having avg(SALARY) > 42000);

create view ALL_COURSES as
select COURSE.COURSE_ID, SEC_ID, BUILDING, ROOM_NUMBER
from SECTION, COURSE
where SECTION.COURSE_ID = COURSE.COURSE_ID
and COURSE.DEPT_NAME = 'Physics' 
and SECTION.SEMESTER = 'Fall'
and SECTION.YEAR = 2009;

select COURSE_ID
from ALL_COURSES;

create view DEPARTMENT_TOTAL_SALARY as
select DEPT_NAME, sum(SALARY) as TOTAL_SALARY
from INSTRUCTOR
group by DEPT_NAME;