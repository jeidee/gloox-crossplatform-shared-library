//
//  TalkClient.h
//  talk.demo
//
//  Created by 신창헌 on 2015. 5. 13..
//  Copyright (c) 2015년 jeidee. All rights reserved.
//

#ifndef __talk_demo__TalkClient__
#define __talk_demo__TalkClient__

#pragma once

#include <vector>
#include <thread>

#include "SingletonHolder.h"
#include "MsgClient.h"
#include "TalkClientEventHandler.h"
#include "BackoffManager.h"
#include "CachedInfo.h"

namespace jd {

    
    class TalkClient : public MsgClient {
        friend class SingletonHolder<TalkClient>;
        
        enum ConnectState {
            NONE,
            DISCONNECTED,
            CONNECTING,
            CONNECTED
        };

    public:
        virtual ~TalkClient();

        const CachedInfo& getCachedInfo() { return mCachedInfo; }
        void resetCache() { mCachedInfo.reset(); }
        
        bool connected();
        bool running();

        void init(std::string filePath);
        bool connect(std::string jid, std::string pwd, std::string host, int port);

        /*
         * for thread
         */
        static void runThread();
        static void runHeartbeatThread();
        
        void start();
        void stop();
        
        void run();
        void runHeartbeat();

        
        virtual void setPresence(gloox::Presence::PresenceType presenceType, int priority, string status);

    private:

        /*
         * callback functions
         */
        
        virtual void callbackConnect();
        virtual void callbackDisconnect(int e);
        virtual void callbackRoster(const Roster& roster);
        virtual void callbackVCard(const char* jid, const char* nickname, const char* photo);
        virtual void callbackPresence(const RosterItem& roster, const char* resource, Presence::PresenceType presence, const char* msg);
        virtual void callbackSubscriptionRequest(const char* jid, const char* msg);
        virtual void callbackUnsubscriptionRequest(const char* jid, const char* msg);
        virtual void callbackItemSubscribed(const char* jid);
        virtual void callbackItemAdded(const char* jid);
        virtual void callbackItemUnsubscribed(const char* jid);
        virtual void callbackItemRemoved(const char* jid);
        virtual void callbackItemUpdated(const char* jid);
        virtual void callbackMessage(const Message& msg, MessageSession* session);
        virtual void callbackLog(const char* tag, const char* log);

        
    private:
        TalkClient();
        void Log(std::string tag, std::string message);
      
        std::string mFilePath;

        bool mRecvThreadRunning;
        uint32_t mConnectionTimeoutSeconds = 30;    // 연결타임아웃 : 기본값 = 30초
        ConnectState mConnectState;
        TalkClientEventHandler mTalkClientEventHandler;
        BackoffManager mBackoff;
        CachedInfo mCachedInfo;
        std::thread mRecvThread;
        std::thread mHeartbeatThread;
        
    };  // class TalkClient
};  // namespace jd


#define TALK_CLIENT jd::SingletonHolder<jd::TalkClient>::instance()

#endif /* defined(__talk_demo__TalkClient__) */
