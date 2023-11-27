#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;

class Domain {
public:
    Domain(string name) : name_(name) {
        reverse(name_.begin(), name_.end());
    }

    bool operator==(const Domain& rhs) const {
        return name_ == rhs.name_ ? true : false;
    }

    bool IsSubDomain(Domain& other) const {
        size_t pos = other.name_.find(name_);
        if (pos)
            return false;
        else if (name_ == other.name_)
            return true;
        else return lexicographical_compare(name_.begin(), name_.end(),
            other.name_.begin(), other.name_.end());
    }

    const string GetName() const {
        return name_;
    }

    bool operator<(const Domain& rhs) const {
        return name_ < rhs.name_;
    }

private:
    string name_;
};

class DomainChecker {
public:
    template<typename Iter>
    DomainChecker(Iter begin, Iter end) {
        domain_collection_.reserve(distance(begin, end));
        while (begin != end) {
            domain_collection_.push_back(*begin);
            begin++;
        }
        sort(domain_collection_.begin(), domain_collection_.end());
        auto last = unique(domain_collection_.begin(), domain_collection_.end(), [](Domain& lhs, Domain& rhs) {
            return rhs.GetName().find(lhs.GetName()) != string::npos ? true : false;
            });
        domain_collection_.erase(last, domain_collection_.end());
    }

    bool IsForbidden(Domain domain) const {
        auto upper_pos = upper_bound(domain_collection_.begin(),
            domain_collection_.end(), domain);
        if (upper_pos == domain_collection_.begin())
            return false;
        else return (upper_pos - 1)->IsSubDomain(domain);
    }

private:
    vector<Domain> domain_collection_;
};

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num{};
    std::istringstream(line) >> num;

    return num;
}

template <typename Funct>
vector<Domain> ReadDomains(istream& input, const Funct get_number) {
    vector<Domain> results;
    size_t num_domains = get_number;
    results.reserve(num_domains);
    for (size_t i = 0; i < num_domains; i++) {
        string domain;
        getline(input, domain);
        domain = '.' + domain + '.';
        results.push_back(move(domain));
    }
    return results;
}


int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}