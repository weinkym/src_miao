#!/usr/local/bin/python3
import sys
import os
import datetime
import time
import threading
sys.path.append(os.getcwd())
import subprocess
from zwpy import zwutil
from shutil import copy2
import json

from zwpy import zwutil
from zwpy import zwlog


def test(dp, fp):
    cmd = 'cd \"{}\";clang-format -i \"{}\" -style=file '.format(dp, fp)
    print("cmd={}".format(cmd))
    os.system(cmd)


def test2():
    # dp = '/Users/avc/work/ljlive222/ljobs'
    dp = '/Users/avc/work/srsServer'
    filter_list = ['.cpp', '.h']
    for filter in filter_list:
        file_list = zwutil.getFileNamePaths(dp, filter)
        for fp, name in file_list:
            if 'ljobs/obs/UI' in fp:
                continue
            test(dp, fp)
            # return


def outLine(line):
    index = 0
    for a in line:
        if a == " " or a == "":
            break
        index = index + 1

    print(line[index:-1])


def test3():
    with open('/Users/avc/Documents/code.txt', 'r') as f:
        # while 1:
        line = f.readline()
        while line != "":
            outLine(line)
            line = f.readline()


# fp = '/Users/avc/work/ljlive222/ljobs/document/cwebclouddocitemwidget.cpp'
test2()
