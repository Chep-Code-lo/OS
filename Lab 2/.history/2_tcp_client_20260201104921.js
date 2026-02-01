const net = require('net');
const readline = require('readline');

const client = new net.Socket();
const rl = readline.createInterface({ input: process.stdin, output: process.stdout });

// In ma trận đẹp
function show(m, name) {
    console.log(`${name}:`);
    m.forEach(row => console.log('  ' + row.join('  ')));
}

// Nhập ma trận
function input(name, cb) {
    rl.question(`${name} - Kích thước (vd: 2 2): `, (size) => {
        const [r, c] = size.split(' ').map(Number);
        const m = [];
        let i = 0;
        function next() {
            if (i < r) {
                rl.question(`  Hàng ${i+1}: `, (line) => {
                    m.push(line.split(' ').map(Number));
                    i++;
                    next();
                });
            } else cb(m);
        }
        next();
    });
}

client.connect(5000, '127.0.0.1', () => {
    console.log('✓ Kết nối OK\n');
    ask();
});

function ask() {
    input('Ma trận A', (m1) => {
        input('Ma trận B', (m2) => {
            show(m1, '\nA'); show(m2, 'B');
            console.log('\n→ Gửi...');
            client.write(JSON.stringify({ m1, m2 }));
        });
    });
}

client.on('data', (data) => {
    const res = JSON.parse(data);
    show(res, '\n✓ Kết quả A×B');
    
    rl.question('\nTiếp? (y/n): ', (a) => {
        a === 'y' ? (console.log(''), ask()) : (client.destroy(), rl.close());
    });
});

client.on('close', () => {
    console.log('Đã đóng');
    rl.close();
});