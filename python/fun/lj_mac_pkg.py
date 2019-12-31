#!/usr/local/bin/python3

import datetime
import os
import shutil
import subprocess
import sys
from shutil import copy2
sys.path.append(os.getcwd())

from zwpy import zwutil

g_check_line_valid_list = ["/System/Library/Frameworks/", "/usr/lib/"]


class LineInfo:
    valid = False
    lib_name = ''
    lib_path = ''


def is_system_lib_path(lib_fp):
    for valid in g_check_line_valid_list:
        if lib_fp.startswith(valid):
            return True
    return False


def is_qt_lib_path(lib_fp):
    if '/Qt' in lib_fp:
        return True
    return False


def parse_otool_line(line):
    line_info = LineInfo()
    index = line.find(' (compatibility')
    if index >= 0:
        line_info.lib_path = line[0:index]
        line_info.valid = True
        info = zwutil.getPathFileNameInfo(line_info.lib_path)
        # print(info)
        line_info.lib_name = info[1]
        # print(line_info.lib_name)
    return line_info


def check_otool_line(line):
    if not line.startswith("/"):
        return True
    for valid in g_check_line_valid_list:
        if line.startswith(valid):
            return True
    return False


def getCMDLineContent(cmd, row=0):
    return zwutil.run_cmd(cmd).split('\n')[row]


def createPath(dp):
    if not os.path.exists(dp):
        os.makedirs(dp)
    return os.path.exists(dp)


def copyDir(src_dp, dst_dp):
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


def copyDirList(src_dp, dst_dp, dir_name_list):
    for name in dir_name_list:
        if not copyDir('{}/{}'.format(src_dp, name), '{}/{}'.format(dst_dp, name)):
            print('copy is error name={},src_dp={},dst_dp={}'.format(
                name, src_dp, dst_dp))
            return False
    return True

def copyFile(src_fp,dst_fp):
    if not os.path.exists(src_fp):
        print('src_fp is not exists {}'.format(src_fp))
        return False

    if os.path.exists(dst_fp):
        print('dst_fp is exists ,remove {}'.format(dst_fp))
        os.system('rm -f \'{}\''.format(dst_fp))
    shutil.copy(src_fp,dst_fp,follow_symlinks=False)
    return os.path.exists(dst_fp)

def adjuestFileDepends(lib_fp, lib_name, LIB_NAME_DICT):
    if not os.path.exists(lib_fp):
        print('lib_fp is not exists {}'.format(lib_fp))
        return False
    os.system("install_name_tool -id \"{}\" \"{}\"".format(lib_name, lib_fp))
    res = zwutil.run_cmd("otool -L \"{}\"".format(lib_fp))
    lineList = res.split("\n")
    adjust_cmd_list = []
    for line in lineList[1:]:
        if lib_name == 'XuanLive':
            print('line={}'.format(line))
        line = line.lstrip()
        if line == "":
            continue
        info = parse_otool_line(line)
        if not info.valid:
            print("info is invalid lib_fp={},line={}".format(lib_fp, line))
            return False
        if is_system_lib_path(info.lib_path):
            continue
        if is_qt_lib_path(info.lib_path):
            continue
        if info.lib_path == lib_name:
            continue

        
        if not info.lib_name in LIB_NAME_DICT.keys():
            print("lib_name({}) is not in LIB_NAME_DICT lib_fp={},line={}".format(info.lib_name, lib_fp, line))
            return False
        new_fp = LIB_NAME_DICT[info.lib_name]
        if new_fp == info.lib_path:
            continue
        change_cmd = 'install_name_tool -change \"{}\" \"{}\" \"{}\"'.format(
            info.lib_path, new_fp, lib_fp)
        adjust_cmd_list.append(change_cmd)
        # print('line={},change_cmd={}'.format(line,change_cmd))

    if len(adjust_cmd_list) == 0:
        print("no changed lib_fp={},lib_name={}".format(lib_fp,lib_name))
    else:
        print("adjuestFileDepends lib_fp={},lib_name={}".format(lib_fp,lib_name))
    for cmd in adjust_cmd_list:
        print('cmd={}'.format(cmd))
        os.system(cmd)
    return True


