export default function exitMessage(
    code: number,
    data: string,
    pos: [number, number] | []
) {
    const posMsg = `[${pos.join(":")}] `;
    switch (code) {
        case 1:
            process.stderr.write(
                posMsg + data + " 가 잘못된 위치에 있습니다.\n"
            );
            break;
        case 2:
            process.stderr.write(posMsg + data + " 가 필요합니다\n");
            break;
        case 3:
            process.stderr.write(
                posMsg +
                    data +
                    " 는 하나 이상의 인자가 필요합니다. (또는 변수가 선언되지 않았습니다.)\n"
            );
            break;
        case 4:
            process.stderr.write(
                posMsg + data + " 인식할 수 없는 토큰입니다.\n"
            );
            break;
        default:
            process.stderr.write(
                posMsg + data + " 알 수 없는 오류가 발생했습니다.\n"
            );
    }
}
