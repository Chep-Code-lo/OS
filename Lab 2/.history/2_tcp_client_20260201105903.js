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
    rl.question(`${name} (r c + số): `, (line) => {
        const n = line.split(' ').map(Number);
        const [r, c] = [n[0], n[1]];
        const m = [];
        for(let i=0; i<r; i++){
            m[i] = [];
            for(let j=0; j<c; j++){
                m[i][j] = n[2 + i*c + j];
            }
        }
        cb(m);
    });
}

client.connect(5000, '127.0.0.1', () => {
    console.log('OK\n');
    ask();
});

function ask(){
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

client.on('close', () => {
    console.log('Ngắt kết nối');
    rl.close();
});