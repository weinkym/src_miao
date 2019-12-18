#!/usr/local/bin/python3

import sys,os
sys.path.append(os.getcwd())
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

def get_invalid_paths(lib_fp):
    invalid_data_list=[]
    res=zwutil.run_cmd("otool -L \"{}\"".format(lib_fp))
    lineList=res.split("\n")
    for line in lineList[1:]:
        if not check_line(line.lstrip()):
            content = line[0:line.index(" (")]
            invalid_data_list.append(content.lstrip())
    return invalid_data_list

def check_lib(lib_fp):
    res=zwutil.run_cmd("otool -L \"{}\"".format(lib_fp))
    lineList=res.split("\n")
    for line in lineList[1:]:
        if not check_line(line.lstrip()):
            print("check is error,path={}\n{},\n{}".format(lib_fp,line,res))
            return False
    return True

def check_lib_list(lib_list,exists_lib_name_list):
    print(exists_lib_name_list)
    for path,name in lib_list:
        if not os.path.exists(path):
            print("{} is not exists".format(path))
            return False
        invalid_data_list=get_invalid_paths(path)
        if len(invalid_data_list) == 0:
            print("change finished lib={}  no changed".format(name))
            continue
        for line in invalid_data_list:
            # (file_path,file_name,shot_name,suffix)
            info = zwutil.getPathFileNameInfo(line)
            file_name=info[1]
            if file_name not in exists_lib_name_list:
                print("{} is no found".format(file_name))
                return False
            print("changed lib={},line={},file={}".format(name,line,file_name))
            os.system("install_name_tool -change \"{}\" \"{}\" \"{}\"".format(line,file_name,path))
        if not check_lib(path):
            print("change error lib={}".format(name))
            return False   
        

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
        exists_lib_fp_list=[]
        ffmpeg_lib_dp=os.path.join(to_root_dp,"ffmpeg/lib/")
        if not os.path.exists(ffmpeg_lib_dp):
            print("{} is not exists".format(ffmpeg_lib_dp))
            return False
        exists_lib_fp_list+=zwutil.getFileNamePaths(ffmpeg_lib_dp,".dylib")
        exists_lib_fp_list+=zwutil.getFileNamePaths(to_lib_dp,".dylib")
        exists_lib_name_list=[]
        for lib_path,lib_name in exists_lib_fp_list:
            os.system("install_name_tool -id \"{}\" \"{}\"".format(lib_name,lib_path))
            exists_lib_name_list.append(lib_name)

        need_adjuest_lib_fp_list=exists_lib_fp_list
        need_adjuest_lib_fp_list+=plugins_list
        if check_lib_list(need_adjuest_lib_fp_list,exists_lib_name_list):
            print("error is happend")
            return False
    return True

g_lib_subffix_list=[".so",".dylib",".a"]
build_dp="/Users/miaozw/Documents/TEMP/build-obs"
to_root_dp="/Users/miaozw/work/ljlive/vendor/obs/mac"
copy_build_lib(build_dp,to_root_dp,g_lib_subffix_list,True)
