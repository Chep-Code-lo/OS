const dgram = require('dgram');
const server = dgram.createSocket('udp4');

function solve(a, b, c) {
    const delta = b * b - 4 * a * c;
    if (delta < 0) return 'Vô nghiệm';
    if (delta === 0) {
        const x = -b / (2 * a);
        return `Nghiệm kép: x = ${x.toFixed(2)}`;
    }
    const x1 = (-b + Math.sqrt(delta)) / (2 * a);
    const x2 = (-b - Math.sqrt(delta)) / (2 * a);
    return `x1 = ${x1.toFixed(2)}, x2 = ${x2.toFixed(2)}`;
}

server.on('message', (msg, rinfo) => {
    const nums = msg.toString().split(' ').map(Number);
    const [a, b, c] = nums;
    
    console.log(`Nhận: ${a}x² + ${b}x + ${c} = 0`);
    
    const result = solve(a, b, c);
    console.log(`→ ${result}\n`);
    
    server.send(result, rinfo.port, rinfo.address);
});

server.bind(5000, () => {
    console.log('UDP Server chạy cổng 5000\n');
});
