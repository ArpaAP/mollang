#include <bits/stdc++.h>
#include "../ErrorHandler/exit.hpp";
using ll = long long;
using namespace std;

const ll LITERAL = -1;
const ll PAIR_KEYWORD = INT16_MAX;

const ll PLUS = -1;
const ll MINUS = -2;
const ll MULTIPLY = 0;

class Literal_Parsed {
public:
	wstring text;
	vector<ll> content;
	Literal_Parsed(wstring token) {
		text = token;
		for (ll i = 0; i < token.size(); i++) {
			if (token[i] == L'?') content.push_back(PLUS);
			else if (token[i] == L'!') content.push_back(MINUS);
			else if (token[i] == L'.') content.push_back(MULTIPLY);
			else if (token[i] == L'��') content.push_back(1);
			else if (token[i] == L'��') {
				ll len = 2, t = i + 1;
				while (token[t] == L'��') len++, t++;
				if (token[t] != L'��') ErrorCode(WRONG_EXPRESSION);
				content.push_back(len);
				i = t;
			}
			else ErrorCode(WRONG_EXPRESSION);
		}
	}
};

class Tokenized {
public:
	vector<tuple<ll, ll, ll>> tokens;
	vector<Literal_Parsed> literals;
	vector<ll> gotopoint;
};

set<wchar_t> literal_char = { L'��', L'��', L'��', L'��', L'?', L'!', L'.' };
vector<wstring> keywords = { L"��?", L"��", L"0��0"};
vector<pair<wstring, wstring>> pair_keywords = { {L"��", L"��"}, {L"��?��", L"��!��"}, {L"��", L"��!"} };

set<ll> front_parametered = { 0, 1, PAIR_KEYWORD + 1 };
set<ll> back_parametered = { 2 };

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
	pair<ll, wstring> closePair = { -1, L"" };
	for (ll i = 0; i < script.size(); i++) {
		wchar_t cur = script[i];
		if (cur == L'\n') {
			ret.gotopoint.push_back(ret.tokens.size());
			continue;
		}
		if (cur == ' ') continue;
		if (literal_char.count(cur)) {
			ll len = literalToken(script, i);
			ret.tokens.push_back({ LITERAL, ret.literals.size(), -1 });
			ret.literals.push_back(Literal_Parsed(script.substr(i, len)));
			i += len - 1;
			continue;
		}
		if (closePair.first != -1) {
			if (script.substr(i, closePair.second.size()) == closePair.second) {
				get<1>(ret.tokens[closePair.first]) = ret.tokens.size() - 1;
				i += closePair.second.size() - 1;
				closePair = { -1, L"" };
				continue;
			}
		}
		bool foundToken = false;
		for (ll t = 0; t < keywords.size(); t++) {
			if (script.substr(i, keywords[t].size()) == keywords[t]) {
				foundToken = true;
				ret.tokens.push_back({ t, -1, -1 });
				i += keywords[t].size() - 1;
				break;
			}
		}
		if (foundToken) continue;
		for (ll t = 0; t < pair_keywords.size(); t++) {
			if (script.substr(i, pair_keywords[t].first.size()) == pair_keywords[t].first) {
				foundToken = true;
				closePair = { ret.tokens.size(), pair_keywords[t].second };
				ret.tokens.push_back({ PAIR_KEYWORD + t, -1, -1 });
				i += pair_keywords[t].first.size() - 1;
				break;
			}
		}
		if (foundToken) continue;

		ErrorCode(UNKNOWN_TOKEN);
	}

	if (closePair.first != -1) {
		ErrorCode(MISSING_PAIR);
	}

	return ret;
}