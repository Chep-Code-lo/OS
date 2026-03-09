const net = require('net')

function solve(a, b){
    const res = [];
    for(let i=0; i<a.length; ++i){
        res[i] = [];
        for(let j=0; j<b[0].length; ++j){
            res[i][j] = 0;
            for(let k=0; k<b.length; ++k)
                res[i][j] += a[i][k]*b[k][j];
        }
    }
    return res; 
}

const server = net.createServer((socket) => {
    console.log('Client connect');
    socket.on('data', (data) => {
        const {m1, m2} = JSON.parse(data);
        const ans = solve(m1, m2);
        console.log('Ket qua', ans, '\n');
        socket.write(JSON.stringify(ans));
        console.log('Gui');
    });
});

server.listen(5000, () => {
    console.log('Serer chay ...')
});