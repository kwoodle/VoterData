//
// Created by kwoodle on 2/16/18.
//

#include <ktrade/Ksql.h>
#include <ktrade/Util.h>
#include <boost/program_options.hpp>
#include "Stats.h"
#include "Voter.h"
#include <regex>

using namespace std;
namespace po = boost::program_options;
//using VHist = vector<map<string, vector<int>>>;
int main()
{
    drk::MySqlOptions opts;
    drk::KSql kSql(opts);
    // Get parameters from config file
    string database;
    ifstream cfg("../stats.ini");
//    ifstream cfg("../voter.ini");
    if (!cfg.is_open()) {
        cout << "Failed to open config file\n";
        return 1;
    }
    po::options_description desc("Config");
    desc.add_options()
            ("mysql.database", po::value<string>());

    po::variables_map vm;

    // set third parameter to true to allow unregistered options
    // in config file.
    store(parse_config_file(cfg, desc, true), vm);
    notify(vm);


    database = vm["mysql.database"].as<string>();

    kSql.Execute("use "+database);
    row_stats(kSql);
//    hist_stats(kSql);
//    histograms();
//    string tbl{R"%%(test.CD1_2018)%%"};
    string tbl{R"%%(test.ward4active)%%"};
//    vhist_stats(kSql, tbl, "General Election");
//    vhist_stats(kSql, tbl, "Primary Election");
//    vhist(tbl, "General Election");
    dates();
    return 0;
}

