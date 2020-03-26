#!/usr/local/bin/python3

import sys,os
import datetime
import time

sys.path.append(os.getcwd())
import subprocess
from zwpy import zwutil
from shutil import copy2
import json

def createVa(k,v):
    pass

def creatList(k,objList):

def createStruct(key,value):
    if isinstance(value,str):
        return "QString {};\n".format(value)
    else if isinstance(value,int):
        return "int {};\n".format(value)
    else if isinstance(value,list):
        obj_name="{}_item".format(key)
        return "QList<{}> {};\n".format(obj_name,key)
    else if isinstance(value,dict):
        return "{} {}".format(key,).format(createStruct())
    else:
        print("error")
def test():
    fp_test='/Users/miaozw/Documents/TEMP/test.json'
    fp_w='/Users/miaozw/Documents/TEMP/struct.txt'
    dict_struct={}
    root_name="TEST_STRUCT"
    with open(fp_test,'r') as fr:
        data = json.load(fr)
        print(data.keys())
test()