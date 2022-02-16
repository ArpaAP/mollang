const short UNKNOWN_TOKEN = 1;
const short MISSING_PAIR = 2;
const short MISSING_PARAMETER = 3;

const short UNDEFINED_VARIABLE = -1;
const short WRONG_EXPRESSION = -2;
const short WRONG_MULTIPLY = -3;
const short WRONG_PARAMETER = -4;
const short MISSING_MID_PARAMETER = -5;
const short DIVIDE_BY_ZERO = -6;
const short MOD_WITH_FLOAT = -7;
const short UNDEFINED_FUNCTION = -8;
const short UNMATCHED_PARAMETER = -9;

using ll = long long;
using namespace std;

void ErrorCode(short code, ll position) {
	cout << '[' << position + 1 << ']';
	exit(code);
}