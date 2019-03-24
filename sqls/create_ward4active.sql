-- use test;
-- use voting;
drop table if exists `ward4active`;
create table `ward4active` as select * from `ward4` where `STATUS` = 'ACTIVE';
alter table `ward4active` drop column `STATUS`;
alter table `ward4active` add PRIMARY KEY (`SBOEID`,`LASTNAME`);

-- 09:13:39	create table `ward4active` as select * from `ward4` where `STATUS` = 'ACTIVE'
-- 46890 row(s) affected Records: 46890  Duplicates: 0  Warnings: 0	0.727 sec

-- 09:13:40	alter table `ward4active` drop column `STATUS`
-- 0 row(s) affected Records: 0  Duplicates: 0  Warnings: 0	0.696 sec

-- 09:13:40	alter table `ward4active` add PRIMARY KEY (`SBOEID`,`LASTNAME`)
-- 0 row(s) affected Records: 0  Duplicates: 0  Warnings: 0	0.968 sec
