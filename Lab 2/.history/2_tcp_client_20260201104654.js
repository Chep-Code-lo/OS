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

function askInput() {
    rl.question('Nhập 2 ma trận (vd: [[1,2],[3,4]] [[5,6],[7,8]]): ', (input) => {
        try {
            const [m1str, m2str] = input.split(']] [[');
            const m1 = JSON.parse(m1str + ']]');
            const m2 = JSON.parse('[[' + m2str);
            
            console.log('Gửi:', m1, 'x', m2, '\n');
            client.write(JSON.stringify({ m1, m2 }));
        } catch (e) {
            console.log('Lỗi format! Thử lại\n');
            askInput();
        }
    });
}
client.on('data', (data) => {
    const res = JSON.parse(data);
    console.log('→ Kết quả:', res, '\n');
    
    rl.question('Tiếp tục? (y/n): ', (answer) => {
        if (answer.toLowerCase() === 'y') {
            askInput();
        } else {
            console.log('Tạm biệt!');
            client.destroy();
            rl.close();
        }
    });
});

client.on('close', () => {
    console.log('Ngắt kết nối');
    rl.close();
});