//
//  MsgClientForUnity.hpp
//  gloox
//
//  Created by 신창헌 on 2016. 8. 26..
//  Copyright © 2016년 jeidee. All rights reserved.
//

#ifndef MsgClientForUnity_hpp
#define MsgClientForUnity_hpp

#include <stdio.h>
#include "IMsgCallback.h"

extern "C" {
    void* createMsgClient();
    void deleteMsgClient(void* msgClient);
    
    void registerOnConnect(void* msgClient, callbackConnectFunc callback);
    void registerOnDisconnect(void* msgClient, callbackDisconnectFunc callback);
    void registerOnLog(void* msgClient, callbackLogFunc callback);
    
    bool connect(void* msgClient, const char* jid, const char* pwd, const char* host, int port);
    void disconnect(void* msgClient);
    int recv(void* msgClient);
    
    int test();
    
}

#endif /* MsgClientForUnity_hpp */