C_DATE_TIME_START = datetime.datetime.now()
C_LJ_PROJECT_PATH = "/Users/miaozw/work/ljlive"
C_DEVELOPER_ID = "6F8YPTA92C"
C_PKG_INSTALL_ID = "Developer ID Installer: Jiangsu Yunxuetang Network Technology  Co.,Ltd (6F8YPTA92C)"
C_BUNDLE_EG_NAME = "XuanLive"
C_BUNDLE_CN_NAME = "绚星直播"
C_BUNDLE_AVATAR_NAME = "agora_avatar_push"
C_LIB_SUFFIX_LIST = ['.so', '.dylib']

# C_USER_LOCAT_PATH=zwutil.run_cmd('cd;pwd;').replace('\n','')
C_USER_LOCAT_PATH = getCMDLineContent('cd;pwd;')
C_QTBIN_PATH = "{}/Qt5.7.1/5.7/clang_64/bin".format(C_USER_LOCAT_PATH)
C_QT_MACDEPLOYQT_FILE_PATH = '{}/macdeployqt'.format(C_QTBIN_PATH)

def lj_check_path(path):
    if not os.path.exists(path):
        print('path is not exists {}'.format(path))
        return False
    return True

def lj_create_temp_path(dp):
    if os.path.exists(dp):
        shutil.rmtree(dp)
    if os.path.exists(dp):
        print('{} can not removed'.format(dp))
        return False
    if not createPath(dp):
        print('{} is not exists'.format(dp))
        return False
    return True

