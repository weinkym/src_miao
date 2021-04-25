#include "imrpc.h"
#include <time.h>
const char *g_crypto_key = "crypto_key";
const char *g_decrypto_key = "decrypto_key";

void ImRpc::setRpcDelegate(ImRpcDelegate *imRpcDelegate)
{
    m_imRpcDelegate = imRpcDelegate;
}

void ImRpc::createConversation(const conversation_data &conv, std::function<void (const result &)> callback)
{
    result res;
    res.error = error_define_ok;


    if(findConvExisted(conv.cid) >= 0)
    {
        if(m_imRpcDelegate)
        {
            m_imRpcDelegate->onAddConversation(conv);
//            std::vector<message_data> msgs;
            m_conversationDataVec.push_back(conv);
        }
        else
        {
            res.error = error_define_error;
            res.info = "imRpcDelegate is null";
        }
    }
    else
    {
        res.error = error_define_error;
        res.info = "conversion is existed";
    }
    if(callback)
    {
        callback(res);
    }
}

void ImRpc::removeConversation(const conversation_data &conv, std::function<void (const result &)> callback)
{
    result res;
    res.error = error_define_ok;

    auto index = findConvExisted(conv.cid);
    if(index >= 0)
    {
        m_conversationDataVec.erase(m_conversationDataVec.begin() + index);
        if(m_imRpcDelegate)
        {
            m_imRpcDelegate->onRemoveConversation(conv);
        }
        else
        {
            res.error = error_define_error;
            res.info = "imRpcDelegate is null";
        }
    }
    else
    {
        res.error = error_define_error;
        res.info = "conversion is not existed";
    }
    if(callback)
    {
        callback(res);
    }
}

void ImRpc::fechConversations(int64_t start, int64_t offset, std::function<void (const std::vector<conversation_data> &, const result &)> callback)
{
    std::vector<conversation_data> convs;
    result res;
    res.error = error_define_ok;
    for (size_t i = start; i < start + offset; i ++)
    {
        if( i >= m_conversationDataVec.size())
        {
            break;
        }
        auto obj = m_conversationDataVec[i];
        convs.push_back(obj);
    }
    if(convs.size() == 0)
    {
        res.error = error_define_error;
        res.info = "not found";
    }
    if(callback)
    {
        callback(convs,res);
    }
}

void ImRpc::fechMessage(const std::string &cid, int64_t start, int64_t offset, std::function<void (const std::vector<message_data> &, const result &)> callback)
{
    std::vector<message_data> msgs;
    result res;
    res.error = error_define_ok;
    for (size_t i = start; i < start + offset; i ++)
    {
        if( i >= m_messageDataVec.size())
        {
            break;
        }
        auto obj = m_messageDataVec[i];
        msgs.push_back(obj);
    }
    if(msgs.size() == 0)
    {
        res.error = error_define_error;
        res.info = "not found";
    }
    if(callback)
    {
        callback(msgs,res);
    }
}

void ImRpc::sendMessage(const std::string &cid, const message_data &data, std::function<void (const result &)> callback)
{
    result res;
    res.error = error_define_ok;
    auto index = findConvExisted(cid);
    if(findConvExisted(cid) >= 0)
    {
        res.error = error_define_error;
        res.info = "cid is existed";
    }
    else
    {
        //这里假设消息里带了mid，实际业务可能本地无此mid，可能需要服务器返回更新
        if(findConvExisted(data.mid) >= 0)
        {
            res.error = error_define_error;
            res.info = "mid is existed";
        }
        else
        {
            auto conv = m_conversationDataVec[index];
            message_data msg = data;
            msg.text = getText(data.text,conv.crypto_type,);
            m_messageDataVec.push_back(msg);
        }
    }
    if(callback)
    {
        callback(res);
    }
}

std::string ImRpc::getCryptoText(const std::string &text, crypto_type type, const std::string &key)
{
    std::string v = text;
    if(type == crypto_type_aes)
    {
        //do crypto_type_aes
    }
    else if(type == crypto_type_des)
    {
        //do crypto_type_des
    }
    return v;

}

int ImRpc::findConvExisted(const std::string &cid)
{

    for (size_t i =0; i < m_conversationDataVec.size(); i ++)
    {
        auto obj = m_conversationDataVec[i];
        if(obj.cid == cid)
        {
            return i;
            break;
        }
    }
    return -1;
}

int ImRpc::findMsgExisted(const std::string &mid)
{
    for (size_t i =0; i < m_conversationDataVec.size(); i ++)
    {
        auto obj = m_messageDataVec[i];
        if(obj.mid == mid)
        {
            return i;
            break;
        }
    }
    return -1;
}
