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
const short HEAP_OUT_OF_INDEX = -10;

using ll = long long;
using namespace std;

class TokenPosition {
public:
	ll line, index;
};

class CallStack {
public:
	u16string name;
	vector<ll> parameters;
	string filename;
};

wstring_convert<codecvt_utf8<char16_t>, char16_t> converter;

void ErrorCode(short code, TokenPosition position, stack<CallStack> callstack) {
	cerr << "\nError Occured\n";
	cerr << "at " << callstack.top().filename << '[' << position.line + 1 << ':' << position.index << ']';
	while (!callstack.empty()) {
		cerr << "\nin " << callstack.top().filename << ':' << converter.to_bytes(callstack.top().name) << '(';
		for (ll i = 0; i < callstack.top().parameters.size(); i++) {
			cerr << callstack.top().parameters[i];
			if (i + 1 != callstack.top().parameters.size())
				cerr << ", ";
		}
		cerr << ")";
		callstack.pop();
	}
	exit(code);
}