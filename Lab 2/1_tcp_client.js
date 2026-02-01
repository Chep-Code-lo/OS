const net = require('net');         
const readline = require('readline'); 
const client = new net.Socket();

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

client.connect(5000, '127.0.0.1', () => {
    console.log('Kết nối thành công!');
    console.log('Nhập 3 số a b c (cách nhau bởi dấu cách)\n');
    askInput();
});

function askInput() {
    rl.question('Nhập a b c: ', (input) => {
        client.write(input);
    });
}

client.on('data', (data) => {
    console.log(`→ ${data}\n`);
    askInput(); // Hỏi tiếp
});

client.on('close', () => {
    console.log('Ngắt kết nối');
    rl.close();
});
