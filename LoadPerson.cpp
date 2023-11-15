#include <iostream>
#include <optional>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;

struct Person
{
    string name;
    int age;
};

enum class DBLogLevel
{
    HIGH,
    LOW
};

struct DataBaseHandler {
    DataBaseHandler& SetDataBaseName(string&& name) {
        db_name_ = move(name);
        return *this;
    }
    DataBaseHandler& SetTimeout(int timeout) {
        connetion_timeout_ = timeout;
        return *this;
    }
    DataBaseHandler& SetException(bool except) {
        allow_exceptions_ = except;
        return *this;
    }
    DataBaseHandler& SetLogLevel(DBLogLevel level) {
        log_level_ = level;
        return *this;
    }

    string db_name_ = "None";
    int connetion_timeout_ = 100;
    bool allow_exceptions_ = false;
    DBLogLevel log_level_ = DBLogLevel::LOW;
};

#define DECLARE_INT_PARAM(Name) \
    struct Name {               \
        int value;              \
        explicit Name(int v)    \
            : value(v) {        \
        }                       \
        operator int() const {  \
            return value;       \
        }                       \
    }

DECLARE_INT_PARAM(Min_age);
DECLARE_INT_PARAM(Max_age);

optional<vector<Person>> LoadPersons(DataBaseHandler& handler, int min_age, int max_age, string_view name_filter) {
    DBConnector connector(handler.allow_exceptions_, handler.log_level_);
    DBHandler db;
    if (handler.db_name_.starts_with("tmp."s)) {
        db = connector.ConnectTmp(handler.db_name_, handler.connetion_timeout_);
    }
    else {
        db = connector.Connect(handler.db_name_, handler.connetion_timeout_);
    }
    if (!handler.allow_exceptions_ && !db.IsOK()) {
        return nullopt;
    }

    ostringstream query_str;
    query_str << "from Persons "s
        << "select Name, Age "s
        << "where Age between "s << min_age << " and "s << max_age << " "s
        << "and Name like '%"s << db.Quote(name_filter) << "%'"s;
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({ move(name), age });
    }
    return persons;
}


int main() {
    DataBaseHandler handler;
    handler.SetDataBaseName("SimpleDb"s).SetTimeout(100).SetLogLevel(DBLogLevel::HIGH).SetException(true);
    auto result = LoadPersons({ DataBaseHandler().SetDataBaseName("SimpleDB").SetLogLevel(DBLogLevel::HIGH)
                                                .SetTimeout(100).SetException(true) },
                                Min_age(10),
                                Max_age(100),
              /*DB filter*/     "Ivanov");
    if (!result)
        cout << "DB isn't ok!";
    return 0;
}