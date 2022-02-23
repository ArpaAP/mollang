#include <bits/stdc++.h>
#include "../Compiler/compiler.hpp";
using ll = long long;
using ld = long double;
using namespace std;

class FUNCTION {
public:
	vector<ll> parameter;
	ll st = -1, ed = -1;
};

ll emp = -1;

class RUNTIMEVAR {
public:
	ll value;
};

class ENV {
public:
	map<ll, RUNTIMEVAR> variables;
	map<ll, RUNTIMEVAR*> copyed_variables;
};

class GLOBAL {
public:
	map<u16string, ll> function;
	vector<FUNCTION> functions;
	Heap<ll> int_heap, char_heap;
	Heap<ld> float_heap;
};

ll& write_variable(ENV& env, ll var, TokenPosition position) {
	if (env.variables.count(var))
		return env.variables[var].value;
	if (env.copyed_variables.count(var))
		return env.copyed_variables[var]->value;
	env.variables[var] = { 0 };
	return env.variables[var].value;
}

ll calc(GLOBAL& global, ENV& env, Literal_Parsed& x, ll idx, ll ed, TokenPosition position, stack<CallStack>& callstack) {
	if (ed == -1) ed = x.content.size();
	if (x.content[0] == MULTIPLY) ErrorCode(WRONG_MULTIPLY, position, callstack);
	if (x.content.back() == MULTIPLY) ErrorCode(WRONG_MULTIPLY, position, callstack);

	ll ans = 1, cur = 0;
	ll element_type = MULTIPLY;
	for (ll i = idx; i < ed; i++) {
		TokenPosition errorPos = { position.line, position.index + i };
		if (x.heap_pointer[i].type != NOT_HEAP) {
			Pointer p = x.heap_pointer[i];
			ll p_i = write_variable(env, p.position.first, errorPos), p_j = write_variable(env, p.position.second, errorPos);
			if (p.type == INTHEAP)
				cur += global.int_heap.write(p_i, p_j, errorPos, callstack);
			if (p.type == FLOATHEAP)
				cur += (ll)global.float_heap.write(p_i, p_j, errorPos, callstack);
			if (p.type == STRHEAP)
				cur += global.char_heap.write(p_i, p_j, errorPos, callstack);
		}
		else if (x.content[i] == SIZECOUNTER) {
			if (i + 1 >= ed) ErrorCode(WRONG_EXPRESSION, errorPos, callstack);
			Pointer p = x.heap_pointer[i + 1];
			if (p.type == NOT_HEAP) ErrorCode(WRONG_EXPRESSION, errorPos, callstack);
			ll p_i = write_variable(env, p.position.first, errorPos), p_j = write_variable(env, p.position.second, errorPos);
			if (p.type == INTHEAP)
				cur += global.int_heap.data_size(p_i, p_j);
			if (p.type == FLOATHEAP)
				cur += global.float_heap.data_size(p_i, p_j);
			if (p.type == STRHEAP)
				cur += global.char_heap.data_size(p_i, p_j);
			i++;
		}
		else if (x.content[i] <= MULTIPLY) {
			if (element_type == MULTIPLY) ans *= cur, cur = 0;
			else {
				if (cur == 0) ErrorCode(DIVIDE_BY_ZERO, errorPos, callstack);
				if (element_type == DIVIDE_INT) ans /= cur, cur = 0;
				if (element_type == DIVIDE_MOD) ans %= cur, cur = 0;
			}
			element_type = x.content[i];
		}
		else if (x.content[i] == PLUS) cur++;
		else if (x.content[i] == MINUS) cur--;
		else {
			if (env.variables.count(x.content[i]))
				cur += env.variables[x.content[i]].value;
			else if (env.copyed_variables.count(x.content[i]))
				cur += env.copyed_variables[x.content[i]]->value;
			else ErrorCode(UNDEFINED_VARIABLE, errorPos, callstack);
		}
	}
	if (element_type == MULTIPLY) ans *= cur, cur = 0;
	else {
		if (cur == 0) ErrorCode(DIVIDE_BY_ZERO, { position.line, position.index + ed }, callstack);
		if (element_type == DIVIDE_INT) ans /= cur, cur = 0;
		if (element_type == DIVIDE_MOD) ans %= cur, cur = 0;
	}

	return ans;
}

