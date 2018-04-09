//
// Created by kwoodle on 2/16/18.
//

#include <ktrade/Ksql.h>
#include <ktrade/Util.h>
#include <boost/program_options.hpp>
#include "Stats.h"
#include "Voter.h"

using namespace std;
namespace po = boost::program_options;

int main()
{
    // Get parameters from config file
    string service, user, pass;
    string database;
    ifstream cfg("../stats_test.ini");
//    ifstream cfg("../voter.ini");
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
//    row_stats(kSql);
//    hist_stats(kSql);

    string mysq{
            "/usr/bin/mysql -e 'select (2017 - year(dob)) as `Age`, year(lastvoteddate) as `lastvoted` from ward4active "
            "where lastvoteddate is not NULL' test > ../gnuplot/w4activedata"};
    drk::get_from_cmd(mysq);

    string mysq2{"/usr/bin/mysql -e 'select (2017 - year(dob)) as `Age`, year(lastvoteddate) as `lastvoted` from ward4 "
                 "where lastvoteddate is not NULL' test > ../gnuplot/w4data"};
    drk::get_from_cmd(mysq2);
    string gplot = R"%%(/usr/bin/gnuplot --persist)%%";
    cout << "\n" << gplot << "\n";
    FILE* GNU;
    GNU = popen(gplot.c_str(), "w");
    if (GNU==NULL) {
        std::cerr << "Error opening pipe to GNU plot!\n";
        exit(1);
    }
    string cmd = R"%%(
col = "Age"
col2 = "lastvoted"
stats '../gnuplot/w4activedata' using col name "Aa" nooutput
stats '../gnuplot/w4activedata' using col2 name "la" nooutput
stats '../gnuplot/w4data' using col name "A" nooutput
stats '../gnuplot/w4data' using col2 name "l" nooutput
mina = Aa_min
maxa = Aa_max
minla = la_min
maxla = la_max
)%%";
    fprintf(GNU, cmd.c_str());
    string cmd2 = R"%%(set boxwidth 1
set style fill pattern 4
set tics out nomirror
set ylabel 'counts'
set multiplot layout 2,1 title 'Age (ward4)'
#set title 'Age'
#unset key
set key left
set yrange [0:*]
set xrange [mina-1:maxa+1]
plot '../gnuplot/w4data' using (column(col)):(1) smooth frequency with boxes fill pattern 4 title 'all', \
     '../gnuplot/w4activedata' using (column(col)):(1) smooth frequency with boxes fill pattern 4 title 'active'
set xrange [minla-1:maxla+1]
#set style fill pattern 5
set xrange [mina-1:40]
#plot '../gnuplot/w4activedata' using (column(col2)):(1) smooth frequency with boxes lc 2 title col2
plot '../gnuplot/w4data' using (column(col)):(1) smooth frequency with boxes fill pattern 4 title 'all', \
     '../gnuplot/w4activedata' using (column(col)):(1) smooth frequency with boxes fill pattern 4 title 'active'
unset multiplot
pause -1 "Hit return to continue"
)%%";
    fprintf(GNU, cmd2.c_str());
    pclose(GNU);
    string script{cmd+cmd2};
    string scrname{"../gnuplot/script.gnu"};
    ofstream scrs(scrname);
    if (!scrs) {
        std::cerr << "Failed to open " << script << "\n";
        return 1;
    }
    scrs << script;


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
    sqlstring = "select count(*) as 'total active' from ward4active";
    res = kSql.ExcuteQuery(sqlstring);
    while (res->next()) {
        total_active_ward4 = res->getInt("total active");
    }
    long total_active_CD1;
    sqlstring = "select count(*) as 'total active' from CD1_2018active";
    res = kSql.ExcuteQuery(sqlstring);
    while (res->next()) {
        total_active_CD1 = res->getInt("total active");
    }
    cout << total_cd1 << "\tRows in CD1\n";
    cout << total_active_CD1 << "\tRows in CD1_2018active\n";
    cout << total_ward4 << "\trows in ward4\n";
    cout << total_active_ward4 << "\tRows in ward4active\n";

/*  Test shows not to worry about duplicate SBOEID's
 *
    res = kSql.ExcuteQuery(id_dup);
    while (res->next()) {
        string s = res->getString("SBOEID");
        string l = res->getString("LASTNAME");
        cout << s <<"\t\t"<<l<< "\n";
    }
*/
    delete (res);
}

void hist_stats(drk::KSql& kSql) {
    Json::CharReaderBuilder rbuilder;
    JRdrPtr const rdr(rbuilder.newCharReader());
    // NO prevyearvoted in CD1_2018 file! "optional from registration form"
    string sqlstr("select lastname, lastvoteddate as lvd, voterhistory"
                  ", regdate from ward4active where lastname like 'wood%'");
//    string sqlstr("select lastname, lastvoteddate as lvd, voterhistory, regdate from CD1_2018");
    auto res = kSql.ExcuteQuery(sqlstr);
    while (res->next()) {
        string nm{res->getString("lastname")};
        string hist{res->getString("voterhistory")};
        string lvd{res->getString("lvd")};
        string regd{res->getString("regdate")};
        lvd = lvd=="" ? "NULL" : lvd;
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