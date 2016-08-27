//
//  Util.h
//  talk.demo
//
//  Created by 신창헌 on 2015. 5. 13..
//  Copyright (c) 2015년 jeidee. All rights reserved.
//

#ifndef __talk_demo__Util__
#define __talk_demo__Util__

#pragma once

#include <string>
#include <vector>
#include <stdint.h>
#include <stdarg.h>

namespace jd {
    
    class Util {
    public:
        static uint32_t getTick();
        static std::vector<std::string> split(const char* source, char delimeter);
        static std::string format(const char* fmt, ...);
        static std::string format(const char* fmt, va_list args);

    };
    
}   // namespace jd

#endif /* defined(__talk_demo__Util__) */