def runPkg(project_dp, need_codesign, need_qmake, need_make, need_cef_app):
    if not os.path.exists(project_dp):
        print('{} is not exists'.format(project_dp))
        return False

    OBS_ROOT_DP = '{}/vendor/obs/mac'.format(project_dp)
    OBS_LIB_DP = '{}/vendor/obs/mac/lib/release'.format(project_dp)
    CEF_NAME='Chromium Embedded Framework'
    CEF_FRAMWORK_DP='{}/vendor/cef/mac/Release/{}.framework'.format(project_dp,CEF_NAME)
    C_PAK_BUILD_BIN_FP=getCMDLineContent('which packagesbuild')
    C_PKG_PROJECT_DIR_NAME='pkg'
    C_PKG_PRJOECT_DP='{}/install/mac/{}'.format(project_dp,C_PKG_PROJECT_DIR_NAME)
    app_buil_out_dp = '/Users/miaozw/work/ljlive/release/mac/32bit'
    app_xuanlive_from_dp = '{}/{}.app'.format(app_buil_out_dp, C_BUNDLE_EG_NAME)
    app_avatar_from_dp = '{}/{}.app'.format(app_buil_out_dp, C_BUNDLE_AVATAR_NAME)

    THIRD_DIR_NAME_LIST = ['log4Qt', 'qjson']
    THIRD_DIR_DP_LIST = []
    THIRD_DIR_DP_LIST.append(OBS_LIB_DP)
    for name in THIRD_DIR_NAME_LIST:
        THIRD_DIR_DP_LIST.append('{}/vendor/{}'.format(project_dp, name))

    # T_TEMP_PATH='{}/TEMP_PKG_{}'.format(C_LJ_PROJECT_PATH,C_DATE_TIME_START.strftime('%Y%m%d%I%M%S'))
    T_TEMP_PATH = '{}/TEMP_PKG_{}'.format(C_LJ_PROJECT_PATH,C_DATE_TIME_START.strftime('%Y%m%d'))
    # if not lj_create_temp_path(T_TEMP_PATH):
    #     return False
    # app_xuanlive_to_dp = '{}/{}.app'.format(T_TEMP_PATH, C_BUNDLE_CN_NAME)
    # if not copyDir(app_xuanlive_from_dp,app_xuanlive_to_dp):
    #     return False
    # app_xuanlive_fp='{}/Contents/MacOS/{}'.format(app_xuanlive_to_dp,C_BUNDLE_EG_NAME)
    # if not lj_check_path(app_xuanlive_fp):
    #     return False
    
    # app_avatar_to_dp = '{}/{}.app'.format(T_TEMP_PATH, C_BUNDLE_AVATAR_NAME)
    # app_avatar_fp='{}/Contents/MacOS/{}'.format(app_avatar_to_dp,C_BUNDLE_AVATAR_NAME)
    # app_avatar_to_fp='{}/Contents/MacOS/{}'.format(app_xuanlive_to_dp,C_BUNDLE_AVATAR_NAME)
    # if not copyDir(app_avatar_from_dp,app_avatar_to_dp):
    #     return False

    # zwutil.run_cmd('macdeployqt \'{}\''.format(app_avatar_to_dp))
    # if not copyFile(app_avatar_fp,app_avatar_to_fp):
    #     return False

    # zwutil.run_cmd('macdeployqt \'{}\''.format(app_xuanlive_to_dp))
    # frameworks_to_dp = '{}/Contents/Frameworks'.format(app_xuanlive_to_dp)
    # if not lj_check_path(frameworks_to_dp):
    #     return False
    
    # CEF_FRAMWORK_TO_DP='{}/{}.framework'.format(frameworks_to_dp,CEF_NAME)
    # if not copyDir(CEF_FRAMWORK_DP,CEF_FRAMWORK_TO_DP):
    #     return False

    # os.system('cd \'{}\';xattr -c -r *'.format(frameworks_to_dp))
    # os.system('chmod +x \'{}\''.format(CEF_FRAMWORK_TO_DP))

    # obs_plugins_name='obs-plugins'
    # dir_name_list = ['data', 'ljdata',obs_plugins_name]
    # if not copyDirList(OBS_ROOT_DP,'{}/Contents'.format(app_xuanlive_to_dp),dir_name_list):
    #     return False
    # obs_plugins_to_dp = '{}/Contents/{}'.format(app_xuanlive_to_dp, obs_plugins_name)
    # if not lj_check_path(obs_plugins_to_dp):
    #     return False
    # obs_plugins_list = zwutil.getFileNamePaths(obs_plugins_to_dp,"")
    # LIB_NAME_DICT = {}
    # LIB_NAME_DICT[CEF_NAME]='@executable_path/../Frameworks/{}.framework/{}'.format(CEF_NAME,CEF_NAME)
    # for dp in THIRD_DIR_DP_LIST:
    #     for suffix in C_LIB_SUFFIX_LIST:
    #         obj_list = zwutil.getFileNamePaths(dp, suffix)
    #         for fp, name in obj_list:
    #             if '/debug/' in fp:
    #                 print("{} is debug,ignore".format(fp))
    #                 continue
    #             shutil.copy(fp,frameworks_to_dp,follow_symlinks=False)
    #             LIB_NAME_DICT[name]='@executable_path/../Frameworks/{}'.format(name)

    # for name in LIB_NAME_DICT.keys(): 
    #     if name == CEF_NAME:
    #         continue
    #     lib_fp = '{}/{}'.format(frameworks_to_dp, name)
    #     if not adjuestFileDepends(lib_fp,name,LIB_NAME_DICT):
    #         return False

    # for lib_fp,name in obs_plugins_list:
    #     if not adjuestFileDepends(lib_fp,name,LIB_NAME_DICT):
    #         return False

    # if not adjuestFileDepends(app_xuanlive_fp,C_BUNDLE_EG_NAME,LIB_NAME_DICT):
    #     return False
    # if not adjuestFileDepends(app_avatar_to_fp,C_BUNDLE_AVATAR_NAME,LIB_NAME_DICT):
    #     return False

    
    # C_PKG_PRJOECT_TO_DP='{}/{}'.format(T_TEMP_PATH,C_PKG_PROJECT_DIR_NAME)
    # if not copyDir(C_PKG_PRJOECT_DP,C_PKG_PRJOECT_TO_DP):
    #     return False

    C_PKG_PROJECT_FN = '{}/{}.pkgproj'.format(C_PKG_PRJOECT_TO_DP,C_BUNDLE_EG_NAME)
    os.system('{} --verbose --identity \'{}\' \'{}\''.format(C_PAK_BUILD_BIN_FP,C_PKG_INSTALL_ID,C_PKG_PROJECT_FN))
    return True


def test():
    C_PAK_BIN_FILE_PATH=getCMDLineContent('which packagesbuild')
    print('res={}'.format(C_PAK_BIN_FILE_PATH))
    # THIRD_DIR_NAME_LIST=['log4Qt','qjson']
    # THIRD_DIR_DP_LIST=[]
    # for name in THIRD_DIR_NAME_LIST:
    #     THIRD_DIR_DP_LIST.append('{}/vendor/{}'.format(project_dp,name))
    # for dp in THIRD_DIR_DP_LIST:
    #     obj_list = zwutil.getFileNamePaths(dp,'.dylib')
    #     print(obj_list)

# a=1
# test()
# print(a)


project_dp = '/Users/miaozw/work/ljlive'
need_codesign = True
need_qmake = True
need_make = True
need_cef_app = True
res = runPkg(project_dp, need_codesign, need_qmake, need_make, need_cef_app)
# print('res={}'.format(res))
# test()
