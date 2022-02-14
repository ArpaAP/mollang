#include <bits/stdc++.h>
#include "../Compiler/compiler.hpp";
using ll = long long;
using namespace std;

class ENV {
public:
	map<ll, ll> variables;
	stack<ll> runtimeStack;
};

ll calc(ENV& env, Literal_Parsed& x, ll idx, ll position) {
	if (x.content[0] == MULTIPLY) ErrorCode(WRONG_MULTIPLY, position);
	if (x.content.back() == MULTIPLY) ErrorCode(WRONG_MULTIPLY, position);

	ll ans = 1, cur = 0;
	for (ll i = idx; i < x.content.size(); i++) {
		if (x.content[i] == MULTIPLY)
			ans *= cur, cur = 0;
		else if (x.content[i] == PLUS) cur++;
		else if (x.content[i] == MINUS) cur--;
		else {
			if (env.variables.count(x.content[i]))
				cur += env.variables[x.content[i]];
			else ErrorCode(UNDEFINED_VARIABLE, position);
		}
	}
	ans *= cur;

	return ans;
}

void assign(ENV& env, Literal_Parsed& x, ll position) {
	if (x.content[0] <= 0) ErrorCode(WRONG_EXPRESSION, position);
	if (x.content.back() == MULTIPLY) ErrorCode(WRONG_EXPRESSION, position);

	if (x.content.size() == 1) {
		env.variables[x.content[0]] = 0;
		return;
	}

	if (env.variables.count(x.content[0])) {
		if (x.content[1] == MULTIPLY) {
			ll value = calc(env, x, 2, position);
			env.variables[x.content[0]] *= value;
		}
		else {
			ll value = calc(env, x, 1, position);
			env.variables[x.content[0]] += value;
		}
	}
	else {
		if (x.content[1] == MULTIPLY) ErrorCode(WRONG_MULTIPLY, position);
		ll value = calc(env, x, 1, position);
		env.variables[x.content[0]] = value;
	}
}

ll run(ENV& env, Tokenized& x, Compiled& y) {
	for (ll i = 0; i < x.tokens.size(); i++) {
		ll f = get<0>(x.tokens[i]), s = get<1>(x.tokens[i]), t = get<2>(x.tokens[i]);
		if (f == LITERAL) {
			if (y.no_calc[s]) continue;
			if (y.literal_owned[s])
				env.runtimeStack.push(calc(env, x.literals[s], 0, x.tokens_position[i]));
			else assign(env, x.literals[s], x.tokens_position[i]);
		}
		else if (f >= PAIR_KEYWORD) {
			if (s == i) ErrorCode(MISSING_MID_PARAMETER, x.tokens_position[i]);
			if (f == PAIR_KEYWORD + 1) { //은?행 털!자
				if (env.runtimeStack.top() != 0)
					i = s;
				env.runtimeStack.pop();
			}
			else {
				if (s > i + 1) ErrorCode(WRONG_PARAMETER, x.tokens_position[i]);
				if (get<0>(x.tokens[i + 1]) != LITERAL) ErrorCode(WRONG_PARAMETER, x.tokens_position[i]);

				ll midparam = calc(env, x.literals[get<1>(x.tokens[i + 1])], 0, x.tokens_position[i]);

				if (f == PAIR_KEYWORD + 0) { //아루
					wcout << (wchar_t)midparam;
				}
				else if (f == PAIR_KEYWORD + 2) { //가자!
					i = x.gotopoint[midparam - 1] - 1;
				}
			}
		}
		else {
			if (f == 0) { //루?
				if (i == 0)
					ErrorCode(MISSING_PARAMETER, x.tokens_position[i]);
				if (x.literals[get<1>(x.tokens[i - 1])].content.size() >= 2)
					ErrorCode(WRONG_PARAMETER, x.tokens_position[i]);
				if (x.literals[get<1>(x.tokens[i - 1])].content[0] <= 0)
					ErrorCode(WRONG_PARAMETER, x.tokens_position[i]);
				cin >> env.variables[x.literals[get<1>(x.tokens[i - 1])].content[0]];
			}
			if (f == 1) { //루
				cout << env.runtimeStack.top();
				env.runtimeStack.pop();
			}
			if (f == 2) { //0ㅅ0
				if (i + 1 < x.tokens.size() && get<0>(x.tokens[i + 1]) == LITERAL)
					return calc(env, x.literals[get<1>(x.tokens[i + 1])], 0, x.tokens_position[i]);
				else return 0;
			}
		}
	}

	return 0;
}