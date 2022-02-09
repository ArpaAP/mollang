#include <vector>
#include <string>
#include <tuple>
#include <vector>
#include "exit.hpp"
using namespace std;
using ll = long long;

struct Parsed {
	vector<tuple<ll, ll, ll>> tokens;
	vector<ll> line_backpoint;
};

//1: Int Literal
//2: Variable with Initialize
//3: Print
//4: Input
//5: PrintMode :Character
//6: Goto Start
//7: Goto End
//8: Solo Variable Token
//9: If Start
//10: Exit Code

pair<ll, ll> parseLiteral(wstring& script, ll i) {
	vector<ll> ends;
	ll cur = 0;
	for (; i < script.size(); i++) {
		if (script[i] == '?') cur++;
		else if (script[i] == '!') cur--;
		else if (script[i] == '.') {
			ends.push_back(cur);
			cur = 0;
		}
		else break;
	}
	if (cur != 0) ends.push_back(cur);
	ll res = ends[0];
	for (ll i = 1; i < ends.size(); i++)
		res *= ends[i];
	return { res, i };
}

Parsed parse(wstring script) {
	ll tokenType = -1;
	Parsed result;
	ll line_number = 0;
	stack<ll> if_jumppoint;
	result.line_backpoint.push_back(0);
	for (ll i = 0; i < script.size(); i++) {
		if (script[i] == '?' || script[i] == '!') {
			auto ret = parseLiteral(script, i);
			result.tokens.push_back({ 1, ret.first, -1 });
			i = ret.second;
			i--;
		}
		else if (script[i] == '.') {
			exitMessage(1, ".", { line_number, i });
		}
		else if (script[i] == L'¸ô' || script[i] == L'¸ð') {
			ll length = 1;
			while (i < script.size() && !(script[i] == L'¸ô' || script[i] == L'¿Ã'))
				i++, length++;
			i++;
			ll init = 0;
			if (i < script.size() && (script[i] == '?' || script[i] == '!')) {
				auto ret = parseLiteral(script, i);
				init = ret.first;
				i = ret.second - 1;
				result.tokens.push_back({ 2, length, init });
			}
			else result.tokens.push_back({ 8, length, -1 });
		}
		else if (script[i] == L'·ç') {
			if (i + 1 < script.size() && script[i + 1] == L'?') {
				if (get<0>(result.tokens.back()) == 8) {
					ll length = get<1>(result.tokens.back());
					result.tokens.pop_back();
					result.tokens.push_back({ 4, length, -1 });
				}
				else exitMessage(3, "·ç", { line_number, i });
			}
			else {
				result.tokens.push_back({ 3, -1, -1 });
			}
		}
		else if (script[i] == L'¾Æ') {
			result.tokens.push_back({ 5, -1, -1 });
		}
		else if (script[i] == L'°¡') {
			result.tokens.push_back({ 6, -1, -1 });
		}
		else if (script[i] == L'Àº') {
			if (i + 2 < script.size() && script[i + 1] == L'?' && script[i + 2] == L'Çà') {
				if_jumppoint.push(result.tokens.size());
				result.tokens.push_back({ 9, -1, -1 });
			}
			else exitMessage(2, "?Çà", { line_number, i });
			i += 2;
		}
		else if (script[i] == L'ÅÐ') {
			if (i + 2 < script.size() && script[i + 1] == L'!' && script[i + 2] == L'ÀÚ') {
				get<1>(result.tokens[if_jumppoint.top()]) = result.tokens.size();
				if_jumppoint.pop();
			}
			else exitMessage(2, "?ÀÚ", { line_number, i });
			i += 2;
		}
		else if (script[i] == L'0') {
			if (i + 2 < script.size() && script[i + 1] == L'¤µ' && script[i + 2] == L'0') {
				ll value = 0;
				ll mode = 0;
				if (i + 3 < script.size() && (script[i + 3] == L'!' || script[i + 3] == L'?')) {
					auto ret = parseLiteral(script, i + 3);
					value = ret.first;
					i = ret.second - 1;
				}
				else if (i + 3 < script.size() && (script[i + 3] == L'¸ô' || script[i + 3] == L'¸ð')) {
					mode = 1, value = 1;
					i += 3;
					while (i < script.size() && !((script[i] == L'¸ô' || script[i] == L'¿Ã')))
						value++, i++;
					i--;
				}
				else i += 2;
				result.tokens.push_back({ 10, mode, value });
			}
			else exitMessage(2, "¤µ0", { line_number, i });
		}
		else if (script[i] == L'ÀÚ') {
			if (i + 1 < script.size() && script[i + 1] == L'!') {
				result.tokens.push_back({ 7, -1, -1 });
			}
			else exitMessage(2, "!", { line_number, i });
		}
		else if (script[i] == '\n') {
			line_number++;
			result.line_backpoint.push_back(result.tokens.size());
		}
		else if (script[i] == ' ') {

		}
		else {
			exitMessage(4, to_string(script[i]), {line_number, i});
		}
	}
	return result;
}