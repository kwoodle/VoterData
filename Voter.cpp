//
// Created by kwoodle on 2/9/18.
//

#include "Voter.h"
#include <algorithm>

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
