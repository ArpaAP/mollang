import compile from "./compile";
import run, { ENV } from "./runtime";
import tokenize, { Tokenized } from "./tokenizer";
import show from "./result";

/**
 * Mollang 실행
 * @param code - 실행할 Mollang 코드
 * @param options - 옵션
 * @returns [출력, 오류 코드, 오류 위치, 파싱 결과]
 */
export default function main(
    code: string,
    options: {
        maxRecursion?: number;
        outputFn?: (msg: string) => void;
        errorFn?: (code: number, position: number) => void;
        inputFn?: () => string;
    } = {}
): [string[], number[], number[], string] {
    const {
        maxRecursion = 100000,
        outputFn = process.stdout.write.bind(process.stdout),
        errorFn = (code: number, position: number) =>
            process.stderr.write(
                `* ${position}번째 코드에서 오류 코드 ${code} (이)가 발생했습니다.\n`
            ),
        inputFn = undefined,
    } = options;

    code = code
        .split("\n")
        .map((line: string) => line.trim())
        .join("\n");

    const outputs: string[] = [];
    const errorCodes: number[] = [];
    const errorPositions: number[] = [];

    const data = tokenize(code, (errorCode, position) => {
        errorFn && errorFn(errorCode, position);
        errorCodes.push(errorCode);
        errorPositions.push(position);
    });
    const compiled = compile(data, (errorCode, position) => {
        errorFn && errorFn(errorCode, position);
        errorCodes.push(errorCode);
        errorPositions.push(position);
    });

    const defaultEnv = new ENV();

    run(
        defaultEnv,
        data,
        compiled,
        maxRecursion,
        (msg) => {
            outputFn && outputFn(msg.toString());
            outputs.push(msg.toString());
        },
        (errorCode, position) => {
            errorFn && errorFn(errorCode, position);
            errorCodes.push(errorCode);
            errorPositions.push(position);
        },
        inputFn
    );
    return [outputs, errorCodes, errorPositions, show(data, compiled)];
}

/**
 * Mollang 파싱
 * @param code - 실행할 Mollang 코드
 * @returns [오류 코드, 오류 위치, 파싱 결과]
 */
export function parse(code: string): [number[], number[], Tokenized] {
    code = code
        .split("\n")
        .map((line: string) => line.trim())
        .join("\n");

    const errorCodes: number[] = [];
    const errorPositions: number[] = [];

    const data = tokenize(code, (errorCode, position) => {
        errorCodes.push(errorCode);
        errorPositions.push(position);
    });

    return [errorCodes, errorPositions, data];
}
