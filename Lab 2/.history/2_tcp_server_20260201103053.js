const net = require('net')

function solve(){

}

const server = net.createServer((socket) => {
    console.log('Client connect');
    socket.on('data', (data) => {
        const input = data.toString().trim();

        const ans = solve();
        socket.write(ans);
        console.log('Gui');
    });
});

server.listen(5000, () => {
    console.log('Serer chay ...')
});