#include <bits/stdc++.h>
#include "../Tokenizer/tokenizer.hpp";
using ll = long long;
using namespace std;

class Compiled {
public:
	vector<bool> literal_owned;
};

Compiled compile(Tokenized x) {
	Compiled ret = { vector<bool>(x.literals.size()) };
	for (ll i = 0; i < x.tokens.size(); i++) {
		ll cur = get<0>(x.tokens[i]);
		if (cur == -1) continue;
		if (nonNumberParam.count(cur)) continue;

		if (back_parametered.count(cur)) {
			if (i == x.tokens.size() - 1) {
				if (!not_always_require_parameter.count(cur))
					ErrorCode(MISSING_PARAMETER);
				continue;
			}
			if (get<0>(x.tokens[i + 1]) == LITERAL) {
				if (ret.literal_owned[get<1>(x.tokens[i + 1])])
					ErrorCode(MISSING_PARAMETER);
				ret.literal_owned[get<1>(x.tokens[i + 1])] = true;
			}
			else if (!not_always_require_parameter.count(cur))
				ErrorCode(MISSING_PARAMETER);
		}
		if (front_parametered.count(cur)) {
			if (i == 0) ErrorCode(MISSING_PARAMETER);
			if (get<0>(x.tokens[i - 1]) == LITERAL) {
				if (ret.literal_owned[get<1>(x.tokens[i - 1])])
					ErrorCode(MISSING_PARAMETER);
				ret.literal_owned[get<1>(x.tokens[i - 1])] = true;
			}
			else ErrorCode(MISSING_PARAMETER);
		}
		if (cur >= PAIR_KEYWORD) {
			ll t = i + 1;
			while (t <= get<1>(x.tokens[i])) {
				if (get<0>(x.tokens[t]) == LITERAL)
					ret.literal_owned[get<1>(x.tokens[t])] = true;
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
		wcout << i << ": " << tmp.literals[i].text << ' ' << tmp2.literal_owned[i] << '\n';
	}
	wcout << "===================\n";
}