-- JSON for voterhistory
--
use test;
select * from CD1_2018 where lastname = 'gershon' and firstname = 'perry';
select firstname, lastname, JSON_EXTRACT(VoterHistory, '$[*].*') from CD1_2018 where lastname = 'gershon'; -- and voterhistory->"$[*][0]" = 'General Election';
select voterhistory, voterhistory->'$[*]."Presidential Primary"' as pp from test.ward4active having voterhistory->'$[*]."Presidential Primary"' is not null limit 50;
select voterhistory->'$[*]."General Election"[*]' as `General Election` 
from test.ward4active where voterhistory->'$[*]."General Election"[*]' is not null limit 50;

select firstname, lastname, JSON_EXTRACT(VoterHistory, '$[*]."General Election"') from ward4active where lastname = 'woodle'; -- and voterhistory->"$[*][0]" = 'General Election';
select firstname, lastname, voterhistory->'$[0]."General Election"' from CD1_2018; -- where lastname = 'woodle';
select firstname, lastname, voterhistory->'$[*]."General Election"' from CD1_2018; -- where lastname = 'woodle';
select firstname, lastname, voterhistory->'$[0]."Federal Primary"' from CD1_2018 where voterhistory->'$[0]."Federal Primary"' is not null; -- where lastname = 'woodle';
select firstname, lastname, voterhistory from CD1_2018; -- where lastname = 'woodle';
select voterhistory, voterhistory->'$[*]."General Election"' as ge from test.ward4active where voterhistory->'$[*]."General Election"' is not null limit 50;
select ENROLLMENT, voterhistory->'$[*]."General Election"[*]' from test.ward4active where voterhistory->'$[*]."General Election"[*]'is not null limit 5;
select voterhistory from test.ward4active where voterhistory->'$[*]."General Election"' is not null limit 3;
select ENROLLMENT, voterhistory from test.ward4active having voterhistory->'$[*]."General Election"[7]' like '20%' limit 5;
select ENROLLMENT, voterhistory->'$[*]."General Election"[*]' from test.ward4active where voterhistory->'$[*]."General Election"[*]'is not null limit 5;
select count(*) as ct, timestampdiff(year, dob, '2017-11-08') as age, ENROLLMENT, voterhistory from test.CD1_2018active where 
	voterhistory->'$[*]."General Election"[*]' = json_array(2017, 2016)
    group by age, ENROLLMENT, voterhistory order by ct desc; -- limit 5;
select 100 - (count(*) - count(VoterHistory))/count(*)*100 as 'Percent not null in active' from test.CD1_2018active;
select 100 - (count(*) - count(VoterHistory))/count(*)*100 as 'Percent not null in all' from test.CD1_2018;
select count(*) as `ct CD1active` from test.CD1_2018active where dob = '1900-01-01';
select count(*) as `ct CD1` from test.CD1_2018 where dob = '1900-01-01';
select count(*) as `ct ward4active` from test.ward4active where dob = '1900-01-01';
select count(*) as `ct ward4` from test.ward4 where dob = '1900-01-01';
select count(*) as `ct CD1active not 1900`,  timestampdiff(year, dob, '2017-11-08') as age
from test.CD1_2018active where dob != '1900-01-01' and timestampdiff(year, dob, '2017-11-08') > 99 group by age;
select count(*) as `ct CD1active incl 1900`,  timestampdiff(year, dob, '2017-11-08') as age
from test.CD1_2018active where timestampdiff(year, dob, '2017-11-08') > 99 group by age;
select * from test.CD1_2018active where timestampdiff(year, dob, '2017-11-08') = 120;
select * from test.CD1_2018active where timestampdiff(year, dob, '2017-11-08') > 99 and VoterHistory is not null
and dob != '1900-01-01' order by dob;
select * from test.ward4active where timestampdiff(year, dob, '2017-11-08') > 99 and VoterHistory is not null
and dob != '1900-01-01' order by dob;
