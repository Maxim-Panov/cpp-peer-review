#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

using namespace std;

struct City
{
    string city_name;
};

struct Json
{
    string header;
};

enum Language
{
    EN,
    FR,
    RU
};

class City {
public:
    City& SetName(string name) {
        city_name_ = move(name);
        return *this;
    }
    City& SetISO(string iso) {
        iso_code_ = move(iso);
        return *this;
    }
    City& SetPhoneCode(string phone) {
        phone_code_ = move(phone);
        return *this;
    }
    string& GetName() {
        return city_name_;
    }
    string& GetISO() {
        return iso_code_;
    }
    string& GetPhoneCode() {
        return phone_code_;
    }

private:
    string city_name_;
    string iso_code_ = 0;
    string phone_code_ = 0;
};

class Country {
public:
    Country& SetName(string name) {
        name_ = move(name);
        return *this;
    }
    Country& SetISO(string iso) {
        iso_code_ = move(iso);
        return *this;
    }
    Country& SetPhoneCode(string phone) {
        phone_code_ = move(phone);
        return *this;
    }
    Country& SetTimeZone(string zone) {
        time_zone_ = move(zone);
        return *this;
    }
    Country& AddLanguage(Language language) {
        languages_.push_back(move(language));
    }

    /*
    * ......Все геттеры
    */

    void ParseCitySubjson(const Json& json) {
        for (const auto& city_json : json.AsList())
            cities_.push_back(City().SetName(city_json["name"s].AsString())
                .SetISO(city_json["iso_code"s].AsString())
                .SetPhoneCode(city_json["phone_code"s].AsString())
            );
    }

private:
    string name_;
    string iso_code_ = 0;
    string phone_code_ = 0;
    string time_zone_ = 0;
    vector<City> cities_;
    vector<Language> languages_;
};

vector<Country> ParseCountryJson(const Json& json) {
    vector<Country> result;
    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();
        result.push_back(Country().SetName(country_obj["name"s].AsString())
            .SetISO(country_obj["iso_code"s].AsString())
            .SetPhoneCode(country_obj["phone_code"s].AsString())
            .SetTimeZone(country_obj["time_zone"s].AsString())
        );
        Country& country = result.back();
        for (const auto& lang_obj : country_obj["languages"s].AsList())
            country.AddLanguage(FromString<Language>(lang_obj.AsString()));
        country.ParseCitySubjson(country_obj["cities"s]);
    }
    return result;
}

int main() {

    return 0;
}