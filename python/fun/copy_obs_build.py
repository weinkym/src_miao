#!/usr/local/bin/python3

import sys,os
import subprocess
from zwpy import zwutil
from shutil import copy2

def copy_build_lib(build_dp,to_root_dp,lib_subffix_list,need_adjuest=False):
    to_plugins_dp=os.path.join(to_root_dp,"obs-plugins/")
    to_lib_dp=os.path.join(to_root_dp,"lib/release/")
    ffmpeg_lib_fp_list=[]
    if need_adjuest:
        ffmpeg_lib_dp=os.path.join(to_root_dp,"ffmpeg/lib/")
        if os.path.exists(ffmpeg_lib_dp):
            print("{} is not exists".format(ffmpeg_lib_dp))
            return False
        ffmpeg_lib_fp_list+=zwutil.getFilePaths(ffmpeg_lib_dp,".dylib")

    if not os.path.exists(to_plugins_dp):
        os.makedirs(to_plugins_dp)

    if not os.path.exists(to_lib_dp):
        os.makedirs(to_lib_dp)

    lib_file_list=[]
    for subffix in lib_subffix_list:
        lib_file_list+=zwutil.getFilePaths(build_dp,subffix)

    for file in lib_file_list:
        if "/rundir/" in file:
            continue
        if "/Release/" not in file:
            continue
        if "/plugins/" in file:
            copy2(file,to_plugins_dp,follow_symlinks=False)
        else: 
            copy2(file,to_lib_dp,follow_symlinks=False)
    return True

g_lib_subffix_list=[".so",".dylib",".a"]
build_dp="/Users/miaozw/Documents/TEMP/build-obs"
to_root_dp="/Users/miaozw/Documents/TEMP/test_obs"
# copy_build_lib(build_dp,to_root_dp,g_lib_subffix_list,False)
