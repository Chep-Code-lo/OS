const dgram = require('dgram');
const readline = require('readline');

const client = dgram.createSocket('udp4');
const rl = readline.createInterface({ input: process.stdin, output: process.stdout });

console.log('UDP Client\n');
console.log('Lệnh:');
console.log('  UPPER|text');
console.log('  LOWER|text');
console.log('  DELETE|text|start|end');
console.log('  INSERT|text|position|substring\n');

function ask() {
    rl.question('Nhập lệnh: ', (input) => {
        const msg = Buffer.from(input);
        client.send(msg, 5003, '127.0.0.1');
    });
}

ask();

client.on('message', (msg) => {
    console.log(`→ ${msg}\n`);
    
    rl.question('Tiếp? (y/n): ', (a) => {
        if (a === 'y') ask();
        else { client.close(); rl.close(); }
    });
});
