#include <vector>
#include <string>
#include <iostream>
#include <codecvt>
using namespace std;
using ll = long long;

void exitMessage(ll code, string data, pair<ll, ll> pos) {
	wstring dat = wstring_convert<codecvt_utf8<wchar_t>, wchar_t>().from_bytes(data);

	if (code == 0) {
		wcout << dat << L" �� MOLLANG �ڵ尡 �ƴմϴ�.\n";
	}
	else {
		cout << "[" << pos.first << ":" << pos.second << "] ";
		if (code == 1) {
			wcout << dat << L" �� �߸��� ��ġ�� �ֽ��ϴ�.\n";
		}
		if (code == 2) {
			wcout << dat << L" �� �ʿ��մϴ�.\n";
		}
		if (code == 3) {
			wcout << dat << L" �� �ϳ� �̻��� ���ڰ� �ʿ��մϴ�. (�Ǵ� ������ ������� �ʾҽ��ϴ�.)\n";
		}
		if (code == 4) {
			wcout << dat << L" �ν��� �� ���� ��ū�Դϴ�.\n";
		}
	}

	exit(1);
}