//
//  CachedInfo.h
//  talk.demo
//
//  Created by 신창헌 on 2015. 5. 14..
//  Copyright (c) 2015년 jeidee. All rights reserved.
//

#ifndef __talk_demo__CachedInfo__
#define __talk_demo__CachedInfo__

#pragma once

#include <string>

namespace jd {
    class CachedInfo {
    public:
        CachedInfo();
        virtual ~CachedInfo();
        
        void init(std::string filePath);
        void setLoginInfo(std::string jid, std::string pwd, std::string host, int port);
        void setPresenceInfo(std::string status);
        
        std::string jid() const { return mJid; }
        std::string pwd() const { return mPwd; }
        std::string host() const { return mHost; }
        int port() const { return mPort; }
        std::string status() const { return mStatus; }

        void load();
        void save();
        void reset();

    private:
        std::string mFilePath;
        
        std::string mJid;
        std::string mPwd;
        std::string mHost;
        int mPort;
        
        std::string mStatus;
    };  // class CachedInfo
}   // namespace jd

#endif /* defined(__talk_demo__CachedInfo__) */
