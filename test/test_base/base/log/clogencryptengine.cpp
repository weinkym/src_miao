#include "clogencryptengine.h"

static int s_number_d = 3;
static int s_lowercase_d = 7;
static int s_capital_d = 16;

QString decryption(const QString &log)
{
    QByteArray dataLog = log.toLatin1();
    for(int i = 0; i <dataLog.size();++i)
    {
        char c = dataLog[i];
        if(c >= '0' && c <= '9')
        {
            dataLog[i] = '0'+(dataLog[i]-'0'+10-s_number_d) % 10;
        }
        else if(c >= 'a' && c <= 'z')
        {
            dataLog[i] = 'a'+(dataLog[i]-'a'+26-s_lowercase_d) % 26;
        }
        else if(c >= 'A' && c <= 'Z')
        {
            dataLog[i] = 'A'+(dataLog[i]-'A'+26-s_capital_d) % 26;
        }
    }
    QString res(dataLog);
    return res;
}

QString adjuestContent(const QString &log)
{
    QByteArray dataLog = log.toLatin1().toBase64(QByteArray::Base64Encoding);
    for(int i = 0; i <dataLog.size();++i)
    {
        char c = dataLog[i];
        if(c >= '0' && c <= '9')
        {
            dataLog[i] = '0'+(dataLog[i]-'0'+s_number_d) % 10;
        }
        else if(c >= 'a' && c <= 'z')
        {
            dataLog[i] = 'a'+(dataLog[i]-'a'+s_lowercase_d) % 26;
        }
        else if(c >= 'A' && c <= 'Z')
        {
            dataLog[i] = 'A'+(dataLog[i]-'A'+s_capital_d) % 26;
        }
    }
    QString res(dataLog);
    return res;
}
