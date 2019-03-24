//
// Created by kwoodle on 2/9/18.
//

#include "Voter.h"
#include <algorithm>
//#include <regex>

// change YYYYMMDD to YYYY-MM-DD
void format_date(string& s)
{
    if (s.length()<8) {
        s = "0000-00-00";
        return;
    }
    s.insert(6, 1, '-');
    s.insert(4, 1, '-');
    return;
}

// change YYYYMMDD to YYYY-MM-DD
void form_date(string& s)
{
    if (s.length()<8) {
        s = "";
        return;
    }
    s.insert(6, 1, '-');
    s.insert(4, 1, '-');
    return;
}



bool not_valid_json(const JRdrPtr& rdr, const string json, string& errs)
{
    Json::Value value;
    return  !rdr->parse(json.data(), json.data()+json.length(), &value, &errs);
}

bool is_date(const int dt)
{
    auto fnd{std::find(std::begin(idates), std::end(idates), dt)};
    return fnd != std::end(idates);
}

//const string vhist_template{R"%%(select voterhistory->'$[*]."<VHIST_RACE>"[*]' as `<VHIST_RACE>`
// from <VHIST_TABLE> where voterhistory->'$[*]."<VHIST_RACE>"[*]' is not null;
//)%%"};

//const string vhist_template{R"%%(select voterhistory->'$[*]."<VHIST_RACE>"[*]' as `<VHIST_RACE>`
//from <VHIST_TABLE> where voterhistory->'$[*]."<VHIST_RACE>"[*]' is not null)%%"};
/*
select count(*) as `count`, timestampdiff(year, dob, '2017-11-08') as age from test.CD1_2018active
        where voterhistory is not null and dob <> '1900-01-01' group by age order by age desc;

select dob, voterhistory->'$[*]."General Election"[*]' as `General Election`
from test.ward4active where json_contains(voterhistory->'$[*]."General Election"[*]', '[2017, 2016]') = 1
 and json_contains(voterhistory->'$[*]."General Election"[*]', '[2011]') = 0;
*/
//const string vhist_template{R"%%(select voterhistory->'$[*]."<VHIST_RACE>"[*]' as `<VHIST_RACE>`
//from <VHIST_TABLE> where JSON_CONTAINS(voterhistory->'$[*]."<VHIST_RACE>"[*]', <VHIST_DATES>) = 1)%%"};

//const string vhist_template{R"%%(select test.age_2017(dob) as 'Age'
//from <VHIST_TABLE> where JSON_CONTAINS(voterhistory->'$[*]."<VHIST_RACE>"[*]', <VHIST_DATES>) = 1)%%"};



/* ED's in email from Larry Tierney
 * Bellport - First tier:
ED 27, (South of S.Country Rd., almost all in the village).
ED 67, (North of S.Country Rd., all in the village, Country Club, etc.)
Brookhaven Hamlet - First tier:
ED 28, (North of Beaver Dam Rd.)
ED 123, (South of Beaver Dam Rd.)
—————————
Bellport - Second tier:
ED 46, (parts of Village, North Bellport  & Brookhaven, big ED).
ED’s 26 & 234 (North Bellport).
ED 194, (North of S.Country Rd., Country Green Rd area).
Brookhaven Hamlet - Second tier:
ED 228 & 289, (B’haven & Yaphank, north of Montauk)
ED 277, (B’haven & Shirley, north of Montauk)

 * */
