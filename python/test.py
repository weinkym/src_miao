#!/usr/local/bin/python3

import sys,os
import subprocess
from zwpy import zwutil
from shutil import copy2

g_check_line_valid_list=["/System/Library/Frameworks/","/usr/lib/"]
def check_line(line):
    if not line.startswith("/"):
        return True
    for valid in g_check_line_valid_list:
        if line.startswith(valid):
            return True
    return False
def check_lib(lib_fp):
    res=zwutil.run_cmd("otool -L \"{}\"".format(lib_fp))
    lineList=res.split("\n")
    for line in lineList[1:]:
        if not check_line(line.lstrip()):
            print("check is error,path={}\n{},\n{}".format(lib_fp,line,res))
            return False
    return True

def obs_copy(path,name,to_path):
    des_fp=os.path.join(to_path,name)
    if os.path.exists(des_fp):
        os.remove(des_fp)
    if os.path.exists(des_fp):
        print("file can not delete {}".format(des_fp))
        return False 
    copy2(path,to_path,follow_symlinks=False)
    return True

def copy_build_lib(build_dp,to_root_dp,lib_subffix_list,need_adjuest=False):
    to_plugins_dp=os.path.join(to_root_dp,"obs-plugins/")
    to_lib_dp=os.path.join(to_root_dp,"lib/release/")
    ffmpeg_lib_fp_list=[]
    if need_adjuest:
        ffmpeg_lib_dp=os.path.join(to_root_dp,"ffmpeg/lib/")
        if not os.path.exists(ffmpeg_lib_dp):
            print("{} is not exists".format(ffmpeg_lib_dp))
            return False
        ffmpeg_lib_fp_list+=zwutil.getFileNamePaths(ffmpeg_lib_dp,".dylib")

    if not os.path.exists(to_plugins_dp):
        os.makedirs(to_plugins_dp)

    if not os.path.exists(to_lib_dp):
        os.makedirs(to_lib_dp)

    lib_file_list=[]
    for subffix in lib_subffix_list:
        lib_file_list+=zwutil.getFileNamePaths(build_dp,subffix)

    plugins_list=[]
    lib_list=[]
    for path,name in lib_file_list:
        if "/rundir/" in path:
            continue
        if "/Release/" not in path:
            continue
        if "/plugins/" in path:
            if not obs_copy(path,name,to_plugins_dp):
                return False
            plugins_list.append((path,name))
        else: 
            if not obs_copy(path,name,to_lib_dp):
                return False
            if not name.endswith(".a"):
                lib_list.append((path,name))
    if need_adjuest:
        replace_path_list=["/Users/miaozw/Downloads/ffmpeg-3.4.7/build/lib"]
        for p_path,p_name in plugins_list:
            des_fp=os.path.join(to_plugins_dp,p_name)
            if not os.path.exists(des_fp):
                print("{} is not exists".format(des_fp))
                return False
            for rp in replace_path_list:
                for lib_path,lib_name in ffmpeg_lib_fp_list:
                    replace_path=os.path.join(rp,lib_name)
                    print("lib_name{}".format(lib_name))
                    os.system("install_name_tool -change \"{}\" \"{}\" \"{}\"".format(replace_path,lib_name,des_fp))
                    # zwutil.run_cmd("")
            if not check_lib(des_fp):
                print("change error lib={}".format(p_name))
                return False
            print("change finished lib={}".format(p_name))
    return True

g_lib_subffix_list=[".so",".dylib",".a"]
build_dp="/Users/miaozw/Documents/TEMP/build-obs"
to_root_dp="/Users/miaozw/Documents/TEMP/test_obs"
copy_build_lib(build_dp,to_root_dp,g_lib_subffix_list,True)
