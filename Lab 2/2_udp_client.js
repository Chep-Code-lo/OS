const dgram = require('dgram');
const readline = require('readline');

const client = dgram.createSocket('udp4');
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
    rl.question(`${name} (r c): `, (size) => {
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

function ask(){
    input('A', (m1) => {
        input('B', (m2) => {
            show(m1, '\nA'); 
            show(m2, 'B');
            console.log('\nGửi...');
            
            const msg = Buffer.from(JSON.stringify({ m1, m2 }));
            client.send(msg, 5001, '127.0.0.1');
        });
    });
}

console.log('UDP Client\n');
ask();

client.on('message', (msg) => {
    show(JSON.parse(msg.toString()), '\nA×B');
    
    rl.question('\nTiếp? (y/n): ', (a) => {
        if (a === 'y') ask();
        else { client.close(); rl.close(); }
    });
});
