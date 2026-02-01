const net = require('net');

// Hàm giải phương trình bậc 2: ax² + bx + c = 0
function solveQuadratic(a, b, c) {
    try {
        if (a === 0) {
            if (b === 0) {
                if (c === 0) {
                    return "Phương trình vô số nghiệm";
                } else {
                    return "Phương trình vô nghiệm";
                }
            } else {
                const x = -c / b;
                return `Phương trình bậc nhất có nghiệm: x = ${x.toFixed(4)}`;
            }
        }

        // Tính delta
        const delta = b * b - 4 * a * c;

        if (delta < 0) {
            return "Phương trình vô nghiệm (delta < 0)";
        } else if (delta === 0) {
            const x = -b / (2 * a);
            return `Phương trình có nghiệm kép: x = ${x.toFixed(4)}`;
        } else {
            const x1 = (-b + Math.sqrt(delta)) / (2 * a);
            const x2 = (-b - Math.sqrt(delta)) / (2 * a);
            return `Phương trình có 2 nghiệm: x1 = ${x1.toFixed(4)}, x2 = ${x2.toFixed(4)}`;
        }
    } catch (error) {
        return `Lỗi tính toán: ${error.message}`;
    }
}

// Tạo TCP server
const server = net.createServer((socket) => {
    console.log(`[+] Kết nối từ ${socket.remoteAddress}:${socket.remotePort}`);

    socket.on('data', (data) => {
        const message = data.toString().trim();

        if (message.toLowerCase() === 'quit') {
            console.log(`[-] Client ${socket.remoteAddress} ngắt kết nối`);
            socket.end();
            return;
        }

        console.log(`Nhận từ client: ${message}`);

        try {
            const numbers = message.split(/\s+/);
            
            if (numbers.length !== 3) {
                socket.write('Lỗi: Vui lòng gửi đúng 3 số a, b, c');
                return;
            }

            const a = parseFloat(numbers[0]);
            const b = parseFloat(numbers[1]);
            const c = parseFloat(numbers[2]);

            if (isNaN(a) || isNaN(b) || isNaN(c)) {
                socket.write('Lỗi: Dữ liệu không hợp lệ. Vui lòng gửi 3 số');
                return;
            }

            console.log(`Giải phương trình: ${a}x² + ${b}x + ${c} = 0`);
            const result = solveQuadratic(a, b, c);
            console.log(`Kết quả: ${result}`);
            
            socket.write(result);
        } catch (error) {
            socket.write(`Lỗi: ${error.message}`);
        }
    });

    socket.on('end', () => {
        console.log(`Đã đóng kết nối với client\n`);
        console.log('Chờ client mới...\n');
    });

    socket.on('error', (error) => {
        console.log(`[!] Lỗi kết nối: ${error.message}`);
    });
});

const HOST = '127.0.0.1';
const PORT = 5000;

server.listen(PORT, HOST, () => {
    console.log(`TCP Server listening on ${HOST}:${PORT}`);
    console.log('Chờ client kết nối...\n');
});

server.on('error', (error) => {
    console.error(`[!] Server error: ${error.message}`);
});
