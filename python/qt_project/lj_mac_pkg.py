#!/usr/local/bin/python3
import datetime
import os
import shutil
import subprocess
import sys
from shutil import copy2

g_check_line_valid_list = ["/System/Library/Frameworks/", "/usr/lib/"]


class LineInfo:
    valid = False
    lib_name = ''
    lib_path = ''

def getFilePaths(path,suffix):
    fileList=[]
    for parent,dirnames,filenames in os.walk(path,  topdown=False):
        for filename in filenames:
            if filename.endswith(suffix):
                file_path=os.path.join(parent,filename)
                fileList.append(file_path)
                # print(file_path)
    return fileList

def getFileNamePaths(path,suffix):
    fileList=[]
    for parent,dirnames,filenames in os.walk(path,  topdown=False):
        for filename in filenames:
            if filename.endswith(suffix):
                file_path=os.path.join(parent,filename)
                fileList.append((file_path,filename))
                # print(file_path)
    return fileList

def getPathFileNameInfo(path):
    (file_path,file_name) = os.path.split(path)
    (shot_name,suffix) = os.path.splitext(file_name)
    return file_path,file_name,shot_name,suffix

def getDirPaths(path,suffix):
    objList=[]
    for parent,dirnames,filenames in os.walk(path,  topdown=False):
        for name in dirnames:
            if name.endswith(suffix):
                dp=os.path.join(parent,name)
                objList.append(dp)
                # print(dp)
    return objList

def run_cmd(cmd):
    sub = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    sub.wait()
    content=sub.stdout.read().decode("utf-8")
    return content

