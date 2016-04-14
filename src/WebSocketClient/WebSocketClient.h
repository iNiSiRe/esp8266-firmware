//
// Created by inisire on 11.04.16.
//

#ifndef SMARTHOME_WEBSOCKETCLIENT_H
#define SMARTHOME_WEBSOCKETCLIENT_H

#include <SmingCore/SmingCore.h>

// WebSocket protocol constants

// First byte
#define WS_FIN            0x80
#define WS_OPCODE_TEXT    0x01
#define WS_OPCODE_BINARY  0x02
#define WS_OPCODE_CLOSE   0x08
#define WS_OPCODE_PING    0x09
#define WS_OPCODE_PONG    0x0a

// Second byte
#define WS_MASK           0x80
#define WS_SIZE16         126
#define WS_SIZE64         127

typedef Delegate <void (String message)> WebSocketDataDelegate;
typedef Delegate <void ()> WebSocketClientDelegate;

enum WebSocketClientState {
    STATE_NONE = 0,
    STATE_CONNECTED,
    STATE_WAIT_HANDSHAKE,
    STATE_HANDSHAKE_SUCCESS,
    STATE_READY
};

class WebSocketClient {

protected:
    TcpClient * client;
    WebSocketClientState state = STATE_NONE;
    String host;
    int port;

    bool onDataReceived(TcpClient &client, char *data, int size);
    void onEvent(TcpClient& client, TcpConnectionEvent sourceEvent);
    void onComplete(TcpClient& client, bool successful);
    String handle(char *packet, int size);

    bool isSuccessHandshake(char *data, int size);
    void handshake();
public:
    WebSocketClientDelegate onReady;
    WebSocketDataDelegate onData;

    WebSocketClient();
    void connect(String host, int port);
    bool send(String message, uint8_t opcode = WS_OPCODE_TEXT);
    TcpClientState getConnectionState();
};


#endif //SMARTHOME_WEBSOCKETCLIENT_H
