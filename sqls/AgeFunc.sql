-- use test
create function age_2017(dob date)
returns int
return timestampdiff(year, dob, '2017-11-08');