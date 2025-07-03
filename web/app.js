const log = document.getElementById('log');
const socket = new WebSocket('ws://localhost:8001');

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
    fetch('/chat.html')
        .then(res => res.text())
        .then(html => {
            document.getElementById('page-content').innerHTML = html;
        });
}

function sendMessage() {
    const input = document.getElementById('textInput').value;
    if (socket.readyState === WebSocket.OPEN) {
        socket.send(input);
    } else {
        console.error('WebSocket is not open');
    }
}

socket.addEventListener('message', function (event) {
    console.log('Message from server:', event.data);
});