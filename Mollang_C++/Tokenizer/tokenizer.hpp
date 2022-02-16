#include <bits/stdc++.h>
#include "../ErrorHandler/exit.hpp";
using ll = long long;
using namespace std;

const ll LITERAL = -1;
const ll PAIR_KEYWORD = INT16_MAX;

const ll MINUS = -1;
const ll PLUS = -2;
const ll MULTIPLY = -3;
const ll DIVIDE = -4;
const ll DIVIDE_INT = -5;
const ll DIVIDE_MOD = -6;

const ll INTEGER = 0;
const ll FLOAT = 1;
const ll STRING = 2;
const ll ALLTYPE = 3;

class Literal_Parsed {
public:
	wstring text;
	vector<ll> content;
	Literal_Parsed(wstring token, ll position) {
		text = token;
		for (ll i = 0; i < token.size(); i++) {
			if (token[i] == L'?') content.push_back(PLUS);
			else if (token[i] == L'!') content.push_back(MINUS);
			else if (token[i] == L'.') {
				if (content.size() == 0)
					ErrorCode(WRONG_MULTIPLY, position);
				if (content.back() == DIVIDE_INT)
					content.back() = DIVIDE_MOD;
				else if (content.back() == DIVIDE)
					content.back() = DIVIDE_INT;
				else if (content.back() == MULTIPLY)
					content.back() = DIVIDE;
				else content.push_back(MULTIPLY);
			}
			else if (token[i] == L'몰') content.push_back(1);
			else if (token[i] == L'모') {
				ll len = 2, t = i + 1;
				while (token[t] == L'오') len++, t++;
				if (token[t] != L'올') ErrorCode(WRONG_EXPRESSION, position);
				content.push_back(len);
				i = t;
			}
			else ErrorCode(WRONG_EXPRESSION, position);
		}
	}
};

class Tokenized {
public:
	vector<tuple<ll, ll, ll>> tokens;
	vector<ll> tokens_position;
	vector<Literal_Parsed> literals;
	vector<ll> gotopoint;
};

set<wchar_t> literal_char = { L'몰', L'모', L'오', L'올', L'?', L'!', L'.' };
vector<wstring> keywords = { L"루?", L"루!", L"루", L"0ㅅ0" };
vector<pair<wstring, wstring>> pair_keywords = { {L"아", L"루"}, {L"은?행", L"털!자"}, {L"은?행", L"돌!자"}, {L"가", L"자!"} };

set<ll> front_parametered = { 0, 1, 2, PAIR_KEYWORD + 1, PAIR_KEYWORD + 2 };
set<ll> back_parametered = { 3 };
set<ll> non_number_parameter = { 0 };
set<ll> all_type_parameter = { 2 };
set<ll> mid_number_param = { PAIR_KEYWORD, PAIR_KEYWORD + 3 };
set<ll> not_always_require_parameter = { 3 };

ll literalToken(wstring& script, ll idx) {
	ll length = 0;
	for (ll i = idx; i < script.size(); i++) {
		if (literal_char.count(script[i])) length++;
		else break;
	}
	return length;
}

Tokenized tokenize(wstring script) {
	Tokenized ret;
	ret.gotopoint.push_back(0);
	stack<pair<ll, set<pair<wstring, ll>>>> closePair;
	ll line = 0;
	for (ll i = 0; i < script.size(); i++) {
		wchar_t cur = script[i];
		if (cur == L'\n') {
			ret.gotopoint.push_back(ret.tokens.size());
			line++;
			continue;
		}
		if (cur == ' ') continue;
		if (literal_char.count(cur)) {
			ll len = literalToken(script, i);
			ret.tokens.push_back({ LITERAL, ret.literals.size(), -1 });
			ret.tokens_position.push_back(line);
			ret.literals.push_back(Literal_Parsed(script.substr(i, len), line));
			i += len - 1;
			continue;
		}
		bool foundToken = false;
		if (closePair.size() > 0) {
			for (pair<wstring, ll> x : closePair.top().second) {
				if (script.substr(i, x.first.size()) == x.first) {
					foundToken = true;
					get<1>(ret.tokens[closePair.top().first]) = ret.tokens.size() - 1;
					get<0>(ret.tokens[closePair.top().first]) = x.second;
					i += x.first.size() - 1;
					closePair.pop();
					break;
				}
			}
			if (foundToken) continue;
		}
		for (ll t = 0; t < keywords.size(); t++) {
			if (script.substr(i, keywords[t].size()) == keywords[t]) {
				foundToken = true;
				ret.tokens.push_back({ t, -1, -1 });
				ret.tokens_position.push_back(line);
				i += keywords[t].size() - 1;
				break;
			}
		}
		if (foundToken) continue;
		closePair.push({});
		closePair.top().first = ret.tokens.size();
		ll plused = 0;
		for (ll t = 0; t < pair_keywords.size(); t++) {
			if (script.substr(i, pair_keywords[t].first.size()) == pair_keywords[t].first) {
				foundToken = true;
				closePair.top().second.insert({ pair_keywords[t].second, PAIR_KEYWORD + t, });
				plused = pair_keywords[t].first.size() - 1;
			}
		}
		if (closePair.top().second.empty()) closePair.pop();
		if (foundToken) {
			i += plused;
			ret.tokens.push_back({ -1, -1, -1 });
			ret.tokens_position.push_back(line);
			continue;
		}

		ErrorCode(UNKNOWN_TOKEN, line);
	}

	if (closePair.size() > 0) {
		ErrorCode(MISSING_PAIR, line);
	}

	return ret;
}