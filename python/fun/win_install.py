import datetime
import os
import shutil
import subprocess
import sys
from shutil import copy2

g_check_line_valid_list = ["\\System\\Library\\Frameworks\\", "\\usr\\lib\\"]


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

def run_cmd_out(cmd):
    popen = subprocess.Popen(cmd,
                         stdout = subprocess.PIPE,
                         stderr = subprocess.PIPE,
                         bufsize=1)

    # 重定向标准输出
    while popen.poll() is None:         # None表示正在执行中
        r = popen.stdout.readline().decode('gbk')
        sys.stdout.write(r)                    # 可修改输出方式，比如控制台、文件等

    # 重定向错误输出
    if popen.poll() != 0:                      # 不为0表示执行错误
        err = popen.stderr.read().decode('gbk')
        sys.stdout.write(err)                 # 可修改输出方式，比如控制台、文件等



def run_cmd(cmd):
    sub = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    sub.wait()
    # content=sub.stdout.read()
    # content=sub.stdout.read().decode("utf-8")
    content=sub.stdout.read().decode("gbk")
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
    if '\\Qt' in lib_fp:
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
    if not line.startswith("\\"):
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
    with open(version_fp, 'r',encoding='utf-8') as f:
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

def removeFile(fp):
    if os.path.exists(fp):
        os.remove(fp)
    if os.path.exists(fp):
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
        if not copyDir('{}\\{}'.format(src_dp, name), '{}\\{}'.format(dst_dp, name)):
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
        os.remove(dst_fp)
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
C_LJ_PROJECT_PATH = "\\Users\\miaozw\\work\\ljlive"
C_DEVELOPER_ID = "6F8YPTA92C"
C_PKG_INSTALL_ID = "Developer ID Installer: Jiangsu Yunxuetang Network Technology  Co.,Ltd (6F8YPTA92C)"
C_BUNDLE_EG_NAME = "XuanLive"
C_BUNDLE_CN_NAME = "绚星直播"
C_BUNDLE_AVATAR_NAME = "agora_avatar_push"
C_LIB_SUFFIX_LIST = ['.so', '.dylib']

# C_USER_LOCAT_PATH=run_cmd('cd;pwd;').replace('\n','')
C_USER_LOCAT_PATH = getCMDLineContent('cd;pwd;')
C_QTBIN_PATH = "{}\\Qt5.7.1\\5.7\\clang_64\\bin".format(C_USER_LOCAT_PATH)
C_QT_MACDEPLOYQT_FILE_PATH = '{}\\macdeployqt'.format(C_QTBIN_PATH)

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

def makeCode(pro_fp,need_qmake,make_bat_fp,out_exe_fp):
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

    C_QT_DIR=os.path.expandvars('$QTDIR')
    print('log C_QT_DIR={}'.format(C_QT_DIR))
    if not lj_check_path(C_QT_DIR):
        print('set QTDIR environment')
        return False
    C_VSBAT_FP=r"c:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"
    print('log line={}'.format(sys._getframe().f_lineno))
    check_path_list=[make_bat_fp,C_QT_DIR,C_VSBAT_FP]
    for dp in check_path_list:
        if not lj_check_path(dp):
            return False
    start_dt=datetime.datetime.now()
    outTimeString('make start')
    cmd='\"{}\" \"{}\" \"{}\" \"{}\" \"{}\" \"{}\"'.format(make_bat_fp,pro_dp,pro_name,out_exe_fp,C_VSBAT_FP,C_QT_DIR)
    # cmd='\"F:\\wook_root\\project\\ljlive\\install\\win\make.bat\" \'{}\' \'{}\' \'{}\' \'{}\' \'{}\''.format(pro_dp,pro_name,EXE_FP,"c:\\Program Files (x86)\\Microsoft Visual Studio 14.0\\VC",C_QT_DIR)
    print('cmd={}'.format(cmd)) 
    run_cmd_out(cmd)
    # os.system(cmd)
    outTimeString('make end')
    outConsuming(start_dt)
    return True


