#!/usr/local/bin/python3

from wxpy import *

bot = Bot(cache_path=True)

firends = bot.friends()

first_friend = firends[0]
first_friend.send

print("freinds count=",len(firends))

for obj in firends:
    print("name=",type(obj))

# wxpy.api.chats.friend.Friend
