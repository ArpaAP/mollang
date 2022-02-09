#include <iostream>
#include <fstream>
#include <string>
#include <codecvt>
#include "runtime.hpp"
using namespace std;

int main(int argc, char* argv[]) {
	locale::global(locale("ko_KR.UTF-8"));
	wcin.imbue(locale("ko_KR.UTF-8"));
	wcout.imbue(locale("ko_KR.UTF-8"));

	bool isShell = false;

	while (1) {
		string file;
		if (argc == 1) {
			wcout << L"실행할 파일 이름을 입력하세요(.molu) > ";
			isShell = true;
			cin >> file;
		}
		else file = argv[1];

		if (file.substr(file.size() - 5) != ".molu") {
			exitMessage(0, file, {});
		}

		wifstream in(file);
		in.imbue(locale(locale(), new codecvt_utf8_utf16<wchar_t, 0x10ffff, codecvt_mode(consume_header | generate_header)>));

		wstring script, line;
		while (getline(in, line)) {
			script.append(line);
			script += '\n';
		}

		if (isShell)
			cout << "\nReturn: "  << run(parse(script), false) << '\n';
		else return run(parse(script), false);
	}
}