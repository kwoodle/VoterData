//
// Created by kwoodle on 3/21/18.
//
// wc -l CD1suf_01292018.txt 656715
// count(*)                  656441 when Primary Key (SBOEID, STATUS, LASTNAME) and insert ignore
// count(*)                  656436 when Primary Key (SBOEID, STATUS) and insert ignore
// count(*)                  655168 when Primary Key (SBOEID, LASTNAME) and insert ignore


#include "Voter.h"
#include <ktrade/Ksql.h>
#include <boost/program_options.hpp>
#include <fstream>
#include <regex>

using namespace std;
namespace po = boost::program_options;

int main() {
    drk::MySqlOptions opts;
    drk::KSql kSql(opts);

    // Get parameters from config file
    string database, table, table_nohist, source_file;
    bool do_table, debug;
//    ifstream cfg("../insert.ini");
    ifstream cfg("../insert_test.ini");
    if (!cfg.is_open()) {
        cout << "Failed to open config file\n";
        return 1;
    }
    po::options_description desc("Config");
    desc.add_options()
            ("files.source_file", po::value<string>())
            ("mysql.database", po::value<string>())
            ("mysql.table_nohist", po::value<string>())
            ("mysql.do_table", po::value<bool>())
            ("mysql.debug", po::value<bool>())
            ("mysql.table", po::value<string>());

    po::variables_map vm;

    // set third parameter to true to allow unregistered options
    // in config file.
    store(parse_config_file(cfg, desc, true), vm);
    notify(vm);

    database = vm["mysql.database"].as<string>();
    table = vm["mysql.table"].as<string>();
    table_nohist = vm["mysql.table_nohist"].as<string>();
    do_table = vm["mysql.do_table"].as<bool>();
    debug = vm["mysql.debug"].as<bool>();
    source_file = vm["files.source_file"].as<string>();
    cfg.close();

    // source csv file, e.g. data from Board of Elections
    ifstream infile(source_file);
    if (!infile.is_open()) {
        cout << "Failed to open source file " << source_file << endl;
        return 1;
    }
    // Connect to MySQL

    // To read/validate VoterHistory JSON
    Json::CharReaderBuilder rbuilder;
    JRdrPtr const rdr(rbuilder.newCharReader());

    // Voter records all have 45 fields, separated by commas and enclosed
    // in double-quotes. There are commas inside the quotes. VoterHistory
    // is a list separated by semicolons of elements containing brackets and
    // commas. "field","field2",...,"field45"
    // libKtrade provides function for getting column names;
    //     using Cols = std::vector<std::pair<string,string>>;
    //     Cols get_cols(const string& table_schema, const string& table_name);
    //     Cols pcols{kSql.get_cols("cookbook", "profile")};
    //      for(int i=0; i<pcols.size(); ++i) {
    //          cout<<i<<"\t"<<pcols[i].first<<"\t"<<pcols[i].second<<"\n";
    //      }

    // Read/Parse csv file and populate to_set
    //
    drk::Cols cols{kSql.get_cols(database, table)};
    vector<map<string, string>> to_set;
    const string regxstr{R"%%("(.*?)"[,\r])%%"};    // the original file has \r\n!!
    regex pat{regxstr};
    cout << "regxstr = " << regxstr << endl;
    smatch m;
    string line;
    while (getline(infile, line)) {
        int indx{0};
        map<string, string> lnmap;
        string fnd{line};
        while (regex_search(fnd, m, pat)) {
            if (m[1].length() > 0) {
                string cnam{cols[indx].first};
                if (is_date(indx)) {
                    string m1{m[1]};
                    form_date(m1);
                    lnmap[cnam] = m1;
                } else {
                    // This didn't work 07/24/2020 because get_cols returned column names alphabetically
                    // fixed in libKtrade
                    lnmap[cnam] = m[1];
                }
            }
            ++indx;
            fnd = m.suffix();
        }
        to_set.push_back(lnmap);
    }
    infile.close();

    if (debug) {
        for (auto m:to_set) {
            for (auto e:m) {
                cout << "<" << e.first << "->" << e.second << ">";
            }
            cout << endl;
        }
    }

    // json-ize the voter history,
    // and congeal the strings
    //
    // Prior version produced JSON list of lists
    //
// ACHUTHAN!NY000000000009695440![["General Election",  2017], ["Primary Election",  2017], ["General Election",  2016],
// ["Federal Primary",  2016], ["Presidential Primary",  2016], ["General Election",  2015], ["General Election",  2014],
// ["General Election",  2013], ["General Election",  2012], ["General Election",  2010], ["General Election",  2008],
// ["Presidential Primary",  2008], ["General Election",  2006], ["General Election",  2004], []]
    const string quote{"\""};
    const string pats2{"(.+?),(.+?)"};
    const string pats{"(.+?),(.+?);"};
    // const string pats_close{"(.+?),(.+?);"}; // misses the last on list

    pat = pats;
    regex pat2{pats2};

    cout << "pats = " << pats << endl;
    //
    // Better will be list of objects. Each object has {key:value} like {"General Election":[2017, 2016, 2015...]}
    // starting from
//  <VoterHistory->General Election, 2012;General Election, 2008;Primary Election, 2005;General Election, 2004;General Election, 2003>
    vector<map<string, string>> to_set_noVhist;

    for (auto &field_map:to_set) {
        map<string, string> field_map_noVhist;
        bool Vhist_err{false};
        auto findVH = field_map.find("VoterHistory");
        if (findVH != field_map.end()) {
            map<string, vector<string>> mj;
            while (regex_search(findVH->second, m, pat)) {
                string key{m[1]};
                string val{m[2]};
                findVH->second = m.suffix();
                mj[key].push_back(val);
            }
            if (regex_match(findVH->second, m, pat2)) {
                string key{m[1]};
                string val{m[2]};
                mj[key].push_back(val);
            }
            auto sz{mj.size()};
            if (sz == 0) {
                auto st{field_map.find("STATUS")};
                auto ln{field_map.find("LASTNAME")};
                cout << "Unexpected Voter History Layout\n";
                cout << findVH->second << "\n";
                cout << "Status" << st->second << "\n";
                cout << "Lastname" << ln->second << "\n";
                field_map_noVhist = field_map;
                Vhist_err = true;
                field_map.erase(findVH);
            } else {
                string json;
                json = "[{";
// {"General Election":[2017, 2016, 2015...]}
                for (auto j:mj) {
                    json += quote;
                    json += j.first + quote + " : ";
                    json += "[";
                    for (auto yr:j.second) {
                        json += yr + ", ";
                    }
                    json.erase(json.length() - 2);
                    json += "]";
                    json += "}, {";
                }
                json.erase(json.length() - 3);
                json += "]";
                string err;
                if (json.length() < 5 or not_valid_json(rdr, json, err)) {
                    cout << json << "\n";
                    cout << "json errors " << err << endl;
                    field_map_noVhist = field_map;
                    Vhist_err = true;
                    field_map.erase(findVH);
                } else {
                    findVH->second = json;
                }
            }
        }
        // convert NY000000000009701159 to int and back to string
        //     string s2{std::regex_replace(s1, pat1, table_schema)};
        auto findID = field_map.find("SBOEID");
        if (findID != field_map.end()) {
            std::regex regex1{"NY"};
            string id{findID->second};
            string id2{std::regex_replace(id, regex1, "")};
            int intid{stoi(id2)};
            findID->second = to_string(intid);
        } else {
            cout << "SBOEID not found!\n";
        }
        if (Vhist_err) {
            to_set_noVhist.push_back(field_map_noVhist);
        }
    }
/*    for (auto m:to_set) {
        for (auto e:m) {
            cout << "<" << e.first << "->" << e.second << ">";
        }
        cout << endl;
    }
*/
    cout << "to_set_noVhist.size = " << to_set_noVhist.size() << endl;
    if (debug) {
        for (auto m:to_set_noVhist) {
            for (auto e:m) {
                cout << "<" << e.first << "->" << e.second << ">";
            }
            cout << endl;
        }
    }

    // Use sql insert instead of load data infile to handle nulls correctly
    // INSERT INTO table SET field1 = 'value1', field2 = 'value2';

    const string sq{"'"};
    string trunc = "truncate table " + database + "." + table;
    if (do_table) {
        kSql.Execute(trunc);
    }
    string in0 = "insert ignore into " + database + "." + table + " set ";
    kSql.set_autocommit(false);
    if (do_table) {
        for (auto item:to_set) {
            string ins{in0};
            for (auto e:item) {
                if (e.first == "VoterHistory")
                    ins += e.first + "=" + sq + e.second + sq + ", ";
                else
                    ins += e.first + "=" + quote + e.second + quote + ", ";
            }
            ins.erase(ins.length() - 2, 2);
            kSql.Execute(ins);
        }
        kSql.commit();
    }
    // MySQL datatype for VoterHistory column in this table is text NOT json
    //
    if (to_set_noVhist.size() > 0) {
        trunc = "truncate table " + database + "." + table_nohist;
        kSql.Execute(trunc);
        in0 = "insert ignore into " + database + "." + table_nohist + " set ";
        for (auto item:to_set_noVhist) {
            string ins{in0};
            for (auto e:item) {
                if (e.first == "VoterHistory")
                    ins += e.first + "=" + sq + e.second + sq + ", ";
                else
                    ins += e.first + "=" + quote + e.second + quote + ", ";
            }
            ins.erase(ins.length() - 2, 2);
            kSql.Execute(ins);
        }
        kSql.commit();
    }
    return 0;
}
