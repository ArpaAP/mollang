#include <stack>
#include <vector>
#include <map>
#include "parser.hpp"
using ll = long long;
using namespace std;

ll run(Parsed x, bool shell) {
	wcin.imbue(locale("en_US.UTF-8"));
	wcout.imbue(locale("en_US.UTF-8"));

	stack<ll> runtime;
	map<ll, ll> var;
	bool characterPrint = false;
	for (ll i = 0; i < x.tokens.size(); i++) {
		auto cur = x.tokens[i];
		ll f = get<0>(cur), s = get<1>(cur), t = get<2>(cur);

		if (f == 1) runtime.push(s);

		if (f == 2) {
			if (var.count(s)) var[s] += t;
			else var[s] = t;
		}

		if (f == 11) {
			if (var.count(t) == 0) {
				exitMessage(3, "몰", {});
			}
			if (var.count(s)) 
				var[s] += var[t];
			else var[s] = var[t];
		}

		if (f == 3) {
			if (runtime.empty()) {
				exitMessage(3, "루", {});
			}

			if (characterPrint) wcout << wchar_t(runtime.top());
			else cout << runtime.top();
			runtime.pop();
			characterPrint = false;
		}

		if (f == 4) {
			ll tmp;	wcin >> tmp;
			var[s] = tmp;
		}

		if (f == 5) {
			characterPrint = true;
		}

		if (f == 7) {
			if (runtime.empty()) {
				exitMessage(3, "은?행", {});
			}
			i = x.line_backpoint[runtime.top() - 1] - 1;
			runtime.pop();
		}

		if (f == 8) {
			if (var.count(s)) runtime.push(var[s]);
			else var[s] = 0;
		}

		if (f == 9) {
			if (runtime.empty()) {
				exitMessage(3, "가!자", {});
			}
			if (runtime.top() != 0)
				i = s - 1;
			runtime.pop();
		}

		if (f == 10) {
			if (s == 0) return t;
			else {
				if (var.count(t))
					return var[t];
				else
					exitMessage(3, "0ㅅ0", {});
			}
		}
	}

	if (shell && !runtime.empty())
		return runtime.top();
	return 0;
}