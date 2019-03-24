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


const string drop_Suffolk {R"%%(
    truncate table if exists voting.Suffolk2018;
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
