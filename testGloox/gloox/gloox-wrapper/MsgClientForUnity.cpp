//
//  MsgClientForUnity.cpp
//  gloox
//
//  Created by 신창헌 on 2016. 8. 26..
//  Copyright © 2016년 jeidee. All rights reserved.
//

#include "MsgClientForUnity.h"
#include "MsgClient.h"

using namespace jd;

void* createMsgClient() {
    return new MsgClient();
}

void deleteMsgClient(void* msgClient) {
    MsgClient* client = (MsgClient*)msgClient;

    if (client != NULL) {
        delete client;
    }
}

void registerOnConnect(void* msgClient, callbackConnectFunc callback) {
    MsgClient* client = (MsgClient*)msgClient;
    
    client->registerOnConnect(callback);
}

void registerOnDisconnect(void* msgClient, callbackDisconnectFunc callback) {
    MsgClient* client = (MsgClient*)msgClient;
    
    client->registerOnDisconnect(callback);
}

void registerOnLog(void* msgClient, callbackLogFunc callback) {
    MsgClient* client = (MsgClient*)msgClient;
    
    client->registerOnLog(callback);
}

bool connect(void* msgClient, const char* jid, const char* pwd, const char* host, int port) {
    MsgClient* client = (MsgClient*)msgClient;

    client->setLoginInfo(jid, pwd, host, port);

    return client->connect();
}

void disconnect(void* msgClient) {
    MsgClient* client = (MsgClient*)msgClient;
    
    client->disConnect();
}

int recv(void* msgClient) {
    MsgClient* client = (MsgClient*)msgClient;
    
    return client->recv();
}

int test() {
    return 10;
}




