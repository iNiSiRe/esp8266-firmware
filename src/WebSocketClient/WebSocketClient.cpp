#include "WebSocketClient.h"
#include "Base64.h"

void WebSocketClient::handshake()
{
    char keyStart[17];
    char b64Key[25];
    String key = "------------------------";

    randomSeed(0);

    for (int i=0; i<16; ++i) {
        keyStart[i] = (char)random(1, 256);
    }

    base64_encode(b64Key, keyStart, 16);

    for (int i=0; i<24; ++i) {
        key[i] = b64Key[i];
    }

    String packet;
    packet.concat("GET / HTTP/1.1\r\n");
    packet.concat("Upgrade: websocket\r\n");
    packet.concat("Connection: Upgrade\r\n");
    packet.concat("Host: " + host + ":" + port + "\r\n");
    packet.concat("Sec-WebSocket-Key: " + key + "\r\n");
    packet.concat("Sec-WebSocket-Protocol: \r\n");
    packet.concat("Sec-WebSocket-Version: 13\r\n\r\n");

    state = STATE_WAIT_HANDSHAKE;

    client->sendString(packet);
}

WebSocketClient::WebSocketClient()
{
    client = new TcpClient(
        TcpClientCompleteDelegate(&WebSocketClient::onComplete, this),
        TcpClientEventDelegate(&WebSocketClient::onEvent, this),
        TcpClientDataDelegate(&WebSocketClient::onDataReceived, this)
    );
}

bool WebSocketClient::onDataReceived(TcpClient &client, char *data, int size)
{
    debugf("Received data length %d", size);

    if (size == 0 || data == null) {
        return true;
    }

    switch (state) {
        case STATE_WAIT_HANDSHAKE:
            if (isSuccessHandshake(data, size)) {
                debugf("Handshake success");
                state = STATE_HANDSHAKE_SUCCESS;
                client.setTimeOut(65535);
            } else {
                debugf("Handshake failed");
            }
            break;

        case STATE_READY:
            if (onData) {
                String message = handle(data, size);
                onData(message);
            }
            break;

        default:
            break;
    }

    return true;
}

void WebSocketClient::onEvent(TcpClient &client, TcpConnectionEvent sourceEvent)
{
    switch (sourceEvent) {
        case eTCE_Connected:
            debugf("Connected to socket\n");
            if (state == STATE_NONE) {
                state = STATE_CONNECTED;
                handshake();
            }
            break;

        case eTCE_Sent:
            debugf("Sent");
        case eTCE_Poll:
            debugf("Poll");
            break;

        case eTCE_Received:
            debugf("Receive done\n");
            if (state == STATE_HANDSHAKE_SUCCESS) {
                state = STATE_READY;
                if (onReady) {
                    onReady();
                }
            }
            break;
    }
}

void WebSocketClient::onComplete(TcpClient &client, bool successful) {
    debugf("Complete: %s", successful);
}

bool WebSocketClient::send(String message, uint8_t opcode)
{
    debugf("Start send message: %s, length: %d\n", message.c_str(), message.length());

    const int size = message.length();

    int header = 0;

    if (size > 125) {
        header = 8;
    } else {
        header = 6;
    }

    char packet[header + size];

    const char mask[4] = {10, 20, 30, 40};

    // Opcode; final fragment
    packet[0] = (char) (opcode | WS_FIN);

    if (size > 125) {
        packet[1] = (char) (WS_SIZE16 | WS_MASK);
        packet[2] = (char) (size >> 8);
        packet[3] = (char) (size & 0xFF);
        packet[4] = mask[0];
        packet[5] = mask[1];
        packet[6] = mask[2];
        packet[7] = mask[3];
    } else {
        packet[1] = (char) (size | WS_MASK);
        packet[2] = mask[0];
        packet[3] = mask[1];
        packet[4] = mask[2];
        packet[5] = mask[3];
    }

    for (int i = 0; i < size; ++i) {
        packet[i + header] = message.charAt(i) ^ mask[i % 4];
    }

    debugf("Size: %d", sizeof(packet));

    int sent = client->write(packet, sizeof(packet));
    client->flush();

    if (sent == sizeof(packet)) {
        debugf("Message send success");
    } else {
        debugf("Message send failed, %d / %d bytes sent, can send %d bytes", sent, size, client->getAvailableWriteSize());
    }

    debugf("Mask: %d %d %d %d", mask[0], mask[1], mask[2], mask[3]);

    for (int i = 0; i < sizeof(packet); ++i) {
        debugf("Char at %d = %d", i, packet[i]);
    }

    return true;
}

String WebSocketClient::handle(char * packet, int size)
{
    debugf("Start handle packet length: %d", size);

    int pointer = 0;
    int opcode = packet[pointer++] & ~WS_FIN;
    int length = packet[pointer++];
    int mask[4];
    bool hasMask = false;

    if (length & WS_MASK) {
        hasMask = true;
        length = length & ~WS_MASK;
    }

    if (length == WS_SIZE16) {
        length = packet[pointer++] << 8;
        length |= packet[pointer++];
    } else if (length == WS_SIZE64) {
        return String("");
    }

    if (hasMask) {
        mask[0] = packet[pointer++];
        mask[1] = packet[pointer++];
        mask[2] = packet[pointer++];
        mask[3] = packet[pointer++];
    }

    String message;

    for (int i = 0; i < length; ++i) {
        if (hasMask) {
            message += (char) (packet[pointer++] ^ mask[i % 4]);
        } else {
            message += (packet[pointer++]);
        }
    }

    debugf("End handle");

    return message;
}

bool WebSocketClient::isSuccessHandshake(char * data, int size)
{
    String packet = String(data);
    Vector <String> vector;

    packet.replace('\r', ' ');
    splitString(packet, '\n', vector);

    debugf("Headers found: %d\n", vector.count());

    if (vector.count() == 0) {
        return false;
    }

    for (unsigned int i = 0; i < vector.count(); i ++) {
        Vector <String> header;
        String raw = vector.elementAt(i);
        splitString(raw, ':', header);
        if (header.count() != 2) {
            continue;
        }

        String name = header.elementAt(0);
        String value = header.elementAt(1);

        name.trim();
        value.trim();

        if (name.equals("Upgrade")) {
            debugf("Reached Upgrade header\n");
            return true;
        }
    }

    return false;
}

void WebSocketClient::connect(String host, int port) {
    this->host = host;
    this->port = port;
    client->connect(host, port);
}

TcpClientState WebSocketClient::getConnectionState() {
    return client->getConnectionState();
}
