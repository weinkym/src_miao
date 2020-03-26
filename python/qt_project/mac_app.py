#!/usr/local/bin/python3

import sys,os
sys.path.append(os.getcwd())
import subprocess
from zwpy import zwutil
import mac_otool as OTL

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
def lj_set_rpath(fp,check_name_list):
    obj_list = OTL.getLibDeps(fp)
    for obj in obj_list:
        if '@rpath/Qt' in obj.lib_path:
            continue
        if not obj.lib_name in check_name_list:
            print("obj={} can not found".format(obj))
            return False
        new_path = "@rpath/{}".format(obj.lib_name)
        if new_path == obj.lib_path:
            continue
        OTL.changeDPath(fp,obj.lib_path,new_path)
    return True    

def lj_adjust_lib(dp):
    if not os.path.exists(dp):
        print('dp={} is not exists'.format(dp))
        return False
    lib_subfix_list=['.so','.dylib']
    lib_obj_list=lj_getLibs(dp,lib_subfix_list,'')
    exisit_name_list=[]
    for fp,name in lib_obj_list:
        exisit_name_list.append(name)

    default_rpath_list = OTL.getDefaultRpathList()
    default_rpath_list.append('/Users/miaozw/Qt5.7.1/5.7/clang_64/lib')
    default_rpath_list.append('@executable_path/Frameworks')
    

    for fp,name in lib_obj_list:
        # obj_list = OTL.getLibDeps(fp)
        if not lj_set_rpath(fp,exisit_name_list):
            return False
        rpath_list =OTL.getLibRpaths(fp)
        # for obj in obj_list:
        #     if '/Qt' in obj.lib_path:
        #         continue
        #     if not obj.lib_name in exisit_name_list:
        #         print("obj={} can not found,rpath_list={}".format(obj,rpath_list))
        #         return False
        #     new_path = "@rpath/{}".format(obj.lib_name)
        #     if new_path == obj.lib_path:
        #         continue
        #     OTL.changeDPath(fp,obj.lib_path,new_path)    
        for obj in default_rpath_list:
            if obj not in rpath_list:
                OTL.addRpath(fp,obj)
        # break

def lj_adjuestApp(fp,name_list):
    lj_set_rpath(fp,name_list)
    default_rpath_list = OTL.getDefaultRpathList()
    default_rpath_list.append('/Users/miaozw/Qt5.7.1/5.7/clang_64/lib')
    default_rpath_list.append('@executable_path/../Frameworks')
    rpath_list =OTL.getLibRpaths(fp)
    for obj in default_rpath_list:
        if obj not in rpath_list:
            OTL.addRpath(fp,obj)

def test2():
    dp='/Users/miaozw/build-test_http2-Qt_5_7_1_clang_64-Release/test_http2.app/Contents/Frameworks'
    lib_subfix_list=['.so','.dylib']
    lib_obj_list=lj_getLibs(dp,lib_subfix_list,'')
    for fp,name in lib_obj_list:
        obj_list = OTL.getLibDeps(fp)
        for obj in obj_list:
            if '@rpath/Qt' in obj.lib_path:
                continue
            if 'ssl' not in obj.lib_name and 'crypto' not in obj.lib_name:
                continue
                # print("fp={},obj={}".format(fp,obj))
            new_path = "@loader_path/{}".format(obj.lib_name)
            if new_path == obj.lib_path:
                continue
            OTL.changeDPath(fp,obj.lib_path,new_path)

test2()

def test():
    dep_dp='/Users/miaozw/work/ljlive/vendor'
    app_fp='/Users/miaozw/work/ljlive/bin/mac/release/ljlive.app/Contents/MacOS/ljlive'
    macos_dp=os.path.dirname(app_fp)
    contens_dp=os.path.dirname(macos_dp)
    # frameworks_dp=os.path.join(contens_dp,'Frameworks')
    frameworks_dp = createDs(contens_dp,'Frameworks')
    print('contens_dp={}'.format(contens_dp))
    print('frameworks_dp={}'.format(frameworks_dp))
    # lj_copyLibs(dep_dp,frameworks_dp)
    # lj_adjust_lib(frameworks_dp)

    lib_subfix_list=['.so','.dylib']
    lib_obj_list=lj_getLibs(frameworks_dp,lib_subfix_list,'')
    exisit_name_list=[]
    for fp,name in lib_obj_list:
        exisit_name_list.append(name)

    lj_adjuestApp(app_fp,exisit_name_list)
