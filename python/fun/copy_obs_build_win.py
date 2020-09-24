
import sys,os
sys.path.append(os.getcwd())

import platform

import subprocess
from zwpy import zwutil
from shutil import copy2

def obs_copy(path,name,to_path):
    des_fp=os.path.join(to_path,name)
    if os.path.exists(des_fp):
        os.remove(des_fp)
    if os.path.exists(des_fp):
        print("file can not delete {}".format(des_fp))
        return False 
    copy2(path,to_path,follow_symlinks=False)
    return True


def copy_build_lib(build_dp,to_root_dp,lib_subffix_list,isX64=True):
    # to_plugins_dp=os.path.join(to_root_dp,"obs-plugins\\32bit\\")
    # to_lib_dp=os.path.join(to_root_dp,"lib\\release\\")
    arch="64" if isX64 else "32"
    to_plugins_dp=os.path.join(to_root_dp,"obs-plugins\\{}bit\\".format(arch))
    from_plugins_dp=os.path.join(build_dp,"rundir\\Release\\obs-plugins\\{}bit\\".format(arch))

    to_data_dp=os.path.join(to_root_dp,"data\\")
    from_data_dp=os.path.join(build_dp,"rundir\\Release\\data")
    
    to_lib_dp=os.path.join(to_root_dp,"{}bit\\release\\".format(arch))
    
    if not os.path.exists(from_plugins_dp):
        print("from_plugins_dp={} is not exists".format(from_plugins_dp))
        return False
    
    if not os.path.exists(from_data_dp):
        print("from_data_dp={} is not exists".format(from_data_dp))
        return False

    if not os.path.exists(to_plugins_dp):
        os.makedirs(to_plugins_dp)

    if not os.path.exists(to_lib_dp):
        os.makedirs(to_lib_dp)

    lib_file_list=[]
    for subffix in lib_subffix_list:
        lib_file_list+=zwutil.getFileNamePaths(build_dp,subffix)

    for path,name in lib_file_list:
        # print(path)
        if "\\rundir\\" in path:
            continue
        if "\\Release\\" not in path:
            continue
        if "\\plugins\\" in path:
            continue
        if "\\frontend-plugins\\" in path:
            continue
        if not obs_copy(path,name,to_lib_dp):
            return False
        print("copy dll lib:name={}".format(name))
    
    cmd='xcopy \"{}*\" \"{}"  /s/e/y'.format(from_plugins_dp,to_plugins_dp)
    print(cmd)
    os.system(cmd)

    cmd='xcopy \"{}\*\" \"{}"  /s/e/y'.format(from_data_dp,to_data_dp)
    print(cmd)
    os.system(cmd)
    return True

print ("Python Version {}".format(str(sys.version).replace('\n', '')))
# exit(0)
copy_build_lib(r"F:\TEMP\build64",r"F:\wook_root\project\ljlive2\bin\win",[".dll",".lib",'obs-browser-page.exe'],True)
copy_build_lib(r"F:\TEMP\build32",r"F:\wook_root\project\ljlive2\bin\win",[".dll",".lib",'obs-browser-page.exe'],False)
# copy_build_lib(r"E:\build-lj2.x-2015x64",r"F:\wook_root\project\ljlive2\bin\win",[".dll",".lib",'obs-browser-page.exe'],True)
