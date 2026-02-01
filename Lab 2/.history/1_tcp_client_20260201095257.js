const net = require('net');
const readline = require('readline');

const HOST = '127.0.0.1';
const PORT = 5000;

// Tạo TCP client
const client = new net.Socket();

// Tạo interface để đọc input từ console
const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

client.connect(PORT, HOST, () => {
    console.log(`[+] Đã kết nối với server ${HOST}:${PORT}`);
    console.log('Giải phương trình bậc 2: ax² + bx + c = 0');
    console.log('Nhập 3 số a, b, c (cách nhau bởi dấu cách)');
    console.log('Gõ \'quit\' để thoát\n');
    
    promptUser();
});

function promptUser() {
    rl.question('Nhập a b c: ', (input) => {
        const message = input.trim();
        
        if (message.toLowerCase() === 'quit') {
            client.write('quit');
            console.log('Đang ngắt kết nối...');
            setTimeout(() => {
                client.destroy();
                rl.close();
            }, 100);
            return;
        }
        
        client.write(message);
    });
}

client.on('data', (data) => {
    console.log(`Kết quả: ${data.toString()}\n`);
    promptUser();
});

client.on('close', () => {
    console.log('\nĐã đóng kết nối');
    rl.close();
    process.exit(0);
});

client.on('error', (error) => {
    console.log(`[!] Lỗi: ${error.message}`);
    rl.close();
    process.exit(1);
});
