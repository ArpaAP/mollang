#include <bits/stdc++.h>
#include <codecvt>
#include "Runtime/vm.hpp"
typedef std::basic_ifstream<char16_t> u16ifstream;
using namespace std;

int main(int argc, char* argv[]) {
	cin.tie(0)->sync_with_stdio(0); cout.tie(0);
	locale::global(locale("ko_KR.UTF-8"));
	wcin.imbue(locale("ko_KR.UTF-8"));
	wcout.imbue(locale("ko_KR.UTF-8"));

	bool show_parsed = false;
	for (ll i = 2; i < argc; i++) {
		if (strcmp(argv[i], "-parsed") == 0) show_parsed = true;
	}

	string file;
	if (argc == 1) file = "main.molu";
	else file = argv[1];

	u16ifstream in(file);
	in.imbue(locale(locale(), new codecvt_utf8_utf16<wchar_t, 0x10ffff, codecvt_mode(consume_header | generate_header)>));

	u16string script, line;
	while (getline(in, line)) {
		script.append(line);
		script += '\n';
	}

	ll block_counter = 0;

	stack<CallStack> callstack;

	callstack.push({ u"Compile", {}, file });
	Tokenized token = tokenize(script, callstack);
	Compiled src = compile(token, callstack, block_counter);
	callstack.pop();

	if (show_parsed) show_compiled(token, src);

	ENV default_env = ENV();
	GLOBAL default_global = GLOBAL();

	callstack.push({ u"Main", {}, file });
	return run(default_global, default_env, token, src, 0, token.tokens.size(), callstack);
}