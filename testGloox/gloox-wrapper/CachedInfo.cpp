//
//  CachedInfo.cpp
//  talk.demo
//
//  Created by 신창헌 on 2015. 5. 14..
//  Copyright (c) 2015년 jeidee. All rights reserved.
//

#include "CachedInfo.h"
#include "Util.h"
#include <fstream>
#include <iostream>

using namespace std;

namespace jd {

    CachedInfo::CachedInfo() :
        mJid(""), mPwd(""), mHost("msg.iam0.com"), mPort(5223) {
        
    }
    
    CachedInfo::~CachedInfo() {
        save();
    }
    
    void CachedInfo::init(string filePath) {
        mFilePath = filePath + "/cached.txt";
        load();
    }
    
    void CachedInfo::setLoginInfo(std::string jid, std::string pwd, std::string host, int port) {
        mJid = jid;
        mPwd = pwd;
        mHost = host;
        mPort = port;
        
        save();
    }
    
    void CachedInfo::setPresenceInfo(std::string status) {
        
        mStatus = status;

        save();        
    }
    
    void CachedInfo::load() {

        ifstream inFile(mFilePath.c_str());
        
        if (!inFile.is_open()) return;
        
        string input;
        while(!inFile.eof()) {
            getline(inFile, input);
            
            vector<string> tokens = Util::split(input.c_str(), ':');
            if (tokens.size() != 2) continue;
            
            if (tokens[0] == "jid") {
                mJid = tokens[1];
            } else if (tokens[0] == "pwd") {
                mPwd = tokens[1];
            } else if (tokens[0] == "host") {
                mHost = tokens[1];
            } else if (tokens[0] == "port") {
                mPort = atoi(tokens[1].c_str());
            } else if (tokens[0] == "status") {
                mStatus = tokens[1];
            }
        }
        
        inFile.close();
    }
    
    void CachedInfo::save() {
        ofstream outFile(mFilePath.c_str());
        
        if (!outFile.is_open()) return;
        
        outFile << "jid:" << mJid << endl;
        outFile << "pwd:" << mPwd << endl;
        outFile << "host:" << mHost << endl;
        outFile << "port:" << mPort << endl;
        outFile << "status:" << mStatus << endl;
        
        outFile.close();
    }
    
    void CachedInfo::reset() {
        remove(mFilePath.c_str());
    }
    
}