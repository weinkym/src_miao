##!/usr/local/bin/python3
import os
import platform
import sys


# encoding=utf8 
def getPathList(path):
    fileList=[]
    for parent,dirnames,filenames in os.walk(path,  topdown=False):
        for filename in filenames:
            if '/rundir/' in filename:
                continue
            if filename[-3:] == ".py":
                print("中parent=",parent,"filename=",filename)
#                 # os.system("cd "+parent+";git add "+filename)
        
    return fileList

# print("中parent=")
# temp = sys.getdefaultencoding()  
print(platform.python_version())
path="/Users/miaozw/Documents/TEMP/build-lj"
# path="/Users/miaozw/work/src_miao/python"

getPathList(path)
