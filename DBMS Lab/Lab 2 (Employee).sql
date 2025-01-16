create table EMPLOYEE (
EMPNO	number(5)	primary key,
EMPNAME	varchar(15)	not null,
GENDER	char(1)		not null check(GENDER='M' or GENDER='F'),
SALARY	number(8)	not null,
ADDRESS	varchar(20)	not null,
DNO	number(4));

create table DEPARTMENT (
DEPTNO		number(4)	primary key,
DEPTNAME	varchar(15)	unique,
LOCATION	varchar(15));

alter table EMPLOYEE add constraint EMP_FK
foreign key (DNO) references DEPARTMENT(DEPTNO);

insert into DEPARTMENT values (905,'CSE','AB5');
insert into DEPARTMENT values (933,'AERO','AB1');
insert into DEPARTMENT values (926,'ECE','AB2');

insert into EMPLOYEE values (252,'Dhruv','M',374200,'Mumbai',905);
insert into EMPLOYEE values (050,'Anushka','F',110900,'Chennai',905);
insert into EMPLOYEE values (024,'Avnish','F',284500,'Delhi',933);
insert into EMPLOYEE values (192,'Krishanu','M',97300,'Bangalore',926);

/* the following queries will return error */
insert into EMPLOYEE values (122,'Akshat','O',null,'Hyderabad',905);
insert into EMPLOYEE values (074,'Aakarsh','M',69000,'Noida',911);
insert into DEPARTMENT values (911,'CSE','AB4');
delete from DEPARTMENT where DEPTNO=905;
/* till here */

alter table EMPLOYEE drop constraint EMP_FK;
alter table EMPLOYEE add constraint EMP_FK
foreign key (DNO) references DEPARTMENT(DEPTNO) on delete cascade;

alter table EMPLOYEE modify 
SALARY 	number(8)	default(10000);
insert into EMPLOYEE(EMPNO,EMPNAME,GENDER,ADDRESS,DNO) values (122,'Akshat','M','Hyderabad',933);

alter table EMPLOYEE add (DOB date);

update EMPLOYEE set DOB='11-OCT-2005' where EMPNO=252;
update EMPLOYEE set DOB='28-FEB-2005' where EMPNO=050;
update EMPLOYEE set DOB='17-JUN-2005' where EMPNO=192;
update EMPLOYEE set DOB='28-APR-2005' where EMPNO=024;
update EMPLOYEE set DOB='20-JAN-2005' where EMPNO=122;

select TO_CHAR(DOB,'DD-MON-YYYY')
from EMPLOYEE;
select TO_CHAR(DOB,'DD-MON-YY')
from EMPLOYEE;
select TO_CHAR(DOB,'DD-MM-YY')
from EMPLOYEE;