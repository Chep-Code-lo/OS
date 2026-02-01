const dgram = require('dgram');
const readline = require('readline');

const client = dgram.createSocket('udp4');
const rl = readline.createInterface({ input: process.stdin, output: process.stdout });

console.log('UDP Chat Client\n');

rl.question('Nhập tên của bạn: ', (name) => {
    const nameMsg = Buffer.from('NAME:' + name);
    client.send(nameMsg, 5004, '127.0.0.1', () => {
        console.log(`Chào ${name}! Bắt đầu chat:\n`);
        chat();
    });
});

function chat() {
    rl.question('', (message) => {
        if (message.toLowerCase() === 'quit') {
            client.close();
            rl.close();
        } else {
            const msg = Buffer.from(message);
            client.send(msg, 5004, '127.0.0.1');
        }
    });
}

client.on('message', (msg) => {
    const data = msg.toString();
    if (data !== 'ok') {
        console.log(data);
    }
    if (data === 'ok') {
        chat(); 
    }
});