def outTimeString(pre_str):
    print('{} {}'.format(pre_str,datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')))

def outConsuming(dt):
    print('===={}ms'.format((datetime.datetime.now() - dt).microseconds / 1000.0))

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
        info = getPathFileNameInfo(line_info.lib_path)
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
    return run_cmd(cmd).split('\n')[row]

def getKeyNumber(line,key):
    aduest_line = line.lstrip().replace(' ','')
    print('aduest_line={}'.format(aduest_line))
    res=''
    index = aduest_line.find(key)
    if index > 0:
        i = index+1+len(key)
        while i < len(aduest_line):
            c=aduest_line[i]
            # print('c={}'.format(c))
            if c == ';':
                break
            res = res+c
            i = i + 1
    return res
                


def getVersionInfo(version_fp):
    if not os.path.exists(version_fp):
        return False
    v_major=''
    v_minor=''
    v_patch=''
    v_build=''
    with open(version_fp, 'r') as f:
        line_list = f.readlines()
        print(line_list)
        
        for line in line_list:
            if 'C_VERSION_MAJOR' in line:
                v_major = getKeyNumber(line,'C_VERSION_MAJOR')
            if 'C_VERSION_MINOR' in line:
                v_minor = getKeyNumber(line,'C_VERSION_MINOR')
            if 'C_VERSION_PATCH' in line:
                v_patch = getKeyNumber(line,'C_VERSION_PATCH')
            if 'C_VERSION_BUILD' in line:
                v_build = getKeyNumber(line,'C_VERSION_BUILD')
    return '{}.{}.{}.{}'.format(v_major,v_minor,v_patch,v_build)

def createPath(dp):
    if not os.path.exists(dp):
        os.makedirs(dp)
    return os.path.exists(dp)

def removeDir(dp):
    if os.path.exists(dp):
        shutil.rmtree(dp)
    if os.path.exists(dp):
        print('remove error path={}'.format(dp))
        return False
    return True


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
    res = run_cmd("otool -L \"{}\"".format(lib_fp))
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

# C_USER_LOCAT_PATH=run_cmd('cd;pwd;').replace('\n','')
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

def updateCode(project_dp):
    if not lj_check_path(project_dp):
        return False
    # 用HEAD表示当前版本，上一个版本就是HEAD^，上上一个版本就是HEAD^^，当然往上100个版本写100个^比较容易数不过来，所以写成HEAD~100。
    os.system('cd \'{}\';git checkout -b dev;git reset --hard HEAD;git pull origin dev'.format(project_dp))
    return True

def makeCode(pro_fp,need_qmake):
    if not lj_check_path(pro_fp):
        return False
    info = getPathFileNameInfo(pro_fp)
    pro_dp = info[0]
    pro_name = info[1]
    pro_subffix = info[3]
    print(info)
    if not lj_check_path(pro_dp):
        return False
    if pro_subffix != '.pro':
        return False

    # os.system('qmake \'{}\' -spec macx-clang CONFIG+=x86_64'.format(pro_fp))
    start_dt=datetime.datetime.now()
    outTimeString('make start')
    if need_qmake:
        os.system('cd \'{}\';make clean;qmake \'{}\' -spec macx-clang CONFIG+=x86_64;make clean;make -j 4'.format(pro_dp,pro_name))
    else:
        os.system('cd \'{}\';make -j 4'.format(pro_dp))
    outTimeString('make end')
    outConsuming(start_dt)
    return True


def runPkg(project_dp, need_codesign, need_qmake, need_make,need_update,out_dp,temp_cacha_dp):
    if not os.path.exists(project_dp):
        print('{} is not exists'.format(project_dp))
        return False

    print('log line={}'.format(sys._getframe().f_lineno))
    T_TEMP_PATH = temp_cacha_dp
    # T_TEMP_PATH = '{}/TEMP_PKG_{}'.format(C_LJ_PROJECT_PATH,C_DATE_TIME_START.strftime('%Y%m%d'))
    if not lj_create_temp_path(T_TEMP_PATH):
        return False

    print('log line={}'.format(sys._getframe().f_lineno))
    check_path_list=[]
    # if need_update:
    #     updateCode(project_dp)
    OBS_ROOT_DP = '{}/vendor/obs/mac'.format(project_dp)
    OBS_LIB_DP = '{}/vendor/obs/mac/lib/release'.format(project_dp)
    CEF_NAME='Chromium Embedded Framework'
    CEF_FRAMWORK_DP='{}/vendor/cef/mac/Release/{}.framework'.format(project_dp,CEF_NAME)
    C_PKG_PROJECT_DIR_NAME='pkg'
    C_PKG_PRJOECT_DP='{}/install/mac/{}'.format(project_dp,C_PKG_PROJECT_DIR_NAME)
    C_VERSION_FP='{}/ljobs/base/cversion_mac.cpp'.format(project_dp)

    THIRD_DIR_NAME_LIST = ['log4Qt', 'qjson']
    THIRD_DIR_DP_LIST = []
    THIRD_DIR_DP_LIST.append(OBS_LIB_DP)
    for name in THIRD_DIR_NAME_LIST:
        dp='{}/vendor/{}'.format(project_dp, name)
        THIRD_DIR_DP_LIST.append(dp)
        check_path_list.append(dp)

    check_path_list.append(OBS_ROOT_DP)
    check_path_list.append(OBS_LIB_DP)
    check_path_list.append(CEF_FRAMWORK_DP)
    check_path_list.append(C_PKG_PRJOECT_DP)
    check_path_list.append(C_VERSION_FP)
    C_BIN_NAME_LIST=['packagesbuild','macdeployqt','qmake','make']
    C_VERSION_INFO=getVersionInfo(C_VERSION_FP)
    print('C_VERSION_INFO={}'.format(C_VERSION_INFO))

    for name in C_BIN_NAME_LIST:
        fp=getCMDLineContent('which {}'.format(name))
        check_path_list.append(fp)
    for dp in check_path_list:
        if not lj_check_path(dp):
            return False
    print('log line={}'.format(sys._getframe().f_lineno))
    app_buil_out_dp = '{}/release/mac/32bit'.format(project_dp)
    app_xuanlive_from_dp = '{}/{}.app'.format(app_buil_out_dp, C_BUNDLE_EG_NAME)
    app_avatar_from_dp = '{}/{}.app'.format(app_buil_out_dp, C_BUNDLE_AVATAR_NAME)
    if need_make and False:
        # if not removeDir(app_xuanlive_from_dp) or not removeDir(app_avatar_from_dp):
        #     return False
        xuanlive_pro_fp='{}/ljobs/ljobs.pro'.format(project_dp)
        avatar_pro_fp='{}/ljobs/agora/{}.pro'.format(project_dp,C_BUNDLE_AVATAR_NAME)
        # makeCode(avatar_pro_fp,need_qmake)
        if not makeCode(xuanlive_pro_fp,need_qmake) or not makeCode(avatar_pro_fp,need_qmake):
            return False
            
    print('log line={}'.format(sys._getframe().f_lineno))
    check_path_list=[app_xuanlive_from_dp,app_avatar_from_dp]
    for dp in check_path_list:
        if not lj_check_path(dp):
            return False
    print('log line={}'.format(sys._getframe().f_lineno))

    app_xuanlive_to_dp = '{}/{}.app'.format(T_TEMP_PATH, C_BUNDLE_CN_NAME)
    if not copyDir(app_xuanlive_from_dp,app_xuanlive_to_dp):
        return False
    print('log line={}'.format(sys._getframe().f_lineno))
    app_xuanlive_to_fp='{}/Contents/MacOS/{}'.format(app_xuanlive_to_dp,C_BUNDLE_EG_NAME)
    if not lj_check_path(app_xuanlive_to_fp):
        return False
    print('log line={}'.format(sys._getframe().f_lineno))
    app_avatar_to_dp = '{}/{}.app'.format(T_TEMP_PATH, C_BUNDLE_AVATAR_NAME)
    app_avatar_from_dp = '{}/{}.app'.format(app_buil_out_dp, C_BUNDLE_AVATAR_NAME)
    app_avatar_to_fp='{}/Contents/MacOS/{}'.format(app_xuanlive_to_dp,C_BUNDLE_AVATAR_NAME)
    print('log line={}'.format(sys._getframe().f_lineno))
    if not copyDir(app_avatar_from_dp,app_avatar_to_dp):
        return False
    print('log line={}'.format(sys._getframe().f_lineno))
    app_avatar_from_fp='{}/Contents/MacOS/{}'.format(app_avatar_to_dp,C_BUNDLE_AVATAR_NAME)
    if not lj_check_path(app_avatar_from_fp):
        return False
    print('log line={}'.format(sys._getframe().f_lineno))
    run_cmd('macdeployqt \'{}\''.format(app_avatar_to_dp))
    if not copyFile(app_avatar_from_fp,app_avatar_to_fp):
        return False
    print('log line={}'.format(sys._getframe().f_lineno))
    run_cmd('macdeployqt \'{}\''.format(app_xuanlive_to_dp))
    frameworks_to_dp = '{}/Contents/Frameworks'.format(app_xuanlive_to_dp)
    if not lj_check_path(frameworks_to_dp):
        return False
    print('log line={}'.format(sys._getframe().f_lineno))
    CEF_FRAMWORK_TO_DP='{}/{}.framework'.format(frameworks_to_dp,CEF_NAME)
    if not copyDir(CEF_FRAMWORK_DP,CEF_FRAMWORK_TO_DP):
        return False
    print('log line={}'.format(sys._getframe().f_lineno))
    os.system('cd \'{}\';xattr -c -r *'.format(frameworks_to_dp))
    os.system('chmod +x \'{}\''.format(CEF_FRAMWORK_TO_DP))

    obs_plugins_name='obs-plugins'
    dir_name_list = ['data', 'ljdata',obs_plugins_name]
    if not copyDirList(OBS_ROOT_DP,'{}/Contents'.format(app_xuanlive_to_dp),dir_name_list):
        return False
    print('log line={}'.format(sys._getframe().f_lineno))
    obs_plugins_to_dp = '{}/Contents/{}'.format(app_xuanlive_to_dp, obs_plugins_name)
    if not lj_check_path(obs_plugins_to_dp):
        return False
    print('log line={}'.format(sys._getframe().f_lineno))
    obs_plugins_list = getFileNamePaths(obs_plugins_to_dp,"")
    LIB_NAME_DICT = {}
    LIB_NAME_DICT[CEF_NAME]='@executable_path/../Frameworks/{}.framework/{}'.format(CEF_NAME,CEF_NAME)
    for dp in THIRD_DIR_DP_LIST:
        for suffix in C_LIB_SUFFIX_LIST:
            obj_list = getFileNamePaths(dp, suffix)
            for fp, name in obj_list:
                if '/debug/' in fp:
                    print("{} is debug,ignore".format(fp))
                    continue
                shutil.copy(fp,frameworks_to_dp,follow_symlinks=False)
                LIB_NAME_DICT[name]='@executable_path/../Frameworks/{}'.format(name)

    print('log line={}'.format(sys._getframe().f_lineno))
    for name in LIB_NAME_DICT.keys(): 
        if name == CEF_NAME:
            continue
        lib_fp = '{}/{}'.format(frameworks_to_dp, name)
        if not adjuestFileDepends(lib_fp,name,LIB_NAME_DICT):
            return False
    print('log line={}'.format(sys._getframe().f_lineno))
    for lib_fp,name in obs_plugins_list:
        if not adjuestFileDepends(lib_fp,name,LIB_NAME_DICT):
            return False
    print('log line={}'.format(sys._getframe().f_lineno))
    if not adjuestFileDepends(app_xuanlive_to_fp,C_BUNDLE_EG_NAME,LIB_NAME_DICT):
        return False
    print('log line={}'.format(sys._getframe().f_lineno))
    if not adjuestFileDepends(app_avatar_to_fp,C_BUNDLE_AVATAR_NAME,LIB_NAME_DICT):
        return False
    print('log line={}'.format(sys._getframe().f_lineno))
    
    C_PKG_PRJOECT_TO_DP='{}/{}'.format(T_TEMP_PATH,C_PKG_PROJECT_DIR_NAME)
    if not copyDir(C_PKG_PRJOECT_DP,C_PKG_PRJOECT_TO_DP):
        return False
    print('log line={}'.format(sys._getframe().f_lineno))
    C_PKG_PROJECT_FN = '{}/{}.pkgproj'.format(C_PKG_PRJOECT_TO_DP,C_BUNDLE_EG_NAME)
    os.system('{} --verbose --identity \'{}\' \'{}\''.format('packagesbuild',C_PKG_INSTALL_ID,C_PKG_PROJECT_FN))
    pkg_fp='{}/build/{}.pkg'.format(C_PKG_PRJOECT_TO_DP,C_BUNDLE_EG_NAME)
    if not lj_check_path(pkg_fp):
        return False
    print('log line={}'.format(sys._getframe().f_lineno))
    pkg_to_fp='{}/{}_{}.pkg'.format(out_dp,C_BUNDLE_EG_NAME,C_VERSION_INFO)
    shutil.move(pkg_fp,pkg_to_fp)
    if not lj_check_path(pkg_to_fp):
        return False
    os.system('cd \'{}\';open .'.format(out_dp))
    return True


def start(project_dp,output):
    if not lj_check_path(project_dp) or not lj_check_path(output):
        return False
    temp_dp = '{}/TEMP_RUN_PKG_{}'.format(output,datetime.datetime.now().strftime('%Y%m%d'))
    res = runPkg(project_dp,need_codesign=True,need_qmake=True,need_make=True,need_update=True,out_dp=output,temp_cacha_dp=temp_dp)
    if lj_check_path(temp_dp):
        shutil.rmtree(temp_dp)
    if res:
        print("run pkg is finished")
    else:
        print("run pkg is error")



def test():
    starttime = datetime.datetime.now()
    #long running
    endtime = datetime.datetime.now()
    res = (endtime - starttime).microseconds
    print(res)



project_dp = '/Users/miaozw/work/ljlive'
need_codesign = True
need_qmake = True
need_make = True
need_cef_app = True
# cacha_dp='/Users/miaozw/Documents/TEMP/xuan_pkg'
res = runPkg(project_dp, need_codesign=True, 
need_qmake=True, need_make=True,need_update=True,out_dp='/Users/miaozw/Documents/release',
temp_cacha_dp='/Users/miaozw/Documents/TEMP/xuan_pkg')
# res = updateCode('/Users/miaozw/work/ljlive22')
# res = getVersionInfo('/Users/miaozw/work/ljlive22/ljobs/base/cversion_mac.cpp')
# res = makeCode('/Users/miaozw/work/ljlive22/ljobs/ljobs.pro')
print('res={}'.format(res))
# test()
