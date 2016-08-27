
//
//  ITalkClientEventHandler.h
//  talk.demo
//
//  Created by 신창헌 on 2015. 5. 13..
//  Copyright (c) 2015년 jeidee. All rights reserved.
//

#ifndef talk_demo_ITalkClientEventHandlerHandler_h
#define talk_demo_ITalkClientEventHandlerHandler_h

#pragma once

#include <string>
#include "MsgClient.h"

namespace jd {
class TalkClientEventHandler {
public:
    TalkClientEventHandler();
    virtual ~TalkClientEventHandler();

    virtual void onConnect();
    virtual void onDisconnect(int e);
    virtual void onRoster(const Roster& roster);
    virtual void onVCard(const char* jid, const char* nickname, const char* photo);
    virtual void onPresence(const RosterItem& roster, const char* resource, Presence::PresenceType presence, const char* msg);
    virtual void onSubscriptionRequest(const char* jid, const char* msg);
    virtual void onUnsubscriptionRequest(const char* jid, const char* msg);
    virtual void onItemSubscribed(const char* jid);
    virtual void onItemAdded(const char* jid);
    virtual void onItemUnsubscribed(const char* jid);
    virtual void onItemRemoved(const char* jid);
    virtual void onItemUpdated(const char* jid);
    virtual void onMessage(const char* jid, const char* messageId, gloox::Message::MessageType messageType, const char* subject, const char* body, const char* thread);
    virtual void onLog(std::string tag, std::string message);
    
    
private:
    void* mSelf;
};  // class ITalkClientEventHandler
};  // namespace jd

#endif
