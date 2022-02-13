import tokenize, { Tokenized, LiteralParsed, KEY } from "./tokenizer";
import compile, { Compiled } from "./compile";
import { Errors } from "./exit";
import { getOutputFileNames } from "typescript";

let error: (code: number) => void;

export class ENV {
    variables = new Map<number, number>(); //BigInt후보
    runtimeStack: number[] = []; // BigInt 후보
}

function calc(env: ENV, data: LiteralParsed, idx: number) {
    if (data.content[0] == KEY.MULTIPLY) {
        error(Errors.WRONG_MULTIPLY);
    }
    if (data.content[data.content.length - 1] == KEY.MULTIPLY) {
        error(Errors.WRONG_MULTIPLY);
    }

    let ans = 1;
    let cur = 0; // BigInt 후보

    for (let i = idx; i < data.content.length; i++) {
        if (data.content[i] == KEY.MULTIPLY) {
            ans *= cur;
            cur = 0;
        } else if (data.content[i] == KEY.PLUS) {
            cur++;
        } else if (data.content[i] == KEY.MINUS) {
            cur--;
        } else {
            if (data.content[i] in env.variables) {
                cur += Number(env.variables.get(data.content[i]));
            } else {
                error(Errors.UNDEFINED_VARIABLE);
            }
        }
    }
    ans *= cur;

    return ans;
}

function assign(env: ENV, data: LiteralParsed) {
    if (data.content[0] <= 0) {
        error(Errors.WRONG_EXPRESSION);
    }
    if (data.content[data.content.length - 1] == KEY.MULTIPLY) {
        error(Errors.WRONG_MULTIPLY);
    }
    if (data.content.length == 1) {
        env.variables.set(data.content[0], 0);
        return;
    }

    if (data.content[0] in env.variables) {
        let tmp = Number(env.variables.get(data.content[0]));
        if (data.content[1] == KEY.MULTIPLY) {
            let value = calc(env, data, 2);
            env.variables.set(data.content[0], tmp * value);
        } else {
            let value = calc(env, data, 1);
            env.variables.set(data.content[0], tmp + value);
        }
    } else {
        if (data.content[1] == KEY.MULTIPLY) {
            error(Errors.WRONG_MULTIPLY);
        }
        let value = calc(env, data, 1);
        env.variables.set(data.content[0], value);
    }
}

export default function run(
    env: ENV,
    data: Tokenized,
    compiled: Compiled,
    maxRecursion: number,
    outputFn: (msg: string) => void,
    errorFn: (code: number) => void,
    inputFn: () => string
) {
    error = errorFn;
    let recursions = [];
    for (let i = 0; i <= data.gotoPoint.length; i++) {
        recursions.push(0);
    }
    for (let i = 0; i < data.tokens.length; i++) {
        let token = data.tokens[i];
        if (token[0] == KEY.LITERAL) {
            if (compiled.literal_owned[token[1]]) {
                env.runtimeStack.push(calc(env, data.literals[token[1]], 0));
            } else {
                assign(env, data.literals[token[1]]);
            }
        } else if (token[0] >= KEY.PAIR_KEYWORD) {
            if (token[1] == i) {
                error(Errors.MISSING_MID_PARAMETER);
            }
            if (token[0] == KEY.PAIR_KEYWORD + 1) {
                if (env.runtimeStack[env.runtimeStack.length - 1] != 0) {
                    i = token[1];
                }
                env.runtimeStack.pop();
            } else {
                if (token[1] > i + 1) {
                    error(Errors.WRONG_PARAMETER);
                }
                if (data.tokens[i + 1][0] !== KEY.LITERAL) {
                    error(Errors.WRONG_PARAMETER);
                }
                let midParam = calc(env, data.literals[data.tokens[i + 1][1]], 0);
                if (token[0] == KEY.PAIR_KEYWORD) {
                    outputFn(String.fromCharCode(midParam));
                } else if (token[0] == KEY.PAIR_KEYWORD + 2) {
                    i = data.gotoPoint[midParam - 1] - 1;
                    if (++recursions[midParam - 1] > maxRecursion) {
                        error(Errors.MAX_RECURSION);
                    }
                }
            }
        } else {
            if (token[0] == 0) {
                let tmp = Number(inputFn());
                if (data.literals[data.tokens[i - 1][1]].content.length >= 2) {
                    error(Errors.WRONG_PARAMETER);
                }
                if (data.literals[data.tokens[i - 1][1]].content[0] <= 0) {
                    error(Errors.WRONG_PARAMETER);
                }
                env.variables.set(data.literals[data.tokens[i - 1][1]].content[0], tmp);
            }
            if (token[0] == 1) {
                outputFn(env.runtimeStack[env.runtimeStack.length - 1].toString());
                env.runtimeStack.pop();
            }
            if (token[0] == 2) {
                if (i + 1 < data.tokens.length && data.tokens[i + 1][0] == KEY.LITERAL) {
                    return calc(env, data.literals[data.tokens[i + 1][1]], 0);
                } else {
                    return 0;
                }
            }
        }
    }

    return 0;
}
