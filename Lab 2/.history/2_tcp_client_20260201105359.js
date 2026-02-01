const net = require('net');
const readline = require('readline');

const client = new net.Socket();
const rl = readline.createInterface({ input: process.stdin, output: process.stdout });

// In ma trận
function show(m, name) {
    console.log(`${name}:`);
    for (let i = 0; i < m.length; i++) {
        let row = '  ';
        for (let j = 0; j < m[i].length; j++) {
            row += m[i][j] + '  ';
        }
        console.log(row);
    }
}

// Nhập JSON - ngắn nhất
function input(name, cb) {
    rl.question(`${name}: `, (line) => {
        cb(JSON.parse(line));
    });
}

client.connect(5000, '127.0.0.1', () => {
    console.log('OK\n');
    ask();
});

function ask() {
    input('A', (m1) => {
        input('B', (m2) => {
            show(m1, '\nA'); 
            show(m2, 'B');
            console.log('\nGửi...');
            client.write(JSON.stringify({ m1, m2 }));
        });
    });
}

client.on('data', (data) => {
    show(JSON.parse(data), '\nA×B');
    rl.question('\nTiếp? (y/n): ', (a) => {
        if (a === 'y') ask();
        else { client.destroy(); rl.close(); }
    });
});

client.on('close', () => rl.close());