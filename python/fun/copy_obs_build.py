#!/usr/local/bin/python3

import sys,os
sys.path.append(os.getcwd())
import subprocess
from zwpy import zwutil
import shutil
from shutil import copy2

g_check_line_valid_list=["/System/Library/Frameworks/","/usr/lib/"]

class LineInfo:
    valid = False
    lib_name = ''
    lib_path = ''

def getPathFileNameInfo(path):
    (file_path,file_name) = os.path.split(path)
    (shot_name,suffix) = os.path.splitext(file_name)
    return file_path,file_name,shot_name,suffix

def parse_otool_line(line):
    line_info = LineInfo()
    index = line.find(' (compatibility')
    if index >= 0:
        line_info.lib_path = line[0:index]
        line_info.valid = True
        info = getPathFileNameInfo(line_info.lib_path)
        # print(info)
        line_info.lib_name = info[1]
        # print(line_info.lib_name)
    return line_info

def lj_is_system(line):
    for valid in g_check_line_valid_list:
        if line.startswith(valid):
            return True
    return False

def lj_is_qt(line):
    if line.startswith('@rpath/Qt'):
        return True
    return False

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
    # print(exists_lib_name_list)
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
            # if file_name not in exists_lib_name_list:
            #     print("{} is no found".format(file_name))
            #     return False
            print("changed lib={},line={},file={}".format(name,line,file_name))
            os.system("install_name_tool -change \"{}\" \"{}\" \"{}\"".format(line,file_name,path))
        if not check_lib(path):
            print("change error lib={}".format(name))
            return False   
        
def adjuest_lib(fp):
    info = zwutil.getPathFileNameInfo(fp)
    lib_name=info[1]
    os.system("install_name_tool -id \"{}\" \"{}\"".format(lib_name, fp))

    res=zwutil.run_cmd("otool -L \"{}\"".format(fp))
    lineList=res.split("\n")
    cmd_list=[]
    print('fp={}'.format(fp))
    for line in lineList[1:]:
        line=line.lstrip()
        if line == '':
            continue
        if lj_is_system(line):
            continue
        if lj_is_system(line):
            continue
        
        info = parse_otool_line(line)
        if info.lib_path == info.lib_path:
            continue
        cmd = 'install_name_tool -change \"{}\" \"{}\" \"{}\"'.format(
            info.lib_path, info.lib_name, fp)
        cmd_list.append(cmd)
        print('lib_path={},line={}'.format(info.lib_path,line))

    for cmd in cmd_list:
        print('cmd={}'.format(cmd))
        os.system(cmd)

    # print(lineList)


def obs_copy(path,name,to_path):
    des_fp=os.path.join(to_path,name)
    if os.path.exists(des_fp):
        os.remove(des_fp)
    if os.path.exists(des_fp):
        print("file can not delete {}".format(des_fp))
        return False 
    copy2(path,to_path,follow_symlinks=False)
    return True

def copyDir2(src_dp,dst_dp):
    if not os.path.exists(src_dp):
        print('src_dp is not exists {}'.format(src_dp))
        return False
    if os.path.exists(dst_dp):
        print('dst_dp is exists ,remove {}'.format(dst_dp))
        shutil.rmtree(dst_dp)

    shutil.copytree(src_dp, dst_dp)
    if not os.path.exists(dst_dp):
        print('dst_dp is not exists {}'.format(dst_dp))
        return False
    return True

def copyDir(from_dp, to_dp,dir_name):
    src_dp='{}/{}'.format(from_dp,dir_name)
    dst_dp='{}/{}'.format(to_dp,dir_name)
    return copyDir2(src_dp,dst_dp)
    

def copy_build_lib(build_dp,to_root_dp,lib_subffix_list,need_adjuest=False):
    to_plugins_dp=os.path.join(to_root_dp,"obs-plugins")
    # to_lib_dp=os.path.join(to_root_dp,"lib/release")
    to_lib_dp = '{}/lib/release'.format(to_root_dp)
    from_release_dp = '{}/rundir/Release'.format(build_dp)
    print('log line={}'.format(sys._getframe().f_lineno))
    print('from_release_dp={}'.format(from_release_dp))
    print('to_lib_dp={}'.format(to_lib_dp))
    print('to_root_dp={}'.format(to_root_dp))

    # if not copyDir(from_release_dp,to_root_dp,'obs-plugins'):
    #     return False
    # print('log line={}'.format(sys._getframe().f_lineno))
    # if not copyDir(from_release_dp,to_root_dp,'data'):
    #     return False
    print('log line={}'.format(sys._getframe().f_lineno))
    if not os.path.exists(to_lib_dp):
        os.makedirs(to_lib_dp)
    print('log line={}'.format(sys._getframe().f_lineno))
    lib_file_list=[]
    plugins_list=[]
    for subffix in lib_subffix_list:
        lib_file_list+=zwutil.getFileNamePaths(build_dp,subffix)
        plugins_list += zwutil.getFileNamePaths(to_plugins_dp,subffix)

    lib_fp_list=[]
    for path,name in lib_file_list:
        if "/rundir/" in path:
            continue
        if "/Release/" not in path:
            continue
        if "/plugins/" in path:
            continue
        else: 
            # if not obs_copy(path,name,to_lib_dp):
            #     return False
            if not name.endswith(".a"):
                lib_fp_list.append(os.path.join(to_lib_dp,name))
    
    # print(lib_fp_list)
    print('log line={}'.format(sys._getframe().f_lineno))
    if need_adjuest:
        for fp in lib_fp_list:
            # print('fp={}'.format(fp))
            adjuest_lib(fp)
        for fp,name in plugins_list:
            # print('fp={}'.format(fp))
            adjuest_lib(fp)
    return True

def test():
    test_from_dp='/Users/miaozw/Documents/TEMP/build-obs/rundir/Release'
    test_to_dp='/Users/miaozw/Documents/TEMP/test_copy'
    copyDir(test_from_dp,test_to_dp,'data')
    copyDir(test_from_dp,test_to_dp,'data')

g_lib_subffix_list=[".so",".dylib",".a"]
build_dp="/Users/miaozw/Documents/TEMP/build-obs"
to_root_dp="/Users/miaozw/work/ljlive/vendor/obs/mac"
copy_build_lib(build_dp,to_root_dp,g_lib_subffix_list,True)
# test()
