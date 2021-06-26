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

from pdf2docx import Converter


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
    fp_pdf='/Users/miao/Documents/WinDbg中文手册.pdf'
    fp_word='/Users/miao/Documents/WinDbg中文手册.docx'
    cv = Converter(fp_pdf)
    cv.convert(fp_word, start=0, end=None)
    cv.close()


# fp = '/Users/avc/work/ljlive222/ljobs/document/cwebclouddocitemwidget.cpp'
test3()
