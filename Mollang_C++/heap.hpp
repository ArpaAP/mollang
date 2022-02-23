#include <bits/stdc++.h>
using ll = long long;
using namespace std;

class BigInt {
public:
	vector<char> digit;
};

const ll NOT_HEAP = 0;

template <typename T>
class Heap {
public:
	vector<vector<T>> heap_data;
	
	T read(ll i, ll j, TokenPosition position, stack<CallStack>& callstack) {
		if (i <= 0 || j <= 0) ErrorCode(HEAP_OUT_OF_INDEX, position, callstack);
		if (i <= heap_data.size() && j <= heap_data[i - 1].size())
			return heap_data[i - 1][j - 1];
		ErrorCode(HEAP_OUT_OF_INDEX, position, callstack);
	}

	bool canRead(ll i, ll j) {
		if (i <= 0 || j <= 0) 
			return false;
		if (i <= heap_data.size() && j <= heap_data[i - 1].size())
			return true;
		return false;
	}

	T& write(ll i, ll j, TokenPosition position, stack<CallStack>& callstack) {
		if (i <= 0 || j <= 0) ErrorCode(HEAP_OUT_OF_INDEX, position, callstack);

		while (heap_data.size() < i) heap_data.push_back({});
		while (heap_data[i - 1].size() < j) heap_data[i - 1].push_back(0);

		return heap_data[i - 1][j - 1];
	}

	ll data_size(ll i, ll j) {
		stack<CallStack> callstack;
		ll cnt = 0;
		while (canRead(i, j) && read(i, j, { -1, -1 }, callstack) != 0) {
			cnt++, j++;
		}
		return cnt;
	}
};

class Pointer {
public:
	ll type = NOT_HEAP;
	pair<ll, ll> position;
	Pointer(ll type, ll i_var, ll j_var) {
		this->type = type;
		position = { i_var, j_var };
	}
	Pointer() {};
};