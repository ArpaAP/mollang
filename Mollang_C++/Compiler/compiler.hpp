#include <bits/stdc++.h>
#include "../Tokenizer/tokenizer.hpp";
using ll = long long;
using namespace std;

class Compiled {
public:
	vector<bool> literal_owned;
	vector<bool> no_calc;
	vector<pair<ll, bool>> type; //result type, process using float
};

Compiled compile(Tokenized& data) {
	Compiled ret = {
		vector<bool>(data.literals.size()),
		vector<bool>(data.literals.size()),
		vector<pair<ll, bool>>(data.literals.size())
	};
	for (ll i = 0; i < data.literals.size(); i++) {
		for (auto j : data.literals[i].content) {
			if (j == DIVIDE) {
				ret.type[i].second = true;
				break;
			}
		}
	}
	for (ll i = 0; i < data.tokens.size(); i++) {
		ll cur = get<0>(data.tokens[i]);
		if (cur == -1) continue;

		if (back_parametered.count(cur)) {
			if (i == data.tokens.size() - 1) {
				if (!not_always_require_parameter.count(cur))
					ErrorCode(MISSING_PARAMETER, data.tokens_position[i]);
				continue;
			}
			if (get<0>(data.tokens[i + 1]) == LITERAL) {
				if (ret.literal_owned[get<1>(data.tokens[i + 1])])
					ErrorCode(MISSING_PARAMETER, data.tokens_position[i]);
				ret.literal_owned[get<1>(data.tokens[i + 1])] = true;
				if (non_number_parameter.count(cur))
					ret.no_calc[get<1>(data.tokens[i + 1])] = true;
				if (all_type_parameter.count(cur))
					ret.type[get<1>(data.tokens[i + 1])].first = ALLTYPE;
				if (back_function_parameter.count(cur))
					data.literals[get<1>(data.tokens[i + 1])].is_parameter_set = true;
			}
			else if (!not_always_require_parameter.count(cur))
				ErrorCode(MISSING_PARAMETER, data.tokens_position[i]);
		}
		if (front_parametered.count(cur)) {
			if (i == 0) ErrorCode(MISSING_PARAMETER, data.tokens_position[i]);
			if (get<0>(data.tokens[i - 1]) == LITERAL) {
				if (ret.literal_owned[get<1>(data.tokens[i - 1])])
					ErrorCode(MISSING_PARAMETER, data.tokens_position[i]);
				ret.literal_owned[get<1>(data.tokens[i - 1])] = true;
				if(non_number_parameter.count(cur))
					ret.no_calc[get<1>(data.tokens[i - 1])] = true;
				if (all_type_parameter.count(cur))
					ret.type[get<1>(data.tokens[i - 1])].first = ALLTYPE;
			}
			else ErrorCode(MISSING_PARAMETER, data.tokens_position[i]);
		}
		if (mid_number_param.count(cur)) {
			ll t = i + 1;
			while (t <= get<1>(data.tokens[i])) {
				if (get<0>(data.tokens[t]) == LITERAL)
					ret.literal_owned[get<1>(data.tokens[t])] = true;
				t++;
			}
			i = t - 1;
		}
	}

	return ret;
}

void show_compiled(Tokenized& tmp, Compiled& tmp2) {
	wcout << "===================\n";
	wcout << "TOKENS\n";
	for (ll i = 0; i < tmp.tokens.size(); i++) {
		wcout << i << ": " << get<0>(tmp.tokens[i]) << ' '
			<< get<1>(tmp.tokens[i]) << ' ' << get<2>(tmp.tokens[i]) << '\n';
	}
	wcout << "LITERALS\n";
	for (ll i = 0; i < tmp.literals.size(); i++) {
		wcout << i << ": " << tmp.literals[i].text << ' ' << tmp2.literal_owned[i] << ' ' << tmp.literals[i].is_parameter_set << '\n';
	}
	wcout << "===================\n";
}