ld float_calc(GLOBAL& global, ENV& env, Literal_Parsed& x, ll idx, ll ed, TokenPosition position, stack<CallStack>& callstack) {
	if (ed == -1) ed = x.content.size();
	if (x.content[0] == MULTIPLY) ErrorCode(WRONG_MULTIPLY, position, callstack);
	if (x.content.back() == MULTIPLY) ErrorCode(WRONG_MULTIPLY, position, callstack);

	ld ans = 1, cur = 0;
	ll element_type = MULTIPLY;
	for (ll i = idx; i < ed; i++) {
		if (x.heap_pointer[i].type != NOT_HEAP) {
			Pointer p = x.heap_pointer[i];
			ll p_i = write_variable(env, p.position.first, position), p_j = write_variable(env, p.position.second, position);
			if (p.type == INTHEAP)
				cur += (ld)global.int_heap.write(p_i, p_j, position, callstack);
			if (p.type == FLOATHEAP)
				cur += global.float_heap.write(p_i, p_j, position, callstack);
			if (p.type == STRHEAP)
				cur += (ld)global.char_heap.write(p_i, p_j, position, callstack);
		}
		else if (x.content[i] == SIZECOUNTER) {
			if (i + 1 >= ed) ErrorCode(WRONG_EXPRESSION, position, callstack);
			Pointer p = x.heap_pointer[i + 1];
			if (p.type == NOT_HEAP) ErrorCode(WRONG_EXPRESSION, position, callstack);
			ll p_i = write_variable(env, p.position.first, position), p_j = write_variable(env, p.position.second, position);
			if (p.type == INTHEAP)
				cur += global.int_heap.data_size(p_i, p_j);
			if (p.type == FLOATHEAP)
				cur += global.float_heap.data_size(p_i, p_j);
			if (p.type == STRHEAP)
				cur += global.char_heap.data_size(p_i, p_j);
			i++;
		}
		else if (x.content[i] <= MULTIPLY) {
			if (element_type == MULTIPLY) ans *= cur, cur = 0;
			else {
				if (cur == 0) ErrorCode(DIVIDE_BY_ZERO, position, callstack);
				if (element_type == DIVIDE_MOD) ErrorCode(MOD_WITH_FLOAT, position, callstack);
				if (element_type == DIVIDE) ans /= cur, cur = 0;
				if (element_type == DIVIDE_INT) ans = (ll)(ans / cur), cur = 0;
			}
			element_type = x.content[i];
		}
		else if (x.content[i] == PLUS) cur += 1;
		else if (x.content[i] == MINUS) cur -= 1;
		else {
			if (env.variables.count(x.content[i]))
				cur += env.variables[x.content[i]].value;
			if (env.copyed_variables.count(x.content[i]))
				cur += env.copyed_variables[x.content[i]]->value;
			else ErrorCode(UNDEFINED_VARIABLE, position, callstack);
		}
	}
	if (element_type == MULTIPLY) ans *= cur, cur = 0;
	else {
		if (cur == 0) ErrorCode(DIVIDE_BY_ZERO, position, callstack);
		if (element_type == DIVIDE_MOD) ErrorCode(MOD_WITH_FLOAT, position, callstack);
		if (element_type == DIVIDE) ans /= cur, cur = 0;
		if (element_type == DIVIDE_INT) ans = (ll)(ans / cur), cur = 0;
	}

	return ans;
}

