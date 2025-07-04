var ip = location.host;

const log = document.getElementById('log');
const socket = new WebSocket('ws://' + ip);
let LOGIN;

console.log("Socket is oppen");

document.addEventListener('keydown', function (event) {
    if (event.target && event.target.id === 'Message...' && event.key === 'Enter') {
        event.preventDefault();
        document.getElementById('chatSend').click();
    }
});
document.getElementById('LoginInput').addEventListener('keydown', function (event) {
    if (event.key === 'Enter') {
        event.preventDefault();
        document.getElementById('login').click();
    }
});

function logMessage(message) {
    log.textContent += message + '\n';
}

socket.onopen = () => {
    logMessage("Connected to server");
};

socket.onmessage = (event) => {
    logMessage("Received: " + event.data);
};

socket.onclose = () => {
    logMessage("Connection closed");
};

socket.onerror = (error) => {
    logMessage("Error: " + error.message);
};

function loadChat(){
    let login = "LOGIN->"
    login += document.getElementById('LoginInput').value
    LOGIN = document.getElementById('LoginInput').value

    if (socket.readyState === WebSocket.OPEN) {
        socket.send(login);
    } else {
        console.error('WebSocket is not open');
    }
    fetch('/chat.html')
        .then(res => res.text())
        .then(html => {
            document.getElementById('page-content').innerHTML = html;
        });

}

function sendMessage() {
     let input = "CONTENT->";
     input += document.getElementById('Message...').value;

    if (socket.readyState === WebSocket.OPEN) {
        socket.send(input);
    } else {
        console.error('WebSocket is not open');
    }
    document.getElementById('Message...').value = "";
}

socket.addEventListener('message', function (event) {
    console.log('Message from server:', event.data);
});