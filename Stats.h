//
// Created by kwoodle on 3/13/18.
//

#ifndef VOTERDATA_STATS_H
#define VOTERDATA_STATS_H

#include <json/reader.h>
#include <string>
using std::string;

// This method of finding duplicate id's takes about 5 minutes to retrieve
// 6 rows for 3 sboeid's. 2 of the 6 rows had STATUS='ACTIVE' and 4 had STATUS='PURGED'
//
const string id_dup {R"%%(
select c2.sboeid, c2.lastname from ward4 as c1 inner join ward4 as c2
on c1.SBOEID = c2.SBOEID and c2.lastname <> c1.lastname
order by SBOEID;
)%%"

};

void row_stats(drk::KSql&);
void hist_stats(drk::KSql&);
#endif //VOTERDATA_STATS_H
