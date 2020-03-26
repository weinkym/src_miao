import itchat
from itchat.content import *

@itchat.msg_register([TEXT,MAP,CARD,NOTE,SHARING])
def text_reply(msg):
    print(type(msg))
    print(msg)
    # msg.user.send("{}:{}".format(msg.type,msg.text))

itchat.auto_login(enableCmdQR=2,hotReload=True)

friends=itchat.get_friends(update=True)[0:]
for obj in friends:
    print("NickName({})===>Sex({})==>UniFriend({})".format(obj["NickName"],obj["Sex"],obj['UniFriend']))

# itchat.send_msg()
itchat.run(True)
# itchat.logout()
