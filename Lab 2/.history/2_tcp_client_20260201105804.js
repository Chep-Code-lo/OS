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
    rl.question(`${name}: `, (line) => {
        cb(JSON.parse(line));
    });
}

client.connect(5000, '127.0.0.1', () => {
    console.log('OK\n');
    ask();
});

function ask(){
    input('A vd [[1,2],[3,4]]', (m1) => {
        input('B vd [[5,6],[7,8]]', (m2) => {
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