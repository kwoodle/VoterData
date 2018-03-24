//
// Created by kwoodle on 3/21/18.
//

#include "Voter.h"
#include <ktrade/Ksql.h>
#include <boost/program_options.hpp>
#include <fstream>
#include <regex>

using namespace std;
namespace po = boost::program_options;

int main()
{

    // Get parameters from config file
    string service, user, pass;
    string database, table, source_file;
//    ifstream cfg("../stats_test.ini");
    ifstream cfg("../insert_test.ini");
    if (!cfg.is_open()) {
        cout << "Failed to open config file\n";
        return 1;
    }
    po::options_description desc("Config");
    desc.add_options()
            ("mysql.service", po::value<string>())
            ("mysql.user", po::value<string>())
            ("mysql.password", po::value<string>())
            ("files.source_file", po::value<string>())
            ("mysql.database", po::value<string>())
            ("mysql.table", po::value<string>());

    po::variables_map vm;

    // set third parameter to true to allow unregistered options
    // in config file.
    store(parse_config_file(cfg, desc, true), vm);
    notify(vm);

    service = vm["mysql.service"].as<string>();
    user = vm["mysql.user"].as<string>();
    pass = vm["mysql.password"].as<string>();
    database = vm["mysql.database"].as<string>();
    table = vm["mysql.table"].as<string>();
    source_file = vm["files.source_file"].as<string>();
    cfg.close();

    // source csv file, e.g. data from Board of Elections
    ifstream infile(source_file);
    if (!infile.is_open()) {
        cout << "Failed to open source file " << source_file << endl;
        return 1;
    }

    drk::KSql kSql(service, user, pass);
    kSql.Execute("use "+database);

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


    drk::Cols cols{kSql.get_cols(database, table)};
    vector<vector<string>> pre_outs;
    vector<map<string, string>> to_set;
    const string regxstr{R"%%("(.*?)"[,\r])%%"};    // the original file has \r\n!!
    regex pat{regxstr};
    cout << "rexstr = " << regxstr << endl;
    smatch m;
    string line;
    while (getline(infile, line)) {
        int indx{0};
        map<string, string> lnmap;
        string fnd{line};
        while (regex_search(fnd, m, pat)) {
            if (m[1].length()>0) {
                string cnam{cols[indx].first};
                if (is_date(indx)) {
                    string m1{m[1]};
                    form_date(m1);
                    lnmap[cnam] = m1;
                }
                else {
                    lnmap[cnam] = m[1];
                }
            }
            ++indx;
            fnd = m.suffix();
        }
        to_set.push_back(lnmap);
    }
    infile.close();

/*    for (auto m:to_set) {
        for (auto e:m) {
            cout << "<" << e.first << "->" << e.second << ">";
        }
        cout << endl;
    }
*/
    // json-ize the voter history,
    // and congeal the strings

    const string quote{"\""};
    const string ostart{"[["};
    const string split{"], ["};
    const string pats2{"(.+?),(.+?)"};
    const string pats{"(.+?),(.+?);"};
    // const string pats_close{"(.+?),(.+?);"}; // misses the last on list

    pat = pats;
    regex pat2{pats2};

    cout << "pats = " << pats << endl;
    for (auto& map:to_set) {
        auto fnd = map.find("VoterHistory");
        if (fnd!=map.end()) {
            string json{ostart};
            while (regex_search(fnd->second, m, pat)) {
                json += quote;
                json += m[1];
                json += quote+", ";
                json += m[2];
                json += split;
                fnd->second = m.suffix();
                continue;
            }
            if (regex_match(fnd->second, m, pat2)) {
                json += quote;
                json += m[1];
                json += quote+", ";
                json += m[2];
                json += split;
            }
            json += "]]";
            string err;
            if (json.length()<5 or not_valid_json(rdr, json, err)) {
//                cout << "json errors " << err << endl;
                map.erase(fnd);
//                json = "[[\"Invalid JSON\", 2018]]";
            }
            else {
//                cout<<json<<endl;
                json.erase(json.length()-5,4);
                fnd->second = json;
            }
        }
    }
/*    for (auto m:to_set) {
        for (auto e:m) {
            cout << "<" << e.first << "->" << e.second << ">";
        }
        cout << endl;
    }
*/
    // Use sql insert instead of load data infile to handle nulls correctly
    // INSERT INTO table SET field1 = 'value1', field2 = 'value2';

    const string bktk{"`"};
    const string sq{"'"};
    string trunc = "truncate table "+table;
    kSql.Execute(trunc);
    string in0 = "insert ignore into "+table+" set ";
    kSql.set_autocommit(false);
    for (auto m:to_set) {
        string ins{in0};
        for (auto e:m) {
//            cout << "<" << e.first << "->" << e.second << ">";
            if(e.first == "VoterHistory")
                ins += e.first + "=" + sq+ e.second +sq+ ", ";
            else
                ins += e.first + "=" + quote+ e.second +quote+ ", ";

        }
//        cout << endl;
        ins.erase(ins.length()-2,2);
//        cout<<ins<<endl;
        kSql.Execute(ins);
        continue;
    }
    kSql.commit();

    return 0;
}
