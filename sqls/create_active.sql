-- use test;
-- use voting;
drop table if exists `CD1_2018active`;
create table `CD1_2018active` as select * from `CD1_2018` where `STATUS` = 'ACTIVE';
alter table `CD1_2018active` drop column `STATUS`;
alter table `CD1_2018active` add PRIMARY KEY (`SBOEID`,`LASTNAME`);
-- 09:09:04	create table `CD1_2018active` as select * from `CD1_2018` where `STATUS` = 'ACTIVE'
-- 473072 row(s) affected Records: 473072  Duplicates: 0  Warnings: 0	7.348 sec

-- 09:09:11	alter table `CD1_2018active` drop column `STATUS`
-- 0 row(s) affected Records: 0  Duplicates: 0  Warnings: 0	6.754 sec

-- 09:09:18	alter table `CD1_2018active` add PRIMARY KEY (`SBOEID`,`LASTNAME`)
-- 0 row(s) affected Records: 0  Duplicates: 0  Warnings: 0	8.674 sec
