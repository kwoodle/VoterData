use test;
-- use voting;
drop table if exists `CD1_2019`;
create table `CD1_2019` as select * from `Suffolk_2019` where `CD` = '1';
alter table `CD1_2019` drop column `CD`;
alter table `CD1_2019` add PRIMARY KEY (`SBOEID`,`STATUS`,`LASTNAME`);
