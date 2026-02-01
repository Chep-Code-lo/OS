// Import thư viện cần thiết
const net = require('net');           // Để tạo TCP client
const readline = require('readline'); // Để đọc input từ bàn phím

// Tạo client kết nối tới server
const client = new net.Socket();

// Tạo interface để nhập liệu từ console
const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

// Kết nối tới server tại localhost:5000
client.connect(5000, '127.0.0.1', () => {
    console.log('Kết nối thành công!');
    console.log('Nhập 3 số a b c (cách nhau bởi dấu cách)\n');
    askInput();
});

// Hàm hỏi người dùng nhập số
function askInput() {
    rl.question('Nhập a b c: ', (input) => {
        // Gửi dữ liệu tới server
        client.write(input);
    });
}

// Khi nhận kết quả từ server
client.on('data', (data) => {
    console.log(`→ ${data}\n`);
    askInput(); // Hỏi tiếp
});

// Khi kết nối đóng
client.on('close', () => {
    console.log('Ngắt kết nối');
    rl.close();
});
