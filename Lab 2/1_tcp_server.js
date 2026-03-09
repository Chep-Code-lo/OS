const net = require('net');

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

const server = net.createServer((socket) => {
    console.log('Client kết nối!');
    socket.on('data', (data) => {
        const input = data.toString().trim();
        const nums = input.split(' ').map(Number);
        const [a, b, c] = nums;
        
        console.log(`Nhận: ${a}x² + ${b}x + ${c} = 0`);
        const result = solve(a, b, c);
        socket.write(result);
        console.log(`Gửi: ${result}\n`);
    });
});

server.listen(5000, () => {
    console.log('Server chạy tại cổng 5000\n');
});
