//
//  IMsgCallback.h
//  gloox
//
//  Created by 신창헌 on 2016. 8. 26..
//  Copyright © 2016년 jeidee. All rights reserved.
//

#ifndef IMsgCallback_h
#define IMsgCallback_h

extern "C" {
    typedef void (*callbackConnectFunc)(void);
    typedef void (*callbackDisconnectFunc)(int e);
    typedef void (*callbackLogFunc)(const char* tag, const char* log);
}

#endif /* IMsgCallback_h */
