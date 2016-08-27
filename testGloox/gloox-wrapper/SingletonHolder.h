//
//  SingletonHolder.h
//  talk.demo
//
//  Created by 신창헌 on 2015. 5. 13..
//  Copyright (c) 2015년 jeidee. All rights reserved.
//

#ifndef talk_demo_SingletonHolder_h
#define talk_demo_SingletonHolder_h

#pragma once

namespace jd {
    template <class T>
    class SingletonHolder {
    private:
        SingletonHolder(){}
        virtual ~SingletonHolder(){}
    public:
        static T* instance()
        {
            if(!mInstance)
            {
                static T thisInstance;
                mInstance = &thisInstance;
            }
            
            return mInstance;
        }
        
    private:
        static T* mInstance;
    };
    
    template<typename T> T* SingletonHolder<T>::mInstance = 0;
};


#endif
