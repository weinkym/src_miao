#!/usr/local/bin/python3
import os

def getPathList(path):
    fileList=[]
    for parent,dirnames,filenames in os.walk(path,  topdown=False):
        for filename in filenames:
            if '/rundir/' in filename:
                continue
            if filename[-3:] == ".py":
                print("parent=",parent,"filename=",filename)
                os.system("cd "+parent+";git add "+filename)
        

    return fileList


# path="/Users/miaozw/Documents/TEMP/build-lj"
path="/Users/miaozw/work/src_miao/python"

getPathList(path)
