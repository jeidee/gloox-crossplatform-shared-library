//
// Created by 신창헌 on 15. 5. 13..
// Copyright (c) 2015 jeidee. All rights reserved.
//

#include <unistd.h>
#include "BackoffManager.h"

namespace jd {
    BackoffManager::BackoffManager() {

    }

    BackoffManager::~BackoffManager() {

    }

    void BackoffManager::init(int maxRetryCount, int initWaitBackoffSeconds, bool noGiveupAndRepeat) {
        mMaxRetryCount = maxRetryCount;
        mInitWaitBackoffSeconds = initWaitBackoffSeconds;
        mNoGiveupAndRepeat = noGiveupAndRepeat;

        mRetryCount = 0;
        mWaitBackoffSeconds = mInitWaitBackoffSeconds;

    }

    void BackoffManager::reset() {
        mRetryCount = 0;
        mWaitBackoffSeconds = mInitWaitBackoffSeconds;

    }

    bool BackoffManager::retry() {
        if (mNoGiveupAndRepeat == false && mRepeatCycle > 0) {
            return false;
        }

        mRetryCount++;
        mWaitBackoffSeconds *= 2;

        if (mRetryCount > mMaxRetryCount) {
            mRetryCount = 0;
            mWaitBackoffSeconds = mInitWaitBackoffSeconds;
            mRepeatCycle++;
        }

        sleep(mWaitBackoffSeconds);

        return true;
    }

}   // namespace jd