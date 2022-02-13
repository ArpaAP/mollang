import tokenize, { Tokenized, LiteralParsed, KEY } from "./tokenizer";
import { Errors } from "./exit";

let error: (code: number) => void;

export class Compiled {
    literal_owned: boolean[] = [];
}

const front_parametered = [0, 1, KEY.PAIR_KEYWORD + 1];
const back_parametered = [2];
const non_number_param = [0];
const not_always_require_param = [2];

export default function compile(data: Tokenized, errorFn: (code: number) => void) {
    error = errorFn;
    let ret = new Compiled();
    for (let i = 0; i < data.literals.length; i++) {
        ret.literal_owned.push(false);
    }
    for (let i = 0; i < data.tokens.length; i++) {
        let cur = data.tokens[i][0];
        if (cur === -1) {
            continue;
        }
        if (non_number_param.includes(cur)) {
            continue;
        }
        if (back_parametered.includes(cur)) {
            if (i === data.tokens.length - 1) {
                if (!not_always_require_param.includes(cur)) {
                    error(Errors.MISSING_PARAMETER);
                }
                continue;
            }
            if (data.tokens[i + 1][0] === KEY.LITERAL) {
                if (ret.literal_owned[data.tokens[i + 1][1]]) {
                    error(Errors.MISSING_PARAMETER);
                }
                ret.literal_owned[data.tokens[i + 1][1]] = true;
            } else if (!not_always_require_param.includes(cur)) {
                error(Errors.MISSING_PARAMETER);
            }
        }
        if (front_parametered.includes(cur)) {
            if (i == 0) error(Errors.MISSING_PARAMETER);
            if (data.tokens[i - 1][0] == KEY.LITERAL) {
                if (ret.literal_owned[data.tokens[i - 1][1]]) {
                    error(Errors.MISSING_PARAMETER);
                }
                ret.literal_owned[data.tokens[i - 1][1]] = true;
            }
        }
        if (cur >= KEY.PAIR_KEYWORD) {
            let t = i + 1;
            while (t <= data.tokens[i][1]) {
                if (data.tokens[t][0] == KEY.LITERAL) {
                    ret.literal_owned[data.tokens[t][1]] = true;
                }
                t++;
            }
            i = t - 1;
        }
    }

    return ret;
}
