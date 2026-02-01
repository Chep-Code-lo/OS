const dgram = require('dgram');
const readline = require('readline');

const client = dgram.createSocket('udp4');
const rl = readline.createInterface({ input: process.stdin, output: process.stdout });

function ask() {
    rl.question('Nhập a b c: ', (input) => {
        const msg = Buffer.from(input);
        client.send(msg, 5000, '127.0.0.1', () => {
            console.log('Đã gửi\n');
        });
    });
}

console.log('UDP Client\n');
ask();

client.on('message', (msg) => {
    console.log(`→ ${msg}\n`);
    
    rl.question('Tiếp? (y/n): ', (a) => {
        if (a === 'y') ask();
        else { client.close(); rl.close(); }
    });
});
