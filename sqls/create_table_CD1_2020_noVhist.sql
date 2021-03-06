-- use test;
use voting;
drop table if exists `CD1_2020_noVhist`;
CREATE TABLE `CD1_2020_noVhist` (
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
                                        `ENROLLMENT` enum('DEM', 'REP', 'CON', 'GRE', 'WOR', 'IND', 'WEP', 'SCC', 'OTH', 'BLK', 'REF'),
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
                                        `VRSOURCE` enum('AGCY', 'CBOE', 'DMV', 'LOCALREG', 'MAIL', 'SCHOOL'),
                                        `IDREQUIRED` enum('Y', 'N'),
                                        `IDMET` enum('Y', 'N'),
                                        `STATUS` enum('ACTIVE', 'INACTIVE', 'PURGED', 'PREREG'), --  !In table, not in doc!
                                        `REASONCODE` enum('NCOA', 'MOVED', 'OTHER', 'COURT', 'DEATH', 'MAILCHECK', 'INACTIVE'), --  !In table, not in doc!
                                        `INACT_DATE` date DEFAULT NULL,
                                        `PURGE_DATE` date DEFAULT NULL,
                                        `SBOEID` int NOT NULL,
                                        `VoterHistory` text DEFAULT NULL,
                                        PRIMARY KEY (`SBOEID`, `STATUS`, `LASTNAME`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;