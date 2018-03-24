mysql> show create table CD1_2018\G
*************************** 1. row ***************************
       Table: CD1_2018
Create Table: CREATE TABLE `CD1_2018` (
  `LASTNAME` varchar(50) NOT NULL,
  `FIRSTNAME` varchar(50) DEFAULT NULL,
  `MIDDLENAME` varchar(50) DEFAULT NULL,
  `NAMESUFFIX` varchar(10) DEFAULT NULL,
  `RADDNUMBER` varchar(10) DEFAULT NULL,
  `RHALFCODE` varchar(10) DEFAULT NULL,
  `RAPARTMENT` varchar(15) DEFAULT NULL,
  `RPREDIRECTION` varchar(10) DEFAULT NULL,
  `RSTREETNAME` varchar(70) DEFAULT NULL,
  `RPOSTDIRECTION` varchar(10) DEFAULT NULL,
  `RCITY` varchar(50) DEFAULT NULL,
  `RZIP5` varchar(5) DEFAULT NULL,
  `RZIP4` varchar(4) DEFAULT NULL,
  `MAILADD1` varchar(100) DEFAULT NULL,
  `MAILADD2` varchar(100) DEFAULT NULL,
  `MAILADD3` varchar(100) DEFAULT NULL,
  `MAILADD4` varchar(100) DEFAULT NULL,
  `DOB` date DEFAULT NULL,
  `GENDER` varchar(1) DEFAULT NULL,
  `ENROLLMENT` varchar(3) DEFAULT NULL, -- DEM, REP, CON, GRE, WOR, IND, WEP, SCC, OTH, BLK
  `OTHERPARTY` varchar(30) DEFAULT NULL,
  `COUNTYCODE` int(2) DEFAULT NULL,
  `ED` int(3) DEFAULT NULL,
  `LD` int(3) DEFAULT NULL,
  `TOWNCITY` varchar(30) DEFAULT NULL,
  `WARD` varchar(3) DEFAULT NULL,
  `CD` int(3) DEFAULT NULL,
  `SD` int(3) DEFAULT NULL,
  `AD` int(3) DEFAULT NULL,
  `LASTVOTEDDATE` date DEFAULT NULL,
  `PREVYEARVOTED` varchar(4) DEFAULT NULL,
  `PREVCOUNTY` varchar(2) DEFAULT NULL,
  `PREVADDRESS` varchar(100) DEFAULT NULL,
  `PREVNAME` varchar(150) DEFAULT NULL,
  `COUNTYVRNUMBER` varchar(50) DEFAULT NULL,
  `REGDATE` date DEFAULT NULL,
  `VRSOURCE` varchar(10) DEFAULT NULL,-- AGCY, CBOE, DMV, LOCALREG, MAIL, SCHOOL
  `IDREQUIRED` varchar(1) DEFAULT NULL, -- Y, N
  `IDMET` varchar(1) DEFAULT NULL, -- Y, N
  `STATUS` varchar(10) DEFAULT NULL, -- ACTIVE, INACTIVE, PURGED, PREREG !In table, not in doc!
  `REASONCODE` varchar(15) DEFAULT NULL, -- NCOA, MOVED, OTHER, COURT, DEATH, MAILCHECK, INACTIVE !In table, not in doc!
  `INACT_DATE` date DEFAULT NULL,
  `PURGE_DATE` date DEFAULT NULL,
  `SBOEID` varchar(50) NOT NULL,
  `VoterHistory` json DEFAULT NULL,
  PRIMARY KEY (`LASTNAME`,`SBOEID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1
1 row in set (0.00 sec)

mysql> quit
