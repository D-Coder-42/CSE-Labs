create table EMPLOYEE (
EMP_NO number (10) primary key,
EMP_NAME varchar (20),
EMP_ADDRESS varchar (50));

insert into EMPLOYEE values (10,'ANUSHKA','MANIPAL');
insert into EMPLOYEE values (41,'DHRUV','MUMBAI');
insert into EMPLOYEE values (22,'AVNISH','DELHI');
insert into EMPLOYEE values (40,'AKSHAT','HYDERABAD');
insert into EMPLOYEE values (27,'ARAVINTH','MANGALORE');

select EMP_NAME
from EMPLOYEE;

select *
from EMPLOYEE
where EMP_ADDRESS = 'MANIPAL';

alter table EMPLOYEE add (SALARY number (8));

update EMPLOYEE set SALARY = 3700000 where EMP_NAME = 'DHRUV';
update EMPLOYEE set SALARY = 2300000 where EMP_NAME = 'ANUSHKA';
update EMPLOYEE set SALARY = 2100000 where EMP_NAME = 'ARAVINTH';
update EMPLOYEE set SALARY = 2900000 where EMP_NAME = 'AVNISH';
update EMPLOYEE set SALARY = 1800000 where EMP_NAME = 'AKSHAT';

desc EMPLOYEE

delete from EMPLOYEE where EMP_ADDRESS = 'MANGALORE';

rename EMPLOYEE to EMPLOYEE1;

drop table EMPLOYEE1;