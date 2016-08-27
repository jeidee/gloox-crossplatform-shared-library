//
//  TalkClient.cpp
//  talk.demo
//
//  Created by 신창헌 on 2015. 5. 13..
//  Copyright (c) 2015년 jeidee. All rights reserved.
//

#include "TalkClient.h"
#include "Util.h"
#include "BackoffManager.h"
#include <unistd.h>
#include <thread>

using namespace std;

namespace jd {
    TalkClient::TalkClient() :
            mConnectState(TalkClient::NONE) {
    }
    
    TalkClient::~TalkClient() {
    }
    
    
    void TalkClient::init(string filePath) {
        if (mConnectState != TalkClient::NONE) {
            return;
        }

        mFilePath = filePath;
        
        mConnectState = TalkClient::DISCONNECTED;
        mBackoff.init(10, 2, true);
        mCachedInfo.init(filePath);
    }

    bool TalkClient::connected() {
        if (mConnectState == TalkClient::CONNECTED) {
            return true;
        } else {
            return false;
        }
    }
    
    void TalkClient::setPresence(gloox::Presence::PresenceType presenceType, int priority, string status) {
        mCachedInfo.setPresenceInfo(status);
        MsgClient::setPresence(presenceType, priority, status.c_str());
    }

    
    bool TalkClient::connect(std::string jid, std::string pwd, std::string host, int port) {
        if (mConnectState != TalkClient::DISCONNECTED)
            return false;
        
        setLoginInfo(jid.c_str(), pwd.c_str(), host.c_str(), port);
        mCachedInfo.setLoginInfo(jid, pwd, host, port);
        
        mConnectState = TalkClient::CONNECTING;
        
        if (!MsgClient::connect()) {
            Log("TalkClient", "connect failed.");
            mConnectState = TalkClient::DISCONNECTED;
            return false;
        }
        
        uint32_t elapsedTime = 0;
        uint32_t startTime = Util::getTick();
        
        while (elapsedTime < mConnectionTimeoutSeconds * 1000) {
            elapsedTime = Util::getTick() - startTime;
            
            
            usleep(1);
            
            if (recv() != ConnNoError) {
                Log("TalkClient", "recv failed.");
                mConnectState = TalkClient::DISCONNECTED;
                return false;
            }
            
            if (connected()) {
                return true;
            }
        }
        
        Log("TalkClient", "connect timeout!");
        mConnectState = TalkClient::DISCONNECTED;
        
        return false;
    }

    bool TalkClient::running() {
        return mRecvThreadRunning;
    }

    void TalkClient::runThread() {
        TALK_CLIENT->run();
    }
    
    void TalkClient::runHeartbeatThread() {
        TALK_CLIENT->runHeartbeat();
    }

    // recv 워커쓰레드
    void TalkClient::run() {
        mRecvThreadRunning = true;

        while (mRecvThreadRunning) {
            usleep(10);

            if (mConnectState == TalkClient::CONNECTING ||
                    mConnectState == TalkClient::CONNECTED) {
                recv();
                continue;
            }

            // 연결 재시도
            while (mRecvThreadRunning) {
                if (connect(mCachedInfo.jid(), mCachedInfo.pwd(), mCachedInfo.host(), mCachedInfo.port())) {
                    mBackoff.reset();
                    break;
                }

                if (mBackoff.retry() == false) {
                    Log("MsgClient", "최대 연결 재시도 횟수를 초과했습니다. 연결에 실패했습니다.");
                    mRecvThreadRunning = false;
                    return;
                }
            }
        }

        if (mConnectState == TalkClient::CONNECTED) {
            disConnect();
        }

        mConnectState = TalkClient::DISCONNECTED;
    }
    
    // hearbeat 워커쓰레드
    void TalkClient::runHeartbeat() {
        while (running()) {
            if (connected()) {
                heartBeat();
            }
            sleep(10);  // 10초에 한 번씩
        }
    }

    void TalkClient::start() {
        if (running())
            return;

        mRecvThread = std::thread(TalkClient::runThread);
        mHeartbeatThread = std::thread(TalkClient::runHeartbeatThread);
    }

    void TalkClient::stop() {
        mRecvThreadRunning = false;
    }


    void TalkClient::Log(std::string tag, std::string message) {
        mTalkClientEventHandler.onLog(tag, message);
    }
    
    /*
     * callback functions
     */
    
    void TalkClient::callbackConnect() {
        mConnectState = TalkClient::CONNECTED;

        // 프리젠스 전송
        TALK_CLIENT->setPresence(Presence::PresenceType::Available, 0, mCachedInfo.status());
        
        // 이벤트 콜백
        mTalkClientEventHandler.onConnect();
    }

    void TalkClient::callbackDisconnect(int e) {
        mConnectState = TalkClient::DISCONNECTED;

        mTalkClientEventHandler.onDisconnect(e);
    }

    void TalkClient::callbackRoster(const Roster& roster) {
        mTalkClientEventHandler.onRoster(roster);
    }

    void TalkClient::callbackVCard(const char* jid, const char* nickname, const char* photo) {
        mTalkClientEventHandler.onVCard(jid, nickname, photo);
    }

    void TalkClient::callbackPresence(const RosterItem& roster, const char* resource, Presence::PresenceType presence, const char* msg) {
        mTalkClientEventHandler.onPresence(roster, resource, presence, msg);
    }

    void TalkClient::callbackSubscriptionRequest(const char* jid, const char* msg) {
        mTalkClientEventHandler.onSubscriptionRequest(jid, msg);
    }

    void TalkClient::callbackUnsubscriptionRequest(const char* jid, const char* msg) {
        mTalkClientEventHandler.onUnsubscriptionRequest(jid, msg);
    }

    void TalkClient::callbackItemSubscribed(const char* jid) {
        mTalkClientEventHandler.onItemSubscribed(jid);
    }

    void TalkClient::callbackItemAdded(const char* jid) {
        mTalkClientEventHandler.onItemAdded(jid);
    }

    void TalkClient::callbackItemUnsubscribed(const char* jid) {
        mTalkClientEventHandler.onItemUnsubscribed(jid);
    }

    void TalkClient::callbackItemRemoved(const char* jid) {
        mTalkClientEventHandler.onItemRemoved(jid);
    }

    void TalkClient::callbackItemUpdated(const char* jid) {
        mTalkClientEventHandler.onItemUpdated(jid);
    }

    void TalkClient::callbackMessage(const Message& msg, MessageSession* session) {
        mTalkClientEventHandler.onMessage(session->target().bare().c_str(), msg.id().c_str(), msg.subtype(), msg.subject().c_str(), msg.body().c_str(), msg.thread().c_str());
    }

    void TalkClient::callbackLog(const char* tag, const char* log) {
        mTalkClientEventHandler.onLog(tag, log);
    }
    
    
};  // namespace jd