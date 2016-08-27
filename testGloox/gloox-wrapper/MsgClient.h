#ifndef JD_MSG_CLIENT_H__
#define JD_MSG_CLIENT_H__

#include "gloox/client.h"
#include "gloox/messagesessionhandler.h"
#include "gloox/messageeventhandler.h"
#include "gloox/messageeventfilter.h"
#include "gloox/chatstatehandler.h"
#include "gloox/chatstatefilter.h"
#include "gloox/connectionlistener.h"
#include "gloox/disco.h"
#include "gloox/message.h"
#include "gloox/gloox.h"
#include "gloox/lastactivity.h"
#include "gloox/loghandler.h"
#include "gloox/logsink.h"
#include "gloox/connectiontcpclient.h"
#include "gloox/connectionbosh.h"
#include "gloox/connectionsocks5proxy.h"
#include "gloox/connectionhttpproxy.h"
#include "gloox/messagehandler.h"
#include "gloox/rostermanager.h"
#include "gloox/vcardhandler.h"
#include "gloox/vcardmanager.h"
#include "gloox/vcard.h"
#include "gloox/eventhandler.h"

using namespace gloox;

#ifdef __ANDROID_NDK__
#include <jni.h>
#endif

#include <string>
#include <vector>
#include <map>
#include <list>
#include <sstream>
#include <mutex>
#include <thread>

using namespace std;

#include "IMsgCallback.h"

namespace jd {
    typedef struct {
        bool isSet;
        string jid;
        string pwd;
        string host;
        int port;
        
        void clear() {
            isSet = false;
            jid = "";
            pwd = "";
            host = "";
            port = 0;
        }
    } LoginInfo;
    
    class MsgClient : public MessageSessionHandler
    , ConnectionListener
    , LogHandler
    , MessageEventHandler
    , MessageHandler
    , ChatStateHandler
    , RosterListener
    , VCardHandler
    , EventHandler
    {
        typedef map<string, MessageSession*> MessageSessionT;
        typedef MessageSessionT::iterator MessageSessionIt;
        
    public:
#ifdef __ANDROID_NDK__
        MsgClient(JavaVM* jvm, JNIEnv* env, jobject obj);
#else
        MsgClient();
#endif
        virtual ~MsgClient();
        
        void setLoginInfo(const char* jid, const char* pwd, const char* host, int port);
        const char* jid() { return m_loginInfo.jid.c_str(); }
        
        // for connection
        virtual bool connect();
        virtual bool disConnect();
        // for vcard
        virtual void fetchVCard(const char* jid);
        // for presence
        virtual void setPresence(int presenceType, int priority, const char* status);
        // for roster
        virtual void subscribe(const char* jid, const char* name, const char* groups, const char* msg);
        virtual void unsubscribe(const char* jid, const char* msg);
        virtual RosterItem* getRosterItem(const char* jid);
        virtual void ackSubscriptionRequest(const char* jid, bool ack);
        virtual void remove(const char* jid);
        // for message
        virtual const char* sendMessage(const char* jid, const char* msg);
        
        virtual ConnectionError recv();
        virtual void heartBeat();
        void setNickname(const char* nickname);
        void setPhoto(const char* photo);
        
        // for receive thread
        static void recvLoop(MsgClient* msgClient);
        
        // for callback functions
        void registerOnConnect(callbackConnectFunc callback) {
            m_onConnect = callback;
        }
        void registerOnDisconnect(callbackDisconnectFunc callback) {
            m_onDisconnect = callback;
        }
        void registerOnLog(callbackLogFunc callback) {
            m_onLog = callback;
        }
        
    protected:
        
        // ConnectionListener
        virtual void onConnect();
        virtual void onDisconnect(ConnectionError e);
        virtual bool onTLSConnect(const CertInfo& info);
        
        // EventHandler
        virtual void handleEvent(const Event& event);
        
        // MessageHandler
        virtual void handleMessage(const Message& msg, MessageSession* /*session*/);
        
        // MessageEnvetHandler
        virtual void handleMessageEvent(const JID& from, MessageEventType event);
        
        // ChatStateHandler
        virtual void handleChatState(const JID& from, ChatStateType state);
        
        // MessageSessionHandler
        virtual void handleMessageSession(MessageSession* session);
        
        // LogHandler
        virtual void handleLog(LogLevel level, LogArea area, const string& message);
        
        // RosterListener
        virtual void handleItemSubscribed(const JID& jid);
        virtual void handleItemAdded(const JID& jid);
        virtual void handleItemUnsubscribed(const JID& jid);
        virtual void handleItemRemoved(const JID& jid);
        virtual void handleItemUpdated(const JID& jid);
        virtual void handleRoster(const Roster& roster);
        virtual void handleRosterError(const IQ& /*iq*/);
        virtual void handleRosterPresence(const RosterItem& item, const std::string& resource,
                                          Presence::PresenceType presence, const std::string& /*msg*/);
        virtual void handleSelfPresence(const RosterItem& item, const std::string& resource,
                                        Presence::PresenceType presence, const std::string& /*msg*/);
        virtual bool handleSubscriptionRequest(const JID& jid, const std::string& /*msg*/);
        virtual bool handleUnsubscriptionRequest(const JID& jid, const std::string& /*msg*/);
        virtual void handleNonrosterPresence(const Presence& presence);
        
        // VCardHandler
        virtual void handleVCard(const JID& jid, const VCard *v);
        virtual void handleVCardResult(VCardContext context, const JID& jid,
                                       StanzaError se = StanzaErrorUndefined);
        
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
    public:
        virtual void callbackLog(const char* tag, const char* log);
        
    protected:
        
        /*
         * string utilities
         */
        static list<string> split(const char* source, char delimeter);
        static std::string format(const char* fmt, ...);
        static std::string format(const char* fmt, va_list args);

        
    private:
        
        
        //    MessageSession*     m_session;
        //    MessageEventFilter* m_messageEventFilter;
        //    ChatStateFilter*    m_chatStateFilter;
        MessageSessionT m_sessions;
        
        Client*             m_client;
        LoginInfo           m_loginInfo;
        VCardManager*       m_vcardManager;
        VCard*              m_vcard;
        std::mutex          m_lock;
        int                 m_heartBeat;
        
        std::thread         m_recvThread;
        
        // callback functions
        callbackConnectFunc     m_onConnect;
        callbackDisconnectFunc  m_onDisconnect;
        callbackLogFunc         m_onLog;
        
        
#ifdef __ANDROID_NDK__
        JavaVM*     m_jvm;
        JNIEnv*     m_env;
        jobject     m_obj;
        jmethodID   m_cbOnConnect;
#endif
    };  // class MsgClient
};  // namespace jd

#endif  // JD_MSG_CLIENT_H__