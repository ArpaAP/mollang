import { Tokenized, KEY } from "./tokenizer";
import { Errors } from "./exit";

let error: (code: number, position: number) => void;

export class Compiled {
    literal_owned: boolean[] = [];
    no_calc: boolean[] = [];
}

const front_parametered = [0, 1, KEY.PAIR_KEYWORD + 1];
const back_parametered = [2];
const non_number_parameter = [0];
const not_always_require_param = [2];

export default function compile(
    data: Tokenized,
    errorFn: (code: number, position: number) => void
) {
    error = errorFn;
    let ret = new Compiled();
    for (let i = 0; i < data.literals.length; i++) {
        ret.literal_owned.push(false);
        ret.no_calc.push(false);
    }
    for (let i = 0; i < data.tokens.length; i++) {
        let cur = data.tokens[i][0];
        if (cur === -1) {
            continue;
        }
        if (back_parametered.includes(cur)) {
            if (i === data.tokens.length - 1) {
                if (!not_always_require_param.includes(cur)) {
                    error(Errors.MISSING_PARAMETER, data.tokens_position[i]);
                }
                continue;
            }
            if (data.tokens[i + 1][0] === KEY.LITERAL) {
                if (ret.literal_owned[data.tokens[i + 1][1]]) {
                    error(Errors.MISSING_PARAMETER, data.tokens_position[i]);
                }
                ret.literal_owned[data.tokens[i + 1][1]] = true;
                if (non_number_parameter.includes(cur)) {
                    ret.no_calc[data.tokens[i + 1][1]] = true;
                }
            } else if (!not_always_require_param.includes(cur)) {
                error(Errors.MISSING_PARAMETER, data.tokens_position[i]);
            }
        }
        if (front_parametered.includes(cur)) {
            if (i === 0) error(Errors.MISSING_PARAMETER, data.tokens_position[i]);
            if (data.tokens[i - 1][0] === KEY.LITERAL) {
                if (ret.literal_owned[data.tokens[i - 1][1]]) {
                    error(Errors.MISSING_PARAMETER, data.tokens_position[i]);
                }
                ret.literal_owned[data.tokens[i - 1][1]] = true;
                if (non_number_parameter.includes(cur)) {
                    ret.no_calc[data.tokens[i - 1][1]] = true;
                }
            } else {
                error(Errors.MISSING_PARAMETER, data.tokens_position[i]);
            }
        }
        if (cur >= KEY.PAIR_KEYWORD) {
            let t = i + 1;
            while (t <= data.tokens[i][1]) {
                if (data.tokens[t][0] === KEY.LITERAL) {
                    ret.literal_owned[data.tokens[t][1]] = true;
                }
                t++;
            }
            i = t - 1;
        }
    }

    return ret;
}