void row_stats(drk::KSql& kSql)
{
    string sqlstring{"select count(*) as `total` from ward4"};
    long total_ward4{-1};
    auto res = kSql.ExecuteQuery(sqlstring);
    while (res->next()) {
        total_ward4 = res->getInt("total");
    }
    long total_cd1{-1};
    sqlstring = "select count(*) as `total` from CD1_2018";
    res = kSql.ExecuteQuery(sqlstring);
    while (res->next()) {
        total_cd1 = res->getInt("total");
    }
    long total_active_ward4{-1};
    sqlstring = "select count(*) as 'total active' from ward4active";
    res = kSql.ExecuteQuery(sqlstring);
    while (res->next()) {
        total_active_ward4 = res->getInt("total active");
    }
    long total_active_CD1{-1};
    sqlstring = "select count(*) as 'total active' from CD1_2018active";
    res = kSql.ExecuteQuery(sqlstring);
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

void hist_stats(drk::KSql& kSql)
{
    Json::CharReaderBuilder rbuilder;
    JRdrPtr const rdr(rbuilder.newCharReader());
    // NO prevyearvoted in CD1_2018 file! "optional from registration form"
    string sqlstr("select lastname, lastvoteddate as lvd, voterhistory"
                  ", regdate from ward4active where lastname like 'wood%'");
//    string sqlstr("select lastname, lastvoteddate as lvd, voterhistory, regdate from CD1_2018");
    auto res = kSql.ExecuteQuery(sqlstr);
    while (res->next()) {
        string nm{res->getString("lastname")};
        string hist{res->getString("voterhistory")};
        string lvd{res->getString("lvd")};
        string regd{res->getString("regdate")};
        lvd = lvd.empty() ? "NULL" : lvd;
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

void histograms()
{
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
    if (GNU==nullptr) {
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
    fprintf(GNU, "%s", cmd.c_str());
    string cmd2 = R"%%(set boxwidth 1
set style fill pattern 4
set tics out nomirror
set ylabel '# of voters'
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
    fprintf(GNU, "%s", cmd2.c_str());
    pclose(GNU);
    string script{cmd+cmd2};
    string scrname{"../gnuplot/script.gnu"};
    ofstream scrs(scrname);
    if (!scrs) {
        std::cerr << "Failed to open " << script << "\n";
        return;
    }
    scrs << script;
}
// select firstname, lastname, voterhistory->'$[0]."Federal Primary"' from CD1_2018 where voterhistory->'$[0]."Federal Primary"' is not null; -- where lastname = 'woodle';
void vhist_stats(drk::KSql& kSql, const string& table, const string& race)
{
    string sql = R"%%(select voterhistory->'$[*].")%%";
    sql += race+R"%%("')%%";
    sql += " from "+table+R"%%( where voterhistory->'$[*].")%%";
    sql += race+R"%%("' is not null)%%";
    cout << "sql = " << sql << "\n";

    Json::CharReaderBuilder rbuilder;
    std::unique_ptr<Json::CharReader> const rdr(rbuilder.newCharReader());
    Json::Value root;
    string errs;
    vector<vector<int>> outs;
    auto res{kSql.ExecuteQuery(sql)};
    while (res->next()) {
        string r{res->getString(1)};
        bool good = rdr->parse(r.data(), r.data()+r.length(), &root, &errs);
        if (!good) {
            cout << "Failed to read JSON\n";
            return;;
        }
        vector<int> row;
        auto sz = root.size();
        for (int i{0}; i<sz; ++i) {
            auto sz2{root[i].size()};
            for (int j{0}; j<sz2; ++j) {
                row.push_back(root[i][j].asInt());
            }
        }
        outs.push_back(row);
        cout << race << ":" << res->getString(1) << "\n";
    }
    delete res;
}

string vhist_replace(const string& table, const string& race, const string& dates)
{
    string s1{vhist_template};
    std::regex pat1{"<VHIST_RACE>"};
    std::regex pat2{"<VHIST_TABLE>"};
    std::regex pat3{"<VHIST_DATES>"};
    string s2{std::regex_replace(s1, pat1, race)};
    string s3{std::regex_replace(s2, pat2, table)};
    string s4{std::regex_replace(s3, pat3, dates)};
    return s4;
}

void vhist(const string& table, const string& race)
{
    string sql{vhist_replace(table, race, "\'[2017, 2016]\'")};
    cout << "sql = " << sql << "\n";
    ofstream sql_cmd_str("../sqls/vhist_cmd.sql");
    sql_cmd_str << sql << "\n";
    sql_cmd_str.close();
    drk::get_from_cmd("/usr/bin/mysql < ../sqls/vhist_cmd.sql > ../out/vhist_out.txt");
    string gplot = R"%%(/usr/bin/gnuplot --persist)%%";
    cout << "\n" << gplot << "\n";
    FILE* GNU;
    GNU = popen(gplot.c_str(), "w");
    if (GNU==nullptr) {
        std::cerr << "Error opening pipe to GNU plot!\n";
        exit(1);
    }
    string cmd = R"%%(
col = "Age"
stats '../out/vhist_out.txt' using col name "Aa" nooutput
mina = Aa_min
maxa = Aa_max
)%%";
    fprintf(GNU, "%s", cmd.c_str());

}
void dates()
{
//    string mysql{R"%%(/usr/bin/mysql -e ')%%"};
    string sqlf{R"%%(select timestampdiff(year, dob, '2017-11-08') as age from CD1_2018active where
	voterhistory->'$[*]."General Election"[*]' = json_array(2017, 2016))%%"};
    ofstream ofs{"../gnuplot/votehistory.sql"};
    ofs << sqlf << "\n";
    ofs.close();
    string s{"/usr/bin/mysql test < ../gnuplot/votehistory.sql > ../gnuplot/votehisdata 2> ../gnuplot/vothiserr"};
    FILE* MYSQL{popen(s.c_str(), "w")};
    pclose(MYSQL);

    sqlf = R"%%(select timestampdiff(year, dob, '2017-11-08') as age from CD1_2018active where
	voterhistory->'$[*]."General Election"[*]' = json_array(2017))%%";
    ofs.open("../gnuplot/votehistory.sql");
    ofs << sqlf << "\n";
    ofs.close();
    s = "/usr/bin/mysql test < ../gnuplot/votehistory.sql > ../gnuplot/votehisdata2 2> ../gnuplot/vothiserr2";
    MYSQL = popen(s.c_str(), "w");
    pclose(MYSQL);

    sqlf = R"%%(select timestampdiff(year, dob, '2017-11-08') as age from CD1_2018active where
	voterhistory is null)%%";
    ofs.open("../gnuplot/votehistory.sql");
    ofs << sqlf << "\n";
    ofs.close();
    s = "/usr/bin/mysql test < ../gnuplot/votehistory.sql > ../gnuplot/votehisdata3 2> ../gnuplot/vothiserr3";
    MYSQL = popen(s.c_str(), "w");
    pclose(MYSQL);

    sqlf = R"%%(select timestampdiff(year, dob, '2017-11-08') as age from CD1_2018active where
	voterhistory is not null)%%";
    ofs.open("../gnuplot/votehistory.sql");
    ofs << sqlf << "\n";
    ofs.close();
    s = "/usr/bin/mysql test < ../gnuplot/votehistory.sql > ../gnuplot/votehisdata4 2> ../gnuplot/vothiserr4";
    MYSQL = popen(s.c_str(), "w");
    pclose(MYSQL);

    string gplot = R"%%(/usr/bin/gnuplot --persist)%%";
    cout << "\n" << gplot << "\n";
    FILE* GNU;
    GNU = popen(gplot.c_str(), "w");
    if (GNU==nullptr) {
        std::cerr << "Error opening pipe to GNU plot!\n";
        exit(1);
    }
    string cmd = R"%%(
col = "age"
stats '../gnuplot/votehisdata4' using col nooutput
min = STATS_min
max = STATS_max
set boxwidth 1
set style fill pattern 4
#set style fill transparent solid 0.5
set tics out nomirror
set ylabel '# of voters'
set xlabel 'age in 2017'
set title 'CD1active'
#unset key
#set key left
set yrange [0:*]
set xrange [17.5:max+1]
plot '../gnuplot/votehisdata4' using (column(col)):(1) smooth frequency with boxes fill pattern 4 title 'voted', \
     '../gnuplot/votehisdata3' using (column(col)):(1) smooth frequency with boxes fill pattern 4 title 'never voted', \
     '../gnuplot/votehisdata' using (column(col)):(1) smooth frequency with boxes fill pattern 4 title 'voted 2016, 2017 only', \
     '../gnuplot/votehisdata2' using (column(col)):(1) smooth frequency with boxes fill pattern 4 title 'voted 2017 only'
pause -1 "Hit return to continue"
)%%";
    fprintf(GNU, "%s", cmd.c_str());
    pclose(GNU);
    string scrname{"../gnuplot/script_age.gnu"};
    ofstream scrs(scrname);
    if (!scrs) {
        std::cerr << "Failed to open " << cmd << "\n";
        return;
    }
    scrs << cmd;
    return;
}

