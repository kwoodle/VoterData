//
// Created by kwoodle on 2/5/18.
//

#ifndef VOTERDATA_VOTER_H
#define VOTERDATA_VOTER_H

#include<string>
#include <vector>
#include <json/reader.h>
#include <memory>

using JRdrPtr = std::unique_ptr<Json::CharReader>;
using std::string;
// std::unique_ptr passed by cons reference
//
bool not_valid_json(const JRdrPtr&,  const string json, string& err);

using namespace std;
const string drop_vhist_json_test
{
    R"%%(truncate table if exists test.HistoryJ;)%%"
};

const string create_vhist_json_test {R"%%(
create table test.HistoryJ(
    LASTNAME varchar(50) not null,
    SBOEID varchar(50) not null,
    VoterHistory json,
    foreign key (SBOEID) references test.SuffTest (SBOEID)
)
)%%"
};

const string drop_vhist_json
{
    R"%%(truncate table if exists voting.HistoryJ;)%%"
};

const string create_vhist_json{R"%%(
create table voting.HistoryJ(
    LASTNAME varchar(50) not null,
    SBOEID varchar(50) not null,
    VoterHistory json,
    foreign key (SBOEID) references voting.Suffolk2018 (SBOEID)
)
)%%"
};


const string drop_Suffolk_test {R"%%(
    truncate table if exists test.SuffTest;
)%%"
};

const string create_Suffolk_test{R"%%(
CREATE TABLE `test.SuffTest` (
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
  `DOB` varchar(8) DEFAULT NULL,
  `GENDER` varchar(1) DEFAULT NULL,
  `ENROLLMENT` varchar(3) DEFAULT NULL,
  `OTHERPARTY` varchar(30) DEFAULT NULL,
  `COUNTYCODE` varchar(2) DEFAULT NULL,
  `ED` int(11) DEFAULT NULL,
  `LD` int(11) DEFAULT NULL,
  `TOWNCITY` varchar(30) DEFAULT NULL,
  `WARD` varchar(3) DEFAULT NULL,
  `CD` int(11) DEFAULT NULL,
  `SD` int(11) DEFAULT NULL,
  `AD` int(11) DEFAULT NULL,
  `LASTVOTEDDATE` varchar(8) DEFAULT NULL,
  `PREVYEARVOTED` varchar(4) DEFAULT NULL,
  `PREVCOUNTY` varchar(2) DEFAULT NULL,
  `PREVADDRESS` varchar(100) DEFAULT NULL,
  `PREVNAME` varchar(150) DEFAULT NULL,
  `COUNTYVRNUMBER` varchar(50) DEFAULT NULL,
  `REGDATE` varchar(8) DEFAULT NULL,
  `VRSOURCE` varchar(10) DEFAULT NULL,
  `IDREQUIRED` varchar(1) DEFAULT NULL,
  `IDMET` varchar(1) DEFAULT NULL,
  `STATUS` varchar(10) DEFAULT NULL,
  `REASONCODE` varchar(15) DEFAULT NULL,
  `INACT_DATE` varchar(8) DEFAULT NULL,
  `PURGE_DATE` varchar(8) DEFAULT NULL,
  `SBOEID` varchar(50) NOT NULL,
  `VoterHistory` text,
  PRIMARY KEY (`LASTNAME`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1
)%%"
};

const string drop_Suffolk {R"%%(
    truncate table if exists voting.Suffolk2018;
)%%"
};

const string create_Suffolk{R"%%(
CREATE TABLE `voting.Suffolk2018` (
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
  `DOB` varchar(8) DEFAULT NULL,
  `GENDER` varchar(1) DEFAULT NULL,
  `ENROLLMENT` varchar(3) DEFAULT NULL,
  `OTHERPARTY` varchar(30) DEFAULT NULL,
  `COUNTYCODE` varchar(2) DEFAULT NULL,
  `ED` int(11) DEFAULT NULL,
  `LD` int(11) DEFAULT NULL,
  `TOWNCITY` varchar(30) DEFAULT NULL,
  `WARD` varchar(3) DEFAULT NULL,
  `CD` int(11) DEFAULT NULL,
  `SD` int(11) DEFAULT NULL,
  `AD` int(11) DEFAULT NULL,
  `LASTVOTEDDATE` varchar(8) DEFAULT NULL,
  `PREVYEARVOTED` varchar(4) DEFAULT NULL,
  `PREVCOUNTY` varchar(2) DEFAULT NULL,
  `PREVADDRESS` varchar(100) DEFAULT NULL,
  `PREVNAME` varchar(150) DEFAULT NULL,
  `COUNTYVRNUMBER` varchar(50) DEFAULT NULL,
  `REGDATE` varchar(8) DEFAULT NULL,
  `VRSOURCE` varchar(10) DEFAULT NULL,
  `IDREQUIRED` varchar(1) DEFAULT NULL,
  `IDMET` varchar(1) DEFAULT NULL,
  `STATUS` varchar(10) DEFAULT NULL,
  `REASONCODE` varchar(15) DEFAULT NULL,
  `INACT_DATE` varchar(8) DEFAULT NULL,
  `PURGE_DATE` varchar(8) DEFAULT NULL,
  `SBOEID` varchar(50) NOT NULL,
  `VoterHistory` text,
  PRIMARY KEY (`LASTNAME`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1
)%%"
};

// dates in voter record, change from 1 to zero based
// DOB - col17, LASTVOTEDDATE  - col29, REGDATE - col 35,
// INACT_DATE - col41, PURGE_DATE - col42
const std::vector<int> idates{17, 29, 35, 41, 42};
bool is_date(const int dt);
void format_date(string&);

// don't create 0000-00-00
void form_date(string&);
//mysql_options(&mysql,MYSQL_INIT_COMMAND,"SET autocommit=0"); C API
#endif //VOTERDATA_VOTER_H
