use voting;
-- use test;
SELECT LASTNAME, FIRSTNAME, dob, enrollment, REGDATE, left(VoterHistory, 110) as history FROM VotersCD2_24 where
-- SELECT LASTNAME, FIRSTNAME, dob, enrollment, rcity, json_pretty(VoterHistory) as history FROM Suffolk_2019 where
-- SELECT LASTNAME, FIRSTNAME, dob, enrollment, raddnumber, rcity, rstreetname, lastvoteddate, VoterHistory as history FROM Suffolk_2019 where
    (LASTNAME, FIRSTNAME) = ('Shannon','felicia')
or (LASTNAME, FIRSTNAME) = ('McGroary','christine')
or (LASTNAME, FIRSTNAME) = ('Woodle','Kim')
or (LASTNAME, FIRSTNAME) = ('FitzPatrick','Owen')
or (LASTNAME, FIRSTNAME) = ('reilly', 'david')
or (LASTNAME, FIRSTNAME) = ('gilmartin', 'timothy')
# or (LASTNAME, FIRSTNAME) = ('heffernan', 'theresa')
# or (LASTNAME, FIRSTNAME) = ('vanderwyde', 'daniel')
# or (LASTNAME, RCITY)	 = ('gibbons', 'bellport')
# or (lastname, firstname) = ('morse', 'william');