void assign(GLOBAL& global, ENV& env, Literal_Parsed& x, TokenPosition position, bool process_using_float, stack<CallStack>& callstack) {
	if (x.content[0] <= 0 && x.content[0] != HEAP_POINTER) ErrorCode(WRONG_EXPRESSION, position, callstack);
	if (x.content.back() == MULTIPLY) ErrorCode(WRONG_EXPRESSION, position, callstack);

	if (x.content.size() == 1) {
		env.variables[x.content[0]] = { 0 };
		return;
	}

	Pointer p; ll i, j;
	if (x.heap_pointer[0].type != NOT_HEAP) {
		p = x.heap_pointer[0];
		i = write_variable(env, p.position.first, position), j = write_variable(env, p.position.second, position);
	}

	if (x.content[1] == MULTIPLY) {
		if (process_using_float) {
			ld value = float_calc(global, env, x, 2, -1, position, callstack);
			if (x.heap_pointer[0].type != NOT_HEAP) {
				if (p.type == INTHEAP)
					global.int_heap.write(i, j, position, callstack) *= (ll)value;
				if (p.type == FLOATHEAP)
					global.float_heap.write(i, j, position, callstack) *= value;
				if (p.type == STRHEAP)
					global.char_heap.write(i, j, position, callstack) *= (ll)value;
			}
			else write_variable(env, x.content[0], position) *= (ll)value;
		}
		else {
			ll value = calc(global, env, x, 2, -1, position, callstack);
			if (x.heap_pointer[0].type != NOT_HEAP) {
				if (p.type == INTHEAP)
					global.int_heap.write(i, j, position, callstack) *= value;
				if (p.type == FLOATHEAP)
					global.float_heap.write(i, j, position, callstack) *= value;
				if (p.type == STRHEAP)
					global.char_heap.write(i, j, position, callstack) *= value;
			}
			else write_variable(env, x.content[0], position) *= value;
		}
	}
	else {
		if (process_using_float) {
			ld value = float_calc(global, env, x, 1, -1, position, callstack);
			if (x.heap_pointer[0].type != NOT_HEAP) {
				if (p.type == INTHEAP)
					global.int_heap.write(i, j, position, callstack) += (ll)value;
				if (p.type == FLOATHEAP)
					global.float_heap.write(i, j, position, callstack) += value;
				if (p.type == STRHEAP)
					global.char_heap.write(i, j, position, callstack) += (ll)value;
			}
			else write_variable(env, x.content[0], position) += (ll)value;
		}
		else {
			ll value = calc(global, env, x, 1, -1, position, callstack);
			if (x.heap_pointer[0].type != NOT_HEAP) {
				if (p.type == INTHEAP)
					global.int_heap.write(i, j, position, callstack) += value;
				if (p.type == FLOATHEAP)
					global.float_heap.write(i, j, position, callstack) += value;
				if (p.type == STRHEAP)
					global.char_heap.write(i, j, position, callstack) += value;
			}
			else write_variable(env, x.content[0], position) += value;
		}
	}
}

