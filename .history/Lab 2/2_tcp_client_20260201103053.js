const net = require('net');
const readline = require('readline');
const client = new net.Socket();

const rl = readline.createInterface({
    input: process.stdin, output:process.stdout
});

client.connect(5000, '127.0.0.1', () => {
    console.log('connect ok ');
    askInput();
})

function askInput(){
    rl.question('Nhap input : ', (input) => {
        client.write(input);
    });
}

client.on('data', (data) => {
    console.log('${data}\n');
    askInput();
});

client.on('close', () => {
    console.log('Ngat ket noi');
    rl.close();
});