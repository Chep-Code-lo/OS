const net = require('net');
const readline = require('readline');

const client = new net.Socket();
const rl = readline.createInterface({ input: process.stdin, output: process.stdout });

client.connect(5003, '127.0.0.1', () => {
    console.log('✓ Kết nối OK\n');
    console.log('Lệnh:');
    console.log('  UPPER|text');
    console.log('  LOWER|text');
    console.log('  DELETE|text|start|end');
    console.log('  INSERT|text|position|substring\n');
    ask();
});

function ask() {
    rl.question('Nhập lệnh: ', (input) => {
        client.write(input);
    });
}

client.on('data', (data) => {
    console.log(`→ ${data}\n`);
    
    rl.question('Tiếp? (y/n): ', (a) => {
        if (a === 'y') ask();
        else { client.destroy(); rl.close(); }
    });
});

client.on('close', () => {
    console.log('Đã đóng');
    rl.close();
});
