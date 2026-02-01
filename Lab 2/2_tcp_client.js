const net = require('net');
const readline = require('readline');

const client = new net.Socket();
const rl = readline.createInterface({ input: process.stdin, output: process.stdout });

function show(m, name) {
    console.log(`${name}:`);
    for(let i=0; i<m.length; i++){
        let row = '  ';
        for(let j=0; j<m[i].length; j++){
            row += m[i][j] + '  ';
        }
        console.log(row);
    }
}
function input(name, cb){
    rl.question(`${name} (Kích thước n, m): `, (size) => {
        const [r, c] = size.split(' ').map(Number);
        rl.question(`Nhập ${r*c} số: `, (line) => {
            const n = line.split(' ').map(Number);
            const m = [];
            for(let i=0; i<r; i++){
                m[i] = [];
                for(let j=0; j<c; j++){
                    m[i][j] = n[i*c + j];
                }
            }
            cb(m);
        });
    });
}

client.connect(5000, '127.0.0.1', () => {
    console.log('OK\n');
    ask();
});

function ask(){
    input('Ma trận A : ', (m1) => {
        input('Ma trận B', (m2) => {
            console.log('\nGửi lên server ...');
            client.write(JSON.stringify({ m1, m2 }));
        });
    });
}

client.on('data', (data) => {
    show(JSON.parse(data), 'Kết quả ma trận A×B');
    rl.question('\nTiếp? (y/n): ', (a) => {
        if(a === 'y') ask();
        else{ 
            client.destroy(); 
            rl.close(); 
        }
    });
});

client.on('close', () => {
    console.log('Ngắt kết nối');
    rl.close();
});