#include <bits/stdc++.h>
#include "../ErrorHandler/exit.hpp";
#include "../heap.hpp";
using ll = long long;
using namespace std;

const ll LITERAL = -1;
const ll KPAIR = INT16_MAX;

const ll SEPERATOR = 0;
const ll MINUS = -1;
const ll PLUS = -2;
const ll MULTIPLY = -3;
const ll DIVIDE = -4;
const ll DIVIDE_INT = -5;
const ll DIVIDE_MOD = -6;
const ll HEAP_POINTER = -7;
const ll INTHEAP = -8;
const ll STRHEAP = -9;
const ll FLOATHEAP = -10;
const ll SIZECOUNTER = -11;

const ll INTEGER = 0;
const ll FLOAT = 1;
const ll STRING = 2;
const ll ALLTYPE = 3;

class Literal_Parsed {
public:
	u16string text;
	vector<ll> content;
	vector<Pointer> heap_pointer;
	bool function_identifier = false;
	bool is_parameter_set = false;
	Literal_Parsed(u16string token, TokenPosition position, stack<CallStack>& callstack) {
		text = token;
		for (ll i = 0; i < token.size(); i++) {
			if (token[i] == u'?') content.push_back(PLUS);
			else if (token[i] == u'!') content.push_back(MINUS);
			else if (token[i] == u'*') content.push_back(INTHEAP);
			else if (token[i] == u'~') content.push_back(STRHEAP);
			else if (token[i] == u'=') content.push_back(FLOATHEAP);
			else if (token[i] == u'&') content.push_back(SIZECOUNTER);
			else if (token[i] == u',') {
				is_parameter_set = true;
				content.push_back(SEPERATOR);
			}
			else if (token[i] == u'.') {
				if (content.size() == 0)
					ErrorCode(WRONG_MULTIPLY, { position.line, position.index + i }, callstack);
				if (content.back() == DIVIDE_INT)
					content.back() = DIVIDE_MOD;
				else if (content.back() == DIVIDE)
					content.back() = DIVIDE_INT;
				else if (content.back() == MULTIPLY)
					content.back() = DIVIDE;
				else content.push_back(MULTIPLY);
			}
			else if (token[i] == u'몰') content.push_back(1);
			else if (token[i] == u'모') {
				ll len = 2, t = i + 1;
				while (token[t] == u'오') len++, t++;
				if (token[t] != u'올') ErrorCode(WRONG_EXPRESSION, { position.line, position.index + i }, callstack);
				content.push_back(len);
				i = t;
			}
			else ErrorCode(WRONG_EXPRESSION, { position.line, position.index + i }, callstack);
		}

		for (ll i = 0; i < content.size(); i++) {
			if (i + 2 < content.size() && (FLOATHEAP == content[i + 1] || STRHEAP == content[i + 1] || INTHEAP == content[i + 1])) {
				if (content[i] <= 0 || content[i + 2] <= 0) ErrorCode(WRONG_EXPRESSION, position, callstack);
				heap_pointer.push_back(Pointer(content[i + 1], content[i], content[i + 2]));
				content[i] = HEAP_POINTER;
				content.erase(content.begin() + i + 1, content.begin() + i + 3);
			}
			else heap_pointer.push_back(Pointer());
		}
	}
	Literal_Parsed(u16string token) {
		text = token;
		function_identifier = true;
	}
};

class Tokenized {
public:
	vector<tuple<ll, ll, ll>> tokens;
	vector<TokenPosition> tokens_position;
	vector<Literal_Parsed> literals;
	vector<ll> gotopoint;
};

set<char16_t> literal_char = { u'몰', u'모', u'오', u'올', u'?', u'!', u'.', u',', u'*', u'~', u'=', u'&' };
vector<u16string> keywords = { u"루?", u"루!", u"루", u"0ㅅ0" };
vector<pair<u16string, u16string>> pair_keywords = {
	{u"아", u"루"}, {u"은?행", u"털!자"}, {u"은?행", u"돌!자"},
	{u"은?행", u"짓!자"}, {u"은?행", u"가!자"}, {u"가", u"자!"}
};

set<ll> front_parametered = { 0, 1, 2, KPAIR + 1, KPAIR + 2, KPAIR + 3, KPAIR + 4 };
set<ll> back_parametered = { 3, KPAIR + 3, KPAIR + 4 };
set<ll> back_function_parameter = { KPAIR + 3, KPAIR + 4 };
set<ll> non_number_parameter = { 0 };
set<ll> mid_number_param = { KPAIR, KPAIR + 5 };
set<ll> not_always_require_parameter = { 3, KPAIR + 3 };

ll literalToken(u16string& script, ll idx) {
	ll length = 0;
	for (ll i = idx; i < script.size(); i++) {
		if (literal_char.count(script[i])) length++;
		else break;
	}
	return length;
}

Tokenized tokenize(u16string script, stack<CallStack>& callstack) {
	Tokenized ret;
	ret.gotopoint.push_back(0);
	stack<pair<ll, set<pair<u16string, ll>>>> closePair;
	ll line = 0, idx = 0;
	for (ll i = 0; i < script.size(); i++, idx++) {
		wchar_t cur = script[i];
		if (cur == u'\n') {
			ret.gotopoint.push_back(ret.tokens.size());
			line++, idx = -1;
			continue;
		}
		if (cur == u' ') {
			continue;
		}
		if (literal_char.count(cur)) {
			ll len = literalToken(script, i);
			ret.tokens.push_back({ LITERAL, ret.literals.size(), -1 });
			ret.tokens_position.push_back({ line, idx });
			ret.literals.push_back(Literal_Parsed(script.substr(i, len), { line, idx }, callstack));
			i += len - 1, idx += len - 1;
			continue;
		}
		bool foundToken = false;
		if (closePair.size() > 0) {
			for (pair<u16string, ll> x : closePair.top().second) {
				if (script.substr(i, x.first.size()) == x.first) {
					foundToken = true;
					get<1>(ret.tokens[closePair.top().first]) = ret.tokens.size() - 1;
					get<0>(ret.tokens[closePair.top().first]) = x.second;
					i += x.first.size() - 1, idx += x.first.size() - 1;
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
				ret.tokens_position.push_back({ line, idx });
				i += keywords[t].size() - 1, idx += keywords[t].size() - 1;
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
				closePair.top().second.insert({ pair_keywords[t].second, KPAIR + t, });
				plused = pair_keywords[t].first.size() - 1;
			}
		}
		if (closePair.top().second.empty()) closePair.pop();
		if (foundToken) {
			i += plused, idx += plused;
			ret.tokens.push_back({ -1, -1, -1 });
			ret.tokens_position.push_back({ line, idx });
			continue;
		}

		if (i + 1 < script.size()) {
			if (u'마' <= script[i] && script[i] <= u'밓' && u'라' <= script[i + 1] && script[i + 1] <= u'맇') {
				foundToken = true;
				ret.tokens.push_back({ LITERAL, ret.literals.size(), -1 });
				ret.tokens_position.push_back({ line, idx });
				ret.literals.push_back(Literal_Parsed(script.substr(i, 2)));
				i++, idx++;
			}
		}
		if (foundToken) continue;

		ErrorCode(UNKNOWN_TOKEN, { line, idx }, callstack);
	}

	if (closePair.size() > 0) {
		ErrorCode(MISSING_PAIR, { line, 0 }, callstack);
	}

	return ret;
}