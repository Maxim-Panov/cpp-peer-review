#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <vector>

using namespace std;

class CheerSystem {
private:
	const static int NUM_USERS = 100'000;
	const static int NUM_PAGES = 1000;

	vector<int> user_prog_;
	vector<int> page_prog_;

public:
	CheerSystem() {
		user_prog_.resize(NUM_USERS + 1);
		page_prog_.resize(NUM_PAGES + 1);
	}

	void UpdateProgress(istream& input) {
		int user, pages;
		input >> user >> pages;
		input.get();

		int prev_pages = user_prog_[user] == 0 ? 0 : user_prog_[user];
		user_prog_[user] = pages;
		for (int i = prev_pages + 1; i <= pages; i++)
			page_prog_[i]++;
	}

	void CheerUser(istream& input, ostream& out) {
		int cheer_user;
		input >> cheer_user;
		input.get();
		//unknown user
		if (user_prog_[cheer_user] == 0) {
			out << 0 << endl;
			return;
		}
		double num_more = page_prog_[user_prog_[cheer_user]] - 1;
		//first reader
		if (num_more == 0) {
			out << 1 << endl;
			return;
		}
		double total_user = page_prog_[1] - 1;
		out << setprecision(6) << (total_user - num_more) / total_user << endl;

	}
};

void QueryProcess(istream& input, ostream& out) {
	CheerSystem system;
	long num_query;
	input >> num_query;
	input.get();
	string query_type;
	for (long i = 0; i < num_query; i++) {
		input >> query_type;
		if (query_type == "CHEER")
			system.CheerUser(input, out);
		else
			system.UpdateProgress(input);
	}
}

int main() {
	QueryProcess(cin, cout);
	return 0;
}