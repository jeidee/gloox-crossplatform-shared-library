//
//  Util.cpp
//  talk.demo
//
//  Created by 신창헌 on 2015. 5. 13..
//  Copyright (c) 2015년 jeidee. All rights reserved.
//

#include "Util.h"

#include <unistd.h>
#include <time.h>
#include <sstream>

#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

using namespace std;

namespace jd {
    uint32_t Util::getTick() {
        struct timespec ts;
        unsigned theTick = 0U;
        
#ifdef __MACH__ // OS X does not have clock_gettime, use clock_get_time
        clock_serv_t cclock;
        mach_timespec_t mts;
        host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
        clock_get_time(cclock, &mts);
        mach_port_deallocate(mach_task_self(), cclock);
        ts.tv_sec = mts.tv_sec;
        ts.tv_nsec = mts.tv_nsec;
#else
        clock_gettime( CLOCK_MONOTONIC, &ts );
#endif
        
        theTick  = ts.tv_nsec / 1000000;
        theTick += ts.tv_sec * 1000;
        return theTick;
    }
    
    vector<string> Util::split(const char* source, char delimeter) {
        vector<string> tokenList;
        stringstream ss(source);
        
        string token;
        while (getline(ss, token, delimeter)) {
            tokenList.push_back(token);
        }
        
        return tokenList;
    }
    
    std::string Util::format(const char *fmt, ...)
    {
        va_list args;
        
        va_start( args, fmt );
        
        std::string log = format(fmt, args);
        
        va_end( args );
        
        return log;
    }
    
    std::string Util::format(const char *fmt, va_list args)
    {
        if (!fmt) return "";
        int   result = -1, length = 1024;
        char *buffer = 0;
        while (result == -1)    {
            if (buffer)
            {
                delete[] buffer;
            }
            
            buffer = new char[length + 1];
            memset(buffer, 0, length + 1);
            
            result = vsnprintf(buffer, length, fmt, args);
            
            length *= 2;
        }
        std::string s(buffer);
        delete [] buffer;
        return s;
    }
}