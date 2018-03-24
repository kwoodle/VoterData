// CreateCD1.cpp

// Created by kwoodle on 2/8/18.
//
// Read the CD1...csv file, modify to produce date and json types
// Create new file with fields separated by '!' to load into database
// table using load data local infile.
// !! Must change sql_mode for mysqld or you can't load data local infile
// with NULL dates.

/*
Edit /etc/my.cnf, remove NO_ZERO_DATE
# from select @@global.sql_mode
#sql_mode=ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION
# Enable insertion of NULL dates
sql_mode=ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION
 */

#include "Voter.h"
#include <fstream>
#include <exception>
#include <regex>
#include <boost/program_options.hpp>
#include <sstream>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;
namespace po = boost::program_options;

int main()
{
    // Get parameters from config file
    string service, user, pass;
    string database, source_file, table_out;
    string bangsv_file{"/var/tmp/ktrade/"};

//    ifstream cfg("../voter.ini");
    ifstream cfg("../voter_test.ini");
    if (!cfg.is_open()) {
        cout << "Failed to open config file\n";
        return 1;
    }

    po::options_description desc("Config");
    desc.add_options()
            ("files.source_file", po::value<string>())
            ("files.bangsv_to_load", po::value<string>())
            ("mysql.service", po::value<string>())
            ("mysql.user", po::value<string>())
            ("mysql.password", po::value<string>())
            ("mysql.database", po::value<string>())
            ("mysql.table_out", po::value<string>());

    po::variables_map vm;

    // set third parameter to true to allow unregistered options
    // in config file.
    store(parse_config_file(cfg, desc, true), vm);
    notify(vm);

    source_file = vm["files.source_file"].as<string>();
    bangsv_file += vm["files.bangsv_to_load"].as<string>();
    service = vm["mysql.service"].as<string>();
    user = vm["mysql.user"].as<string>();
    pass = vm["mysql.password"].as<string>();
    database = vm["mysql.database"].as<string>();
    table_out = vm["mysql.table_out"].as<string>();

    cfg.close();

    // source csv file, e.g. data from Board of Elections
    ifstream infile(source_file);
    if (!infile.is_open()) {
        cout << "Failed to open source file " << source_file << endl;
        return 1;
    }
    Json::CharReaderBuilder rbuilder;
    JRdrPtr const rdr(rbuilder.newCharReader());

    // Voter records all have 45 fields, separated by commas and enclosed
    // in double-quotes. There are commas inside the quotes. VoterHistory
    // is a list separated by semicolons of elements containing brackets and
    // commas. "field","field2",...,"field45" ... regex ^("(.?)",){44}"(.?)"$

    vector<vector<string>> pre_outs;
    const string regxstr{R"%%("(.*?)"[,\r])%%"};    // the original file has /r/n!!
    regex pat{regxstr};
    cout<<"rexstr = "<<regxstr<<endl;
    smatch m;
    string line;
    while(getline(infile, line)) {
        vector<string> f;
        string fnd{line};
        while (regex_search(fnd, m, pat)) {
            f.push_back(m[1]);
            fnd = m.suffix();
        }
        //format the dates
        for (auto ind:idates) {
            format_date(f.at(ind));
        }
        pre_outs.push_back(f);
    }
    infile.close();

/*    for (auto v:pre_outs) {
        for (auto e:v) {
            cout<<"<"<<e<<">";
        }
        cout<<endl;
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

    cout<<"pats = "<<pats<<endl;
    vector<string> outs;
    for (auto v:pre_outs) {
        string oline;
        for (int i=0;i<v.size()-1;++i) {
            oline +=v[i]+ "!";
        }
        string json{ostart};
        string fnd = v.at(v.size()-1);
        while (regex_search(fnd, m, pat)) {
            json += quote;
            json += m[1];
            json += quote + ", ";
            json += m[2];
            json += split;
            fnd = m.suffix();
            continue;
        }
        if (regex_match(fnd, m, pat2)) {
            json += quote;
            json += m[1];
            json += quote + ", ";
            json += m[2];
            json += split;
        }
        json += "]]";
        string err;
        if(not_valid_json(rdr, json, err)) {
            cout<<"json errors "<<err<<endl;
            json = "[[\"Invalid JSON\", 2018]]";
        }
        oline += json;
        outs.push_back(oline);
    }
    ofstream csv{bangsv_file};
    if (!csv.is_open()) {
        cout << "Failed to open csv file " << bangsv_file << endl;
        return 1;
    }

    for(auto l:outs) {
//        cout<<l<<endl;
        csv<<l<<"\n";
    }

    csv.close();

    try {

        // from https://dev.mysql.com/doc/connector-cpp/en/connector-cpp-examples-complete-example-2.html
        // connect to server
        //
        auto con{get_driver_instance()->connect("localhost", user, pass)};
        //
        // select the database
        con->setSchema(service);

        auto stmt{con->createStatement()};
        //
        // select database
        string cmd{"use "+database+";"};
        stmt->execute(cmd.c_str());
        cmd = "truncate table " + table_out;

        string load_data{"load data local infile '"};
        load_data += bangsv_file + "\' ";
        load_data += "replace into table " + table_out;
        load_data += " fields terminated by '!'";

        auto res = stmt->execute((load_data.c_str()));

//        delete result;
        delete stmt;
        delete con;

    }
    catch (sql::SQLException& e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line "
             << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() <<
             " )" << endl;
    }
    return 0;
}
