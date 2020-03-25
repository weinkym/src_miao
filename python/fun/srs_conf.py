#!/usr/local/bin/python3

import sys,os
import datetime
import time

sys.path.append(os.getcwd())
import subprocess
from zwpy import zwutil
from shutil import copy2
import json

g_transcode_filePath=""

def createTranscode(app_name,stream_name):
    transcode = ""
    with open(g_transcode_filePath,'r') as f:
        transcode = f.read()
        transcode = transcode.replace("$APP/STREAM$","{}/{}".format(app_name,stream_name))+"\n"
    return transcode

def createIngest(url,need_transcode):
    key="://"
    s_index = url.index(key) + len(key)
    key="/"
    s_index = url.index(key,s_index) + len(key)
    m_index = url.index(key,s_index)
    app_name=url[s_index:m_index]
    stream_name=url[m_index+len(key):]
    key="."
    index = stream_name.find(key)
    stream_short_name = stream_name
    if index >= 0:
        stream_short_name = stream_name[0:index]
    
    print("url={}".format(url))
    print("s_index={}".format(s_index))
    print("m_index={}".format(m_index))
    print("app_name={}".format(app_name))
    print("stream_name={}".format(stream_name))
    print("stream_short_name={}".format(stream_short_name))
    res = (
        "ingest   "+stream_short_name+"{\nenabled  on;\ninput{\ntype    stream;\nurl "+url+";\n}\n"
        "ffmpeg   ./objs/ffmpeg/bin/ffmpeg;engine{\nenabled  off;"
        "\noutput   rtmp://127.0.0.1:[port]/"+app_name+"/"+stream_short_name+";\n}\n}\n"
        )
    if need_transcode:
        res = res + createTranscode(app_name,stream_short_name)
    return res

def createIngest2(k,url,need_transcode):
    if need_transcode:
        if k != "L0":
            return ""
    return createIngest(url,need_transcode)

def getUrls(filePath):
    url_list=[]
    with open(filePath,'r') as f:
        data = json.load(f)
        obj_list = data['list']
        for obj in obj_list:
            url_list.append(obj['flvUrls'])
    return url_list

def getFileContent(filePath):
    with open(filePath,'r') as f:
        return f.read()

def test():
    global g_transcode_filePath
    g_transcode_filePath='/Users/miaozw/Downloads/SRS/transcode.txt'
    fp_urls='/Users/miaozw/Downloads/SRS/flvurls.json'
    fp_listen='/Users/miaozw/Downloads/SRS/listen.txt'
    fp_flv='/Users/miaozw/Downloads/SRS/flv.txt'
    fp_hls='/Users/miaozw/Downloads/SRS/hls.txt'
    url_list=getUrls(fp_urls)
    fp_w='/Users/miaozw/Downloads/SRS/ingest.conf'
    with open(fp_w,'w') as fw:
        fw.write(getFileContent(fp_listen))
        fw.write("\nvhost __defaultVhost__ {\n")
        for url in url_list:
            print(url)
            for (k,v) in  url.items(): 
                print("k={},v={}".format(k,v))
                ingest = createIngest2(k,v,True)
                fw.write(ingest)
        fw.write("\n")
        fw.write(getFileContent(fp_flv))
        fw.write("\n")
        fw.write(getFileContent(fp_hls))
        fw.write("\n}\n")
# test_url = "https://play-test.yunxuetang.com.cn/app90/b448e7d5a732461394a8737370a2cc54.flv?timestamp=1584599590683&secret=bb75daa45c5157dce5dd13e465b4a18c"
# ret = createIngest(test_url)
# print(ret)
# print("{0}{1}{1}".format(1,2))
test()