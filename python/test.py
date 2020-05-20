#!/usr/local/bin/python3
import sys
import os
import datetime
import time
sys.path.append(os.getcwd())
import subprocess
from zwpy import zwutil
from shutil import copy2
import json

from zwpy import zwutil


def test(fp):
    cmd = 'cd /Users/avc/work/ljlive222/ljobs;clang-format -i \"{}\" -style=file '.format(
        fp)
    os.system(cmd)


def test2():
    dp = '/Users/avc/work/ljlive222/ljobs'
    filter_list = ['.cpp', '.h']
    for filter in filter_list:
        file_list = zwutil.getFileNamePaths(dp, filter)
        for fp, name in file_list:
            if 'ljobs/obs/UI' in fp:
                continue
            test(fp)


fp = '/Users/avc/work/ljlive222/ljobs/document/cwebclouddocitemwidget.cpp'
test2()
