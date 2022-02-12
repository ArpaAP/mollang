#include <bits/stdc++.h>
#include <codecvt>
#include "Runtime/vm.hpp"
using namespace std;

int main(int argc, char* argv[]) {
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

	wifstream in(file);
	in.imbue(locale(locale(), new codecvt_utf8_utf16<wchar_t, 0x10ffff, codecvt_mode(consume_header | generate_header)>));

	wstring script, line;
	while (getline(in, line)) {
		script.append(line);
		script += '\n';
	}

	Tokenized token = tokenize(script);
	Compiled src = compile(token);

	if (show_parsed) show_compiled(token, src);

	ENV default_env = ENV();
	return run(default_env, token, src);
}