#include <vector>
#include <string>
#include <iostream>
#include <codecvt>
using namespace std;
using ll = long long;

void exitMessage(ll code, string data, pair<ll, ll> pos) {
	wstring dat = wstring_convert<codecvt_utf8<wchar_t>, wchar_t>().from_bytes(data);

	if (code == 0) {
		wcout << dat << L" 는 MOLLANG 코드가 아닙니다.\n";
	}
	else {
		cout << "[" << pos.first << ":" << pos.second << "] ";
		if (code == 1) {
			wcout << dat << L" 가 잘못된 위치에 있습니다.\n";
		}
		if (code == 2) {
			wcout << dat << L" 가 필요합니다.\n";
		}
		if (code == 3) {
			wcout << dat << L" 는 하나 이상의 인자가 필요합니다. (또는 변수가 선언되지 않았습니다.)\n";
		}
		if (code == 4) {
			wcout << dat << L" 인식할 수 없는 토큰입니다.\n";
		}
	}

	exit(1);
}