ll run(GLOBAL& global, ENV& env, Tokenized& x, Compiled& y, ll st, ll ed, stack<CallStack>& callstack) {
	stack<pair<ll, ll>> jump_return;
	for (ll i = st; i < ed; i++) {
		ll f = get<0>(x.tokens[i]), s = get<1>(x.tokens[i]), t = get<2>(x.tokens[i]);
		if (f == LITERAL) {
			if (y.no_calc[s]) continue;
			if (!y.literal_owned[s]) assign(global, env, x.literals[s], x.tokens_position[i], y.use_float[s], callstack);
		}
		else if (f >= KPAIR) {
			if (s == i) ErrorCode(MISSING_MID_PARAMETER, x.tokens_position[i], callstack);
			if (f == KPAIR + 1) { //은?행 털!자
				if (calc(global, env, x.literals[get<1>(x.tokens[i - 1])], 0, -1, x.tokens_position[i], callstack) != 0)
					i = s;
			}
			else if (f == KPAIR + 2) { //은?행 돌!자
				if (jump_return.empty() || jump_return.top().second != i)
					jump_return.push({ s, i });
				if (calc(global, env, x.literals[get<1>(x.tokens[i - 1])], 0, -1, x.tokens_position[i], callstack) == 0) {
					i = s;
					jump_return.pop();
				}
			}
			else if (f == KPAIR + 3) { //은?행 짓!자
				if (!x.literals[get<1>(x.tokens[i + 1])].is_parameter_set)
					ErrorCode(WRONG_PARAMETER, x.tokens_position[i], callstack);
				if (!x.literals[get<1>(x.tokens[i - 1])].function_identifier)
					ErrorCode(WRONG_PARAMETER, x.tokens_position[i], callstack);
				FUNCTION tmp;
				if (get<0>(x.tokens[i + 1]) == LITERAL) {
					auto& parameter_list = x.literals[get<1>(x.tokens[i + 1])].content;
					bool success = true;
					for (ll t = 0; t < parameter_list.size(); t++) {
						if (parameter_list[t] < SEPERATOR) {
							tmp.parameter.clear();
							y.literal_owned[get<1>(x.tokens[i + 1])] = false;
							success = false;
							break;
						}
						if (parameter_list[t] > SEPERATOR)
							tmp.parameter.push_back(parameter_list[t]);
					}
					if (success) tmp.st = i + 2;
					else tmp.st = i + 1;
				}
				else tmp.st = i + 1;
				tmp.ed = s;
				global.function[x.literals[get<1>(x.tokens[i - 1])].text] = global.functions.size();
				global.functions.push_back(tmp);
				i = s;
			}
			else if (f == KPAIR + 4) { //은?행 가!자
				if (!x.literals[get<1>(x.tokens[i + 1])].is_parameter_set)
					ErrorCode(WRONG_PARAMETER, x.tokens_position[i], callstack);
				if (!x.literals[get<1>(x.tokens[i - 1])].function_identifier)
					ErrorCode(WRONG_PARAMETER, x.tokens_position[i], callstack);
				ENV local;
				auto parameters = x.literals[get<1>(x.tokens[i + 1])];
				if (global.function.count(x.literals[get<1>(x.tokens[i - 1])].text) == 0)
					ErrorCode(UNDEFINED_FUNCTION, x.tokens_position[i], callstack);
				auto& tmp = global.functions[global.function[x.literals[get<1>(x.tokens[i - 1])].text]];
				ll parst = 0, parcnt = 0, returnout = -1;
				parameters.content.push_back(SEPERATOR);
				set<ll> local_vars;
				vector<ll> called_parameter;
				for (ll t = 0; t < parameters.content.size(); t++) {
					if (parameters.content[t] == SEPERATOR) {
						if (parcnt < tmp.parameter.size()) {
							local_vars.insert(tmp.parameter[parcnt]);
							ll tmpv = calc(global, env, parameters, parst, t, x.tokens_position[i], callstack);
							local.variables[tmp.parameter[parcnt++]] = { tmpv };
							called_parameter.push_back(tmpv);
						}
						else {
							if (parameters.content[t - 1] < SEPERATOR)
								ErrorCode(WRONG_EXPRESSION, x.tokens_position[i], callstack);
							returnout = parameters.content[t - 1];
							break;
						}
						parst = t + 1;
					}
				}
				for (auto& k : env.variables) {
					if (local_vars.count(k.first) == 0)
						local.copyed_variables[k.first] = &k.second;
				}
				for (auto& k : env.copyed_variables) {
					if (local_vars.count(k.first) == 0)
						local.copyed_variables[k.first] = k.second;
				}
				if (parcnt == tmp.parameter.size()) {
					callstack.push({ x.literals[get<1>(x.tokens[i - 1])].text, called_parameter, callstack.top().filename });
					ll returnv = run(global, local, x, y, tmp.st, tmp.ed + 1, callstack);
					callstack.pop();
					if (returnout != -1) {
						env.variables[returnout] = { returnv };
					}
				}
				else ErrorCode(UNMATCHED_PARAMETER, x.tokens_position[i], callstack);
				i++;
			}
			else {
				if (s > i + 1) ErrorCode(WRONG_PARAMETER, x.tokens_position[i], callstack);
				if (get<0>(x.tokens[i + 1]) != LITERAL)
					ErrorCode(WRONG_PARAMETER, x.tokens_position[i], callstack);

				ll midparam = calc(global, env, x.literals[get<1>(x.tokens[i + 1])], 0, -1, x.tokens_position[i], callstack);

				if (f == KPAIR + 0) { //아루  
					cout << converter.to_bytes(midparam);
				}
				else if (f == KPAIR + 5) { //가자!
					i = x.gotopoint[midparam - 1] - 1;
				}
			}
		}
		else {
			if (f == 0) { //루?
				if (i == 0)
					ErrorCode(MISSING_PARAMETER, x.tokens_position[i], callstack);
				if (x.literals[get<1>(x.tokens[i - 1])].content.size() >= 2)
					ErrorCode(WRONG_PARAMETER, x.tokens_position[i], callstack);
				if (x.literals[get<1>(x.tokens[i - 1])].content[0] > 0) {
					cin >> env.variables[x.literals[get<1>(x.tokens[i - 1])].content[0]].value;
				}
				else if (x.literals[get<1>(x.tokens[i - 1])].content[0] == HEAP_POINTER) {
					Pointer p = x.literals[get<1>(x.tokens[i - 1])].heap_pointer[0];
					ll p_i = write_variable(env, p.position.first, x.tokens_position[i]),
						p_j = write_variable(env, p.position.second, x.tokens_position[i]);
					if (p.type == INTHEAP)
						cin >> global.int_heap.write(p_i, p_j, x.tokens_position[i], callstack);
					if (p.type == FLOATHEAP)
						cin >> global.float_heap.write(p_i, p_j, x.tokens_position[i], callstack);
					if (p.type == STRHEAP) {
						string tmp;
						cin >> tmp;
						for (ll k = 0; k < tmp.size(); k++)
							global.char_heap.write(p_i, p_j + k, x.tokens_position[i], callstack) = tmp[k];
					}
				}
				else ErrorCode(WRONG_PARAMETER, x.tokens_position[i], callstack);
			}
			if (f == 1) { //루!
				cout << fixed;
				cout << setprecision(calc(global, env, x.literals[get<1>(x.tokens[i - 1])], 0, -1, x.tokens_position[i], callstack));
			}
			if (f == 2) { //루
				if (x.literals[get<1>(x.tokens[i - 1])].content == vector<ll>{ HEAP_POINTER }&&
					x.literals[get<1>(x.tokens[i - 1])].heap_pointer[0].type == STRHEAP) {
					Pointer p = x.literals[get<1>(x.tokens[i - 1])].heap_pointer[0];
					ll p_i = write_variable(env, p.position.first, x.tokens_position[i]),
						p_j = write_variable(env, p.position.second, x.tokens_position[i]);
					ll len = global.char_heap.data_size(p_i, p_j);
					for (ll k = 0; k < len; k++)
						cout << (char)global.char_heap.read(p_i, p_j + k, x.tokens_position[i], callstack);
				}
				else if (y.use_float[get<1>(x.tokens[i - 1])])
					cout << float_calc(global, env, x.literals[get<1>(x.tokens[i - 1])], 0, -1, x.tokens_position[i], callstack);
				else cout << calc(global, env, x.literals[get<1>(x.tokens[i - 1])], 0, -1, x.tokens_position[i], callstack);
			}
			if (f == 3) { //0ㅅ0
				if (i + 1 < x.tokens.size() && get<0>(x.tokens[i + 1]) == LITERAL)
					return calc(global, env, x.literals[get<1>(x.tokens[i + 1])], 0, -1, x.tokens_position[i], callstack);
				else return 0;
			}
		}
		if (!jump_return.empty() && jump_return.top().first == i)
			i = jump_return.top().second - 1;
	}

	return 0;
}