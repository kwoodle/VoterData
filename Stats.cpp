//
// Created by kwoodle on 2/16/18.
//

#include <ktrade/Ksql.h>
#include <boost/program_options.hpp>
#include <fstream>
#include "Stats.h"
#include "Voter.h"

using namespace std;
namespace po = boost::program_options;

int main()
{

    // Get parameters from config file
    string service, user, pass;
    string database;
//    ifstream cfg("../stats_test.ini");
    ifstream cfg("../voter.ini");
    if (!cfg.is_open()) {
        cout << "Failed to open config file\n";
        return 1;
    }
    po::options_description desc("Config");
    desc.add_options()
            ("mysql.service", po::value<string>())
            ("mysql.user", po::value<string>())
            ("mysql.password", po::value<string>())
            ("mysql.database", po::value<string>());

    po::variables_map vm;

    // set third parameter to true to allow unregistered options
    // in config file.
    store(parse_config_file(cfg, desc, true), vm);
    notify(vm);

    service = vm["mysql.service"].as<string>();
    user = vm["mysql.user"].as<string>();
    pass = vm["mysql.password"].as<string>();
    database = vm["mysql.database"].as<string>();

    drk::KSql kSql(service, user, pass);
    kSql.Execute("use "+database);
    row_stats(kSql);
    hist_stats(kSql);
    return 0;
}

void row_stats(drk::KSql& kSql)
{
    string sqlstring{"select count(*) as `total` from ward4"};
    long total_ward4;
    auto res = kSql.ExcuteQuery(sqlstring);
    while (res->next()) {
        total_ward4 = res->getInt("total");
    }
    long total_cd1;
    sqlstring = "select count(*) as `total` from CD1_2018";
    res = kSql.ExcuteQuery(sqlstring);
    while (res->next()) {
        total_cd1 = res->getInt("total");
    }
    long total_active_ward4;
    sqlstring = "select count(*) as 'total active' from ward4act";
    res = kSql.ExcuteQuery(sqlstring);
    while (res->next()) {
        total_active_ward4 = res->getInt("total active");
    }
    long total_active_CD1;
    sqlstring = "select count(*) as 'total active' from CD1_2018act";
    res = kSql.ExcuteQuery(sqlstring);
    while (res->next()) {
        total_active_CD1 = res->getInt("total active");
    }
    cout << total_cd1 << "\tRows in CD1\n";
    cout << total_active_CD1 << "\tRows in CD1_2018act\n";
    cout << total_ward4 << "\trows in ward4\n";
    cout << total_active_ward4 << "\tRows in ward4act\n";

/*  Test shows not to worry about duplicate SBOEID's
 *
    res = kSql.ExcuteQuery(id_dup);
    while (res->next()) {
        string s = res->getString("SBOEID");
        string l = res->getString("LASTNAME");
        cout << s <<"\t\t"<<l<< "\n";
    }
*/
}

void hist_stats(drk::KSql& kSql) {
    Json::CharReaderBuilder rbuilder;
    JRdrPtr const rdr(rbuilder.newCharReader());
    // NO prevyearvoted in CD1_2018 file! "optional from registration form"
    string sqlstr("select lastname, lastvoteddate as lvd, voterhistory"
            ", regdate from ward4act where lastname like 'wood%'");
//    string sqlstr("select lastname, lastvoteddate as lvd, voterhistory, regdate from CD1_2018");
    auto res = kSql.ExcuteQuery(sqlstr);
    while (res->next()) {
        string nm{res->getString("lastname")};
        string hist{res->getString("voterhistory")};
        string lvd{res->getString("lvd")};
        string regd{res->getString("regdate")};
        if(lvd == "0000-00-00")
            lvd = "none";
        Json::Value val;
        string errs;
        bool good{rdr->parse(hist.data(), hist.data()+hist.length(), &val, &errs)};
        if (!good) continue;
        auto dif{val.end()-val.begin()};
        if (dif==1) {
            // no voter history
            cout << nm << "\tnone\tnone\t" << dif-1 << "\t" << lvd << "\n";
            continue;
        }
        else {

            int y2 = val[0][1].asInt();
            int y1 = val[dif-2][1].asInt();
            cout << nm << "\t" << y1 << "\t" << y2 << "\t" << dif-1 << "\t" << lvd <<"\t"<<"\t"<<regd<< "\n";
            continue;
        }
    }

}