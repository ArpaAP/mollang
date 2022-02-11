export default function exitMessage(code, data, pos) {
    const posMsg = `[${pos[0]}:${pos[1]}] `;
    if (code == 1) {
        process.stderr.write(posMsg + data + " 가 잘못된 위치에 있습니다.\n");
    }
    if (code == 2) {
        process.stderr.write(posMsg + data + " 가 필요합니다\n");
    }
    if (code == 3) {
        process.stderr.write(
            posMsg +
                data +
                " 는 하나 이상의 인자가 필요합니다. (또는 변수가 선언되지 않았습니다.)\n"
        );
    }
    if (code == 4) {
        process.stderr.write(posMsg + data + " 인식할 수 없는 토큰입니다.\n");
    }
}
