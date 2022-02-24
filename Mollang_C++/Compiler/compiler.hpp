#include <bits/stdc++.h>
#include "../Tokenizer/tokenizer.hpp";
using ll = long long;
using namespace std;

const ll MAINBLOCK = 0;

class Compiled {
public:
	vector<bool> literal_owned;
	vector<bool> no_calc;
	vector<bool> use_float;
	vector<ll> block_owned;
};

Compiled compile(Tokenized& data, stack<CallStack>& callstack, ll& block_counter) {
	Compiled ret = {
		vector<bool>(data.literals.size()),
		vector<bool>(data.literals.size()),
		vector<bool>(data.literals.size()),
		vector<ll>(data.gotopoint.size())
	};
	for (ll i = 0; i < data.literals.size(); i++) {
		for (auto& j : data.literals[i].content) {
			if (j == DIVIDE) {
				ret.use_float[i] = true;
				break;
			}
		}
		for (auto& j : data.literals[i].heap_pointer) {
			if (j.type == FLOATHEAP) {
				ret.use_float[i] = true;
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
					ErrorCode(MISSING_PARAMETER, data.tokens_position[i], callstack);
				continue;
			}
			if (get<0>(data.tokens[i + 1]) == LITERAL) {
				if (ret.literal_owned[get<1>(data.tokens[i + 1])])
					ErrorCode(MISSING_PARAMETER, data.tokens_position[i], callstack);
				ret.literal_owned[get<1>(data.tokens[i + 1])] = true;
				if (non_number_parameter.count(cur))
					ret.no_calc[get<1>(data.tokens[i + 1])] = true;
				if (back_function_parameter.count(cur))
					data.literals[get<1>(data.tokens[i + 1])].is_parameter_set = true;
			}
			else if (!not_always_require_parameter.count(cur))
				ErrorCode(MISSING_PARAMETER, data.tokens_position[i], callstack);
		}
		if (front_parametered.count(cur)) {
			if (i == 0) ErrorCode(MISSING_PARAMETER, data.tokens_position[i], callstack);
			if (get<0>(data.tokens[i - 1]) == LITERAL) {
				if (ret.literal_owned[get<1>(data.tokens[i - 1])])
					ErrorCode(MISSING_PARAMETER, data.tokens_position[i], callstack);
				ret.literal_owned[get<1>(data.tokens[i - 1])] = true;
				if (non_number_parameter.count(cur))
					ret.no_calc[get<1>(data.tokens[i - 1])] = true;
			}
			else ErrorCode(MISSING_PARAMETER, data.tokens_position[i], callstack);
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

	vector<ll> ownblock(data.tokens.size());
	ll inblock = block_counter, processing_line = 0, usedblock = block_counter;

	ll breakpoint = -1;
	for (ll i = 0; i < data.tokens.size(); i++) {
		if (get<0>(data.tokens[i]) == KPAIR + 3) {
			if (inblock != block_counter)
				ErrorCode(WRONG_FUNCTION_DEFINITION, data.tokens_position[i], callstack);
			inblock = ++usedblock, breakpoint = get<1>(data.tokens[i]);
		}
		if (i == breakpoint)
			inblock = block_counter, breakpoint = -1;
		ownblock[i] = inblock;
	}
	for (ll i = 0; i < data.gotopoint.size(); i++) {
		if (data.gotopoint[i] < data.tokens.size())
			ret.block_owned[i] = ownblock[data.gotopoint[i]];
		else ret.block_owned[i] = block_counter;
	}

	block_counter = usedblock + 1;

	return ret;
}

void show_compiled(Tokenized& tmp, Compiled& tmp2) {
	cout << "===================\n";
	cout << "TOKENS\n";
	for (ll i = 0; i < tmp.tokens.size(); i++) {
		cout << i << ": " << get<0>(tmp.tokens[i]) << ' '
			<< get<1>(tmp.tokens[i]) << ' ' << get<2>(tmp.tokens[i]) << '\n';
	}
	cout << "LITERALS\n";
	for (ll i = 0; i < tmp.literals.size(); i++) {
		cout << i << ": " << converter.to_bytes(tmp.literals[i].text) << ' ' << tmp2.literal_owned[i] << ' ' << tmp.literals[i].is_parameter_set << '\n';
	}
	cout << "===================\n";
}