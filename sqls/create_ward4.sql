-- use test;
-- use voting;
drop table if exists `ward4`;
create table `ward4` as select * from `CD1_2018` where ward='4';
alter table `ward4` drop column ward;
alter table `ward4` add PRIMARY KEY (`SBOEID`,`STATUS`,`LASTNAME`);

-- 09:10:59	create table `ward4` as select * from `CD1_2018` where ward='4'
-- 65869 row(s) affected Records: 65869  Duplicates: 0  Warnings: 0	1.560 sec

-- 09:11:01	alter table `ward4` drop column ward
-- 0 row(s) affected Records: 0  Duplicates: 0  Warnings: 0	0.976 sec

-- 09:11:02	alter table `ward4` add PRIMARY KEY (`SBOEID`,`STATUS`,`LASTNAME`)
-- 0 row(s) affected Records: 0  Duplicates: 0  Warnings: 0	1.391 sec