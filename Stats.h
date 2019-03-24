//
// Created by kwoodle on 3/13/18.
//

#ifndef VOTERDATA_STATS_H
#define VOTERDATA_STATS_H

#include <json/reader.h>
#include <string>
#include <map>
#include <vector>
#include <ktrade/nlohmann/json.hpp>

using std::string;
using std::vector;
using std::map;
using json = nlohmann::json;

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
void vhist_stats(drk::KSql& kSql, const string& table, const string& race);
void vhist(const string& table, const string& race = "");
void histograms();
void dates();

// {"General Election":[2017, 2016, 2015...]}
extern const string vhist_template;

string vhist_replace(string table, string race, string dates);

struct VHist {
    using Vmap = map<string, vector<int>>;
    VHist(const string& s)
    {
        vhist = json::parse(s);
    }
    string dump(const int indent = -1, const char indch = ' ', const bool ens = false)
    {
        return vhist.dump(indent, indch, ens);
    }
    json vhist;
    Vmap vmap;

};
#endif //VOTERDATA_STATS_H