def runPkg(project_dp, need_codesign, need_qmake, need_make,need_update,out_dp,temp_cacha_dp):
    if not os.path.exists(project_dp):
        print('{} is not exists'.format(project_dp))
        return False

    check_path_list=[]

    print('log line={}'.format(sys._getframe().f_lineno))
    T_TEMP_PATH = temp_cacha_dp
    # T_TEMP_PATH = '{}\\TEMP_PKG_{}'.format(C_LJ_PROJECT_PATH,C_DATE_TIME_START.strftime('%Y%m%d'))
    # if not lj_create_temp_path(T_TEMP_PATH):
        # return False
    T_TEMP_BIN_DP='{}\\win\\32bit'.format(T_TEMP_PATH)
    # if not lj_create_temp_path(T_TEMP_BIN_DP):
        # return False

    if need_update:
        updateCode(project_dp)
    OBS_ROOT_DP = '{}\\vendor\\obs\\win'.format(project_dp)
    OBS_LIB_DP = '{}\\vendor\\obs\\win\\lib\\release'.format(project_dp)
    CEF_ROOT_DP='{}\\vendor\\cef\\win'.format(project_dp)
    CEF_RELEASE_DP='{}\\vendor\\cef\\win\\Release'.format(project_dp)
    C_VERSION_FP='{}\\ljobs\\base\\cversion_win.cpp'.format(project_dp)

    BASE_LIB_DP='{}\\install\\win\\baselib'.format(project_dp)
    
    NSIS_FILE_NAME='setup.nsi'
    NSIS_SCRIPT_FP='{}\\install\\win\\{}'.format(project_dp,NSIS_FILE_NAME)
    NSIS_BIN_FP='{}\\install\\win\\NSIS\\makensis.exe'.format(project_dp)
    SIGN_CERTIFICATE_FP="{}\\install\\win\\codesign\\Yunxuetang.pfx".format(project_dp)
    SIGN_TOOP_FP="{}\\install\\win\\codesign\\tools\\SignBatch\\tool\\signtool.exe".format(project_dp)


    THIRD_DIR_NAME_LIST = ['log4Qt', 'qjson']
    THIRD_DIR_DP_LIST = []
    THIRD_DIR_DP_LIST.append(OBS_LIB_DP)
    for name in THIRD_DIR_NAME_LIST:
        dp='{}\\vendor\\{}'.format(project_dp, name)
        THIRD_DIR_DP_LIST.append(dp)
        check_path_list.append(dp)

    check_path_list.append(OBS_ROOT_DP)
    check_path_list.append(OBS_LIB_DP)
    check_path_list.append(CEF_ROOT_DP)
    check_path_list.append(CEF_RELEASE_DP)
    check_path_list.append(C_VERSION_FP)
    check_path_list.append(NSIS_SCRIPT_FP)
    check_path_list.append(NSIS_BIN_FP)
    check_path_list.append(SIGN_CERTIFICATE_FP)
    check_path_list.append(SIGN_TOOP_FP)


    C_VERSION_INFO=getVersionInfo(C_VERSION_FP)
    print('C_VERSION_INFO={}'.format(C_VERSION_INFO))
    
    for dp in check_path_list:
        if not lj_check_path(dp):
            return False
    print('log line={}'.format(sys._getframe().f_lineno))
    app_buil_out_dp = '{}\\release\\win\\32bit'.format(project_dp)
    app_xuanlive_from_fp = '{}\\{}.exe'.format(app_buil_out_dp, C_BUNDLE_EG_NAME)
    app_avatar_from_fp = '{}\\{}.exe'.format(app_buil_out_dp, C_BUNDLE_AVATAR_NAME)
    if need_make and True:
        # if not removeFile(app_xuanlive_from_fp) or not removeFile(app_avatar_from_fp):
            # return False
        xuanlive_pro_fp='{}\\ljobs\\ljobs.pro'.format(project_dp)
        avatar_pro_fp='{}\\ljobs\\agora\\{}.pro'.format(project_dp,C_BUNDLE_AVATAR_NAME)
        make_bat_fp='{}\\install\\win\\make.bat'.format(project_dp)
        # makeCode(avatar_pro_fp,need_qmake,make_bat_fp,app_avatar_from_fp)
        # if not makeCode(xuanlive_pro_fp,need_qmake) or not makeCode(avatar_pro_fp,need_qmake):
            # return False
            
    print('log line={}'.format(sys._getframe().f_lineno))
    check_path_list=[app_xuanlive_from_fp,app_avatar_from_fp]
    for dp in check_path_list:
        if not lj_check_path(dp):
            return False
    print('log line={}'.format(sys._getframe().f_lineno))

    app_xuanlive_to_fp = '{}\\{}.exe'.format(T_TEMP_BIN_DP, C_BUNDLE_EG_NAME)
    if not copyFile(app_xuanlive_from_fp,app_xuanlive_to_fp):
        return False
    print('log line={}'.format(sys._getframe().f_lineno))
    app_avatar_to_fp='{}\\{}.exe'.format(T_TEMP_BIN_DP,C_BUNDLE_AVATAR_NAME)
    if not copyFile(app_avatar_from_fp,app_avatar_to_fp):
        return False
  
    NSIS_SCRIPT_TO_FP = '{}/{}'.format(T_TEMP_PATH,NSIS_FILE_NAME)
    if not copyFile(NSIS_SCRIPT_FP,NSIS_SCRIPT_TO_FP):
        return False
    dir_name_list = ['data', 'ljdata','obs-plugins']
    # if not copyDirList(OBS_ROOT_DP,T_TEMP_PATH,dir_name_list):
    #     return False

    # if not copyDir(BASE_LIB_DP,T_TEMP_BIN_DP):
    #      return False

    cmd='\"{0}\" \"/DPRODUCT_VERSION={1}\" \"/DPRODUCT_EG_NAME={2}\" \"/DPRODUCT_CH_NAME={3}\" \"/DPRODUCT_OUTPUT_PATH={4}\" \"{5}\" '.format(
        NSIS_BIN_FP,C_VERSION_INFO,C_BUNDLE_EG_NAME,C_BUNDLE_CN_NAME,out_dp,NSIS_SCRIPT_TO_FP)
    run_cmd_out(cmd)

    # call %P_NSIS_EXE_FP% /DPRODUCT_VERSION=%C_PRODUCT_VERSION% /DPRODUCT_EG_NAME=%C_PRODUCT_EG_NAME% /DPRODUCT_CH_NAME=%C_PRODUCT_CH_NAME% /DPRODUCT_OUTPUT_PATH=%C_OUT_DP% %P_RUN_SCRIPT_PATH% 





   
   
    return True


def start(project_dp,output):
    if not lj_check_path(project_dp) or not lj_check_path(output):
        return False
    temp_dp = '{}\\TEMP_RUN_PKG_{}'.format(output,datetime.datetime.now().strftime('%Y%m%d'))

    res = runPkg(project_dp,need_codesign=True,need_qmake=True,need_make=True,need_update=True,out_dp=output,temp_cacha_dp=temp_dp)
    # if lj_check_path(temp_dp):
    #     shutil.rmtree(temp_dp)
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



def p(f):
    print('{}.{}: {}'.format(f.__module__, f.__name__, f()))


if __name__ == '__main__':
    p(sys.getdefaultencoding)
    # file_dp=os.path.dirname(os.path.realpath(__file__))
    file_dp=r'f:\wook_root\project\ljlive\install\win'
    project_dp=os.path.abspath(os.path.join(file_dp, "..\\.."))
    # output_dp=os.path.join(os.path.expanduser('~'),'Desktop')
    output_dp=os.path.join('F:')
    print('project_dp={},output_dp={}'.format(project_dp,output_dp))
    start(project_dp,output_dp)
    # print (os.path.expandvars('$QTDIR2'))
    