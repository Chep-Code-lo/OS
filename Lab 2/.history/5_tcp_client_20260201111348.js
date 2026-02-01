const net = require('net');
const readline = require('readline');

const client = new net.Socket();
const rl = readline.createInterface({ input: process.stdin, output: process.stdout });

client.connect(5004, '127.0.0.1', () => {
    console.log('✓ Kết nối thành công!\n');
});

client.on('data', (data) => {
    const msg = data.toString();
    
    // Nếu là câu hỏi tên
    if (msg.includes('Nhập tên')) {
        process.stdout.write(msg);
        rl.once('line', (name) => {
            client.write(name);
        });
    } else {
        // Hiển thị tin nhắn
        console.log(msg);
        
        // Nếu đã vào chat, cho phép gửi tiếp
        if (msg.includes('Gõ tin nhắn')) {
            promptMessage();
        }
    }
});

function promptMessage() {
    rl.question('', (message) => {
        if (message.toLowerCase() === 'quit') {
            client.destroy();
            rl.close();
        } else {
            client.write(message);
            // Không gọi promptMessage ở đây, đợi server trả về "✓"
        }
    });
}

client.on('close', () => {
    console.log('\nĐã ngắt kết nối');
    rl.close();
});

// Khi nhận được xác nhận, cho nhập tiếp
const originalWrite = process.stdout.write;
process.stdout.write = function(chunk, ...args) {
    const str = chunk.toString();
    if (str.includes('✓ Đã gửi')) {
        originalWrite.apply(process.stdout, [chunk, ...args]);
        promptMessage();
    } else {
        originalWrite.apply(process.stdout, [chunk, ...args]);
    }
};
