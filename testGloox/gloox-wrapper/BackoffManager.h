//
// Created by 신창헌 on 15. 5. 13..
// Copyright (c) 2015 jeidee. All rights reserved.
//


#ifndef __BackoffManager_H_
#define __BackoffManager_H_

#pragma once

namespace jd {
    class BackoffManager {
    public:
        BackoffManager();
        virtual ~BackoffManager();

        void init(int maxRetryCount, int initWaitBackoffSeconds, bool noGiveupAndRepeat);
        void reset();
        bool retry();

    private:
        int mMaxRetryCount          = 10;
        int mInitWaitBackoffSeconds = 2;
        bool mNoGiveupAndRepeat     = false;

        int mRetryCount;
        int mWaitBackoffSeconds;
        int mRepeatCycle;

    };  // class BackoffManager
}   // namespace jd


#endif //__BackoffManager_H_
