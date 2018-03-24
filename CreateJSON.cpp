//
// 02/05/2018 kwoodle
// Insert into a database table with columns
// LASTNAME, SBOEID, VoterHistoryJSON
// with Foreign key linked to main table
//
//

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
    string database, table_in, table_out;
    string csv_file{"/var/tmp/ktrade/"};

//    ifstream ifstr("../voter.ini");
    ifstream ifstr("../create_json_test.ini");
    if (!ifstr.is_open()) {
        cout << "Failed to open config file\n";
        return 1;
    }

    po::options_description desc("Config");
    desc.add_options()
            ("files.csv_to_load", po::value<string>())
            ("mysql.service", po::value<string>())
            ("mysql.user", po::value<string>())
            ("mysql.password", po::value<string>())
            ("mysql.database", po::value<string>())
            ("mysql.table_in", po::value<string>())
            ("mysql.table_out", po::value<string>());

    po::variables_map vm;

    // set third parameter to true to allow unregistered options
    // in config file.
    store(parse_config_file(ifstr, desc, true), vm);
    notify(vm);

    csv_file += vm["files.csv_to_load"].as<string>();
    service = vm["mysql.service"].as<string>();
    user = vm["mysql.user"].as<string>();
    pass = vm["mysql.password"].as<string>();
    database = vm["mysql.database"].as<string>();
    table_in = vm["mysql.table_in"].as<string>();
    table_out = vm["mysql.table_out"].as<string>();

    ifstr.close();
    ofstream csv{csv_file};
    if (!csv.is_open()) {
        cout << "Failed to open csv file "<<csv_file<<endl;
        return 1;
    }

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

//        cmd = drop_vhist_json_test.c_str();
//        stmt->execute(cmd);

//        cmd = create_vhist_json_test.c_str();
//        stmt->execute(cmd);


        cmd = string{"select LASTNAME, SBOEID, VoterHistory from "}+table_in.c_str();
        auto result{stmt->executeQuery(cmd.c_str())};

        vector<tuple<string, string, string>> ins;
        while (result->next()) {
            ins.emplace_back(result->getString("LASTNAME"), result->getString("SBOEID"),
                    result->getString("VoterHistory"));
        }
        cout<<ins.size()<<" rows returned from "<<table_in<<endl;
        // General Election, 2017;General Election, 2016;Federal Primary, 2016;
        //
        // JSON [ ["General Election", 2017], ["General Election", 2016], ["Federal Primary", 2016] ]
        //

        const string comma_pat{"(.+?),(.+?)"};

//        const string pats{"(.+?);.*?"};
        const string pats{"(.+?),(.+?);.*?"};
        std::regex pat{pats};
        std::smatch matches;
        const string quote{"\""};
        const string ostart{"[["};
        const string split{"], ["};
        vector<tuple<string, string, string>> outs;
        for (auto e : ins) {
            string fnd{get<2>(e)};
            string v{ostart};
            while (regex_search(fnd, matches, pat)) {
                v += quote;
                v += matches[1];
                v += quote + ", ";
                v += matches[2];
                v += split;
                fnd = matches.suffix();
            }
            v += "]]";
            outs.emplace_back(get<0>(e), get<1>(e), v);
        }
        cout<<outs.size()<<" elements in out vector"<<endl;
/*
NY000000000009694946:<General Election, 2016><General Election, 2015><General Election, 2014><General Election, 2013><General Election, 2010><General Election, 2009><General Election, 2008><General Election, 2007><General Election, 2006><General Election, 2005><General Election, 2004><General Election, 2003>
NY000000000009694948:<General Election, 2017><General Election, 2016><Federal Primary, 2016><Presidential Primary, 2016><General Election, 2015><General Election, 2014><General Election, 2013><Special Election (1), 2013><General Election, 2011><General Election, 2010><Primary Election, 2010><Special Election (2), 2010><General Election, 2009><Primary Election, 2009><General Election, 2008><General Election, 2007><General Election, 2006><General Election, 2005><General Election, 2004><General Election, 2003>
NY000000000009694949:<General Election, 2017><General Election, 2016><General Election, 2015><General Election, 2014><General Election, 2013><General Election, 2012><General Election, 2011><General Election, 2010><General Election, 2009><General Election, 2008><General Election, 2004>
NY000000000009694950:<General Election, 2004>
NY000000000009694951:<General Election, 2017><General Election, 2016><Federal Primary, 2016><Presidential Primary, 2016><General Election, 2015><General Election, 2014><Primary Election, 2014><General Election, 2013><General Election, 2012><General Election, 2011><General Election, 2010><Primary Election, 2010><General Election, 2009><Primary Election, 2009><General Election, 2008><Presidential Primary, 2008><General Election, 2007><General Election, 2006><General Election, 2005><General Election, 2004><General Election, 2003>
 */
        for (auto row:outs) {
            string tmp{get<0>(row)};
            tmp += "!" +  get<1>(row) +"!" + get<2>(row);
            csv<<tmp<<'\n';
        }

/* first4.txt
ABEL!NY000000000009701159![[]]
ACHUTHAN!NY000000000009695440![["General Election",  2017], ["Primary Election",  2017], ["General Election",  2016], ["Federal Primary",  2016], ["Presidential Primary",  2016], ["General Election",  2015], ["General Election",  2014], ["General Election",  2013], ["General Election",  2012], ["General Election",  2010], ["General Election",  2008], ["Presidential Primary",  2008], ["General Election",  2006], ["General Election",  2004], []]
ACQUAVELLA!NY000000000009697601![[]]
ACQUINO!NY000000000009701523![["General Election",  2016], []]
*/

/*
 * wc -l 656715 suffolk.csv
 *      - 50,000 = ignore 606715 lines
*/
        string load_data{"load data local infile '"};
        load_data += csv_file + "\' ";
        load_data += "replace into table " + database + "." + table_out;
//        load_data += " fields terminated by \'!\';";
        load_data += " fields terminated by \'!\' ignore 606715 lines;";
        auto res = stmt->execute((load_data.c_str()));

        delete result;
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

/*
ZYTKA:NY000000000010311964:[["General Election",  2016], []]
ZYULKOVSKA:NY000000000054344443:[[]]
ZYWIEN:NY000000000010164756:[[]]
# ERR: SQLException in /home/kwoodle/CLionProjects/VoterData/CreateJSON.cpp(main) on line 164
# ERR: Duplicate entry 'NY000000000009984661' for key 'SBOEID' (MySQL error code: 1062, SQLState: 23000 )

 */