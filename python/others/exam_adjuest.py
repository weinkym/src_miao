#!/usr/local/bin/python3

import sys,os
sys.path.append(os.getcwd())
import subprocess
from zwpy import zwutil
from shutil import copy2

print(sys.stdout.encoding)

g_option_list=['A','B','C','D','E','F']
g_split_list=['、','.','．']

g_invalid_char_list=['\n','\t','\r',' ']
g_number_char_list=[]
for i in range(0,10):
    g_number_char_list.append("{}".format(i))

def isValidChar(c):
    return c not in g_invalid_char_list

def getNumStr(num):
    return "{}、".format(num)

test_fp="/Users/miaozw/Documents/LOG/test.txt"
content=""

with open(test_fp,mode='r',encoding='utf-8') as f:
    all_c = f.read()
    isNumber=False
    isOption=False
    i=0
    while i < len(all_c):
        c=all_c[i:i+1]
        if not isValidChar(c):
            i=i+1
            continue
        if (c in g_split_list) and isNumber:
            c="、"
        else:
            isNumber = c in g_number_char_list
            if c in g_option_list:
                if i+1 < len(all_c):
                    n_c=all_c[i+1:i+2]
                    if n_c in g_split_list:
                        # content+="\n"
                        content+=c
                        content+=n_c
                        i=i+2
                        continue
        content+=c
        i=i+1
# content=content.lstrip()
# content.strip().strip(b'\x00'.decode())
# content=content.replace('\n','').replace('\t','').replace('\r','').replace(' ','')
num=1
isFound = False
isNumFound = False
numStr="{}、".format(num)
foundStr=""

i=0

find_str=getNumStr(num)
index1 = content.find(getNumStr(num),0)
if index1 < 0:
    print("can not found")
    exit()

result_list=[]

while index1 < len(content):
    # find_str=getNumStr(num)
    # index = content.find(find_str,i)
    # if index < i:
    #     print("not found")
    #     break
    
    index2=content.find(getNumStr(num+1),index1)
    # print("index={},index2={},num={}".format(index1,index2,num))
    if index2 < index1:
        print("2not found")
        break
    
    # i=index2+len(getNumStr(num+1))
    # print("num={},c={}".format(num,content[index1:index2]))
    result_list.append(content[index1+len(getNumStr(num)):index2])
    index1=index2
    num=num+1
   # break

result_list.sort()
for line in result_list:
    print("====={}".format(line))
# print(result_list)