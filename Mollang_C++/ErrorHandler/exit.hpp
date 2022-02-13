const short UNKNOWN_TOKEN = 1;
const short MISSING_PAIR = 2;
const short MISSING_PARAMETER = 3;

const short UNDEFINED_VARIABLE = -1;
const short WRONG_EXPRESSION = -2;
const short WRONG_MULTIPLY = -3;
const short WRONG_PARAMETER = -4;
const short MISSING_MID_PARAMETER = -5;

using ll = long long;
using namespace std;

void ErrorCode(short code, ll position) {
	cout << '[' << position + 1 << ']';
	exit(code);
}