#ifndef IMRPC_H
#define IMRPC_H

#include "publicdefines.h"
#include <functional>
#include <vector>
#include <string>

enum crypto_type {
    crypto_type_none = 0,
    crypto_type_aes,
    crypto_type_des,
};

enum error_define {
    error_define_ok = 0,
    error_define_error,
};

struct conversation_data {
    int64_t time_stamp;
    int32_t crypto_type;
    std::string cid;
    std::string crypto_key;
    std::string decrypto_key;
//    bool operator == (const conversation_data &other){return cid != "" && cid == other.cid;}
};

struct message_data {
    int64_t time_stamp;
    std::string cid;
    std::string mid;
    std::string text;
};

struct error_info {
    int64_t time_stamp;
    int32_t error;
    std::string  info;
};

struct result {
    int64_t time_stamp;
    int32_t error;
    std::string  info;
};

class ImRpcDelegate {
public:
    virtual ~ImRpcDelegate() = default;

    virtual void onAddConversation(const conversation_data&) = 0;

    virtual void onRemoveConversation(const conversation_data&) = 0;

    virtual void onUpdateConversation(const conversation_data&) = 0;

    virtual void onRecv(const std::vector<message_data>&) = 0;
};

class ImRpc {
public:
    void setRpcDelegate(ImRpcDelegate*);

    void createConversation(
        const conversation_data& conv,
        std::function<void(const result&)> callback);

    void removeConversation(
        const conversation_data& conv,
        std::function<void(const result&)> callback);

    void fechConversations(
        int64_t start, int64_t offset,
        std::function<void(const std::vector<conversation_data>&, const result&)> callback);

    void fechMessage(
        const std::string& cid, int64_t start, int64_t offset,
        std::function<void(const std::vector<message_data>&, const result&)> callback);

    void sendMessage(
            const std::string& cid,
            const message_data& data,
            std::function<void(const result&)> callback);

    void sendMessage(
        const std::string& cid,
        std::function<void(const std::vector<message_data>&, const result&)> callback);

    std::string getCryptoText(const std::string &text,crypto_type type,const std::string &key);
    int findConvExisted(const std::string &cid);
    int findMsgExisted(const std::string &mid);

private:
    ImRpcDelegate *m_imRpcDelegate;
    std::vector<conversation_data> m_conversationDataVec;
    std::vector<message_data> m_messageDataVec;

};
#endif // IMRPC_H
