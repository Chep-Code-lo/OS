// Import thư viện net để tạo TCP server
const net = require('net');

// Hàm giải phương trình ax² + bx + c = 0
function solve(a, b, c) {
    // Tính delta = b² - 4ac
    const delta = b * b - 4 * a * c;
    
    // Nếu delta < 0: vô nghiệm
    if (delta < 0) return 'Vô nghiệm';
    
    // Nếu delta = 0: nghiệm kép x = -b/2a
    if (delta === 0) {
        const x = -b / (2 * a);
        return `Nghiệm kép: x = ${x.toFixed(2)}`;
    }
    
    // Nếu delta > 0: 2 nghiệm phân biệt
    const x1 = (-b + Math.sqrt(delta)) / (2 * a);
    const x2 = (-b - Math.sqrt(delta)) / (2 * a);
    return `x1 = ${x1.toFixed(2)}, x2 = ${x2.toFixed(2)}`;
}

// Tạo server TCP
const server = net.createServer((socket) => {
    console.log('Client kết nối!');
    
    // Khi nhận data từ client
    socket.on('data', (data) => {
        const input = data.toString().trim();
        
        // Tách chuỗi thành mảng 3 số
        const nums = input.split(' ').map(Number);
        const [a, b, c] = nums;
        
        console.log(`Nhận: ${a}x² + ${b}x + ${c} = 0`);
        
        // Giải và gửi kết quả về client
        const result = solve(a, b, c);
        socket.write(result);
        console.log(`Gửi: ${result}\n`);
    });
});

// Lắng nghe trên cổng 5000
server.listen(5000, () => {
    console.log('Server chạy tại cổng 5000\n');
});
