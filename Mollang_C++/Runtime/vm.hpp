#include <bits/stdc++.h>
#include "../Compiler/compiler.hpp";
using ll = long long;
using namespace std; 

class ENV {
public:
	map<ll, ll> variables;
	stack<ll> runtimeStack;
};

ll calc(ENV& env, Literal_Parsed& x, ll idx) {
	if (x.content[0] == MULTIPLY) ErrorCode(WRONG_MULTIPLY);
	if (x.content.back() == MULTIPLY) ErrorCode(WRONG_MULTIPLY);

	ll ans = 1, cur = 0;
	for (ll i = idx; i < x.content.size(); i++) {
		if (x.content[i] == MULTIPLY)
			ans *= cur, cur = 0;
		else if (x.content[i] == PLUS) cur++;
		else if (x.content[i] == MINUS) cur--;
		else {
			if (env.variables.count(x.content[i]))
				cur += env.variables[x.content[i]];
			else ErrorCode(UNDEFINED_VARIABLE);
		}
	}
	ans *= cur;

	return ans;
}

void assign(ENV& env, Literal_Parsed& x) {
	if (x.content[0] <= 0) ErrorCode(WRONG_EXPRESSION);
	if (x.content.back() == MULTIPLY) ErrorCode(WRONG_EXPRESSION);
	
	if (x.content.size() == 1) {
		env.variables[x.content[0]] = 0;
		return;
	}

	if (env.variables.count(x.content[0])) {
		if (x.content[1] == MULTIPLY) {
			ll value = calc(env, x, 2);
			env.variables[x.content[0]] *= value;
		}
		else {
			ll value = calc(env, x, 1);
			env.variables[x.content[0]] += value;
		}
	}
	else {
		if (x.content[1] == MULTIPLY) ErrorCode(WRONG_MULTIPLY);
		ll value = calc(env, x, 1);
		env.variables[x.content[0]] = value;
	}
}

ll run(ENV& env, Tokenized& x, Compiled& y) {
	for (ll i = 0; i < x.tokens.size(); i++) {
		ll f = get<0>(x.tokens[i]), s = get<1>(x.tokens[i]), t = get<2>(x.tokens[i]);
		if (f == LITERAL) {
			if (y.literal_owned[s]) 
				env.runtimeStack.push(calc(env, x.literals[s], 0));
			else assign(env, x.literals[s]);
		}
		if (f >= PAIR_KEYWORD) {
			if (s == i) ErrorCode(MISSING_MID_PARAMETER);
			if (f == PAIR_KEYWORD + 1) { //은?행 털!자
				if (env.runtimeStack.top() != 0)
					i = s;
				env.runtimeStack.pop();
			}
			else { 
				if (s > i + 1) ErrorCode(WRONG_PARAMETER);
				if (get<0>(x.tokens[i + 1]) != LITERAL) ErrorCode(WRONG_PARAMETER);

				ll midparam = calc(env, x.literals[get<1>(x.tokens[i + 1])], 0);

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
				ll tmp; cin >> tmp;
				if (x.literals[get<1>(x.tokens[i - 1])].content.size() >= 2)
					ErrorCode(WRONG_PARAMETER);
				if (x.literals[get<1>(x.tokens[i - 1])].content[0] <= 0) 
					ErrorCode(WRONG_PARAMETER);
				env.variables[x.literals[get<1>(x.tokens[i - 1])].content[0]] = tmp;
			}
			if (f == 1) { //루
				cout << env.runtimeStack.top();
				env.runtimeStack.pop();
			}
			if (f == 2) { //0ㅅ0
				return calc(env, x.literals[get<1>(x.tokens[i + 1])], 0);
			}
		}
	}

	return 0;
}