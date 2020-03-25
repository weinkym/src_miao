#!/usr/local/bin/python3

import sys,os
sys.path.append(os.getcwd())
import subprocess
from zwpy import zwutil

def copyD2D(dp,to_dp):
    if not os.path.exists(dp):
        print('dp={} is not exists'.format(dp))
        return False

    if not os.path.exists(to_dp):
        print('to_dp={} is not exists'.format(to_dp))
        return False

    cmd='cp -R \'{}\' \'{}\''.format(dp,to_dp)
    zwutil.run_cmd(cmd)
    return True
    
def copyDFS2D(dp,to_dp):
    if not os.path.exists(dp):
        print('dp={} is not exists'.format(dp))
        return False

    if not os.path.exists(to_dp):
        print('to_dp={} is not exists'.format(to_dp))
        return False

    cmd='cd \'{}\';cp -R ./* \'{}\''.format(dp,to_dp)
    zwutil.run_cmd(cmd)
    return True

def copyF2D(fp,to_dp):
    if not os.path.islink(fp):
        if not os.path.exists(fp):
            print('fp={} is not exists'.format(fp))
            return False

    if not os.path.exists(to_dp):
        print('to_dp={} is not exists'.format(to_dp))
        return False
    cmd='cd \'{}\';cp -a \'{}\' ./'.format(to_dp,fp)
    zwutil.run_cmd(cmd)
    return True

def createDs(root_dp,childs_dp):
    dp=os.path.join(root_dp,childs_dp)
    if os.path.exists(dp):
        return dp
    if not os.path.exists(root_dp):
        print('root_dp={} is not exists'.format(root_dp))
        return ''
    cmd='cd \'{}\';mkdir - \'{}\''.format(root_dp,childs_dp)
    zwutil.run_cmd(cmd)
    return dp
def getDirname(dp):
    if not os.path.exists(dp):
        print('dp={} is not exists'.format(dp))
        return ""
    p_dp=os.path.dirname(dp)
    index=1
    if p_dp == '/':
        index=0
    return dp[len(p_dp)+index:]
#===============
def lj_getLibs(dp,lib_subfix_list,ignore_list):
    lib_obj_list=[]
    for subfix in lib_subfix_list:
        lib_obj_list += zwutil.getFileNamePaths(dp,subfix)
    # ignore_list=['/debug/','/obs-plugins/']
    obj_list=[]
    for fp,name in lib_obj_list:
        is_valid=True
        for ignore in ignore_list:
            if ignore in fp:
                is_valid = False
                break 
        if not is_valid:
            continue
        obj_list.append((fp,name))
    return obj_list

def lj_copyLibs(dp,frameworks_dp):
    lib_subfix_list=['.so','.dylib']
    ignore_list=['/debug/','/obs-plugins/']
    lib_obj_list=lj_getLibs(dep_dp,lib_subfix_list,ignore_list)
    for fp,name in lib_obj_list:
        print('name={}'.format(fp))
        copyF2D(fp,frameworks_dp)

#===============
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
        if lj_is_qt(line):
            continue
        
        info = parse_otool_line(line)
        # if info.lib_path == info.lib_name:
        #     continue
        if 'Qt' in info.lib_name:
            continue

        cmd = 'install_name_tool -change \"{}\" \"@loader_path/{}\" \"{}\"'.format(
            info.lib_path, info.lib_name, fp)
        cmd_list.append(cmd)
        print('lib_path={},line={}'.format(info.lib_path,line))

    for cmd in cmd_list:
        print('cmd={}'.format(cmd))
        # os.system(cmd)

    # print(lineList)



def lj_adjust_lib(dp,app_fp):
    if not os.path.exists(dp):
        print('dp={} is not exists'.format(dp))
        return False
    if not os.path.exists(app_fp):
        print('app_fp={} is not exists'.format(app_fp))
        return False
    lib_subfix_list=['.so','.dylib']
    lib_obj_list=lj_getLibs(dp,lib_subfix_list,'')
    for fp,name in lib_obj_list:
        print('name={}'.format(fp))
        adjuest_lib(fp)
        break

dep_dp='/Users/miaozw/work/ljlive/vendor'
app_fp='/Users/miaozw/work/ljlive/ljobs/Release/ljlive.app/Contents/MacOS/ljlive'
contens_dp=os.path.dirname(app_fp)
# frameworks_dp=os.path.join(contens_dp,'Frameworks')
frameworks_dp = createDs(contens_dp,'Frameworks')
lj_adjust_lib(frameworks_dp,app_fp)
# lj_copyLibs(dep_dp,frameworks_dp)

# lib_subfix_list=['.so','.dylib']
# ignore_list=['/debug/','/obs-plugins/']

# lib_obj_list=lj_getLibs(dep_dp,lib_subfix_list,ignore_list)
# for fp,name in lib_obj_list:
#     print('name={}'.format(fp))
#     copyF2D(fp,frameworks_dp)
# dep_dp='/Users/miaozw/work/ljlive/vendor/qjson/lib/mac/release'

# for subfix in lib_subfix_list:
#     lib_obj_list += zwutil.getFileNamePaths(dep_dp,subfix)
#     print('name={}'.format(fp))
#     copyF2D(fp,frameworks_dp)

# for fp,name in lib_obj_list:
#     is_valid=True
#     for ignore in ignore_list:
#         if ignore in fp:
#             is_valid = False
#             break 
#     if not is_valid:
#         continue
#     print('name={}'.format(fp))
#     copyF2D(fp,frameworks_dp)
# print('contens_dp={}'.format(getDirname(contens_dp)))
# print('contens_dp={}'.format(getDirname('/Users')))
# print('frameworks_dp={}'.format(frameworks_dp))
# copyF2D('/Users/miaozw/Documents/TEMP/test-py/dp/lib/release/libx264.dylib','/Users/miaozw/Documents/TEMP/test-py/to_dp')
