import sys, os
sys.path.append(os.getcwd())
import subprocess
# import public_defines as PB
from zwpy import zwutil

g_check_line_valid_list = ["/System/Library/Frameworks/", "/usr/lib/"]

G_DeveloperIDApplication = "Developer ID Application: Jiangsu Yunxuetang Network Technology  Co.,Ltd (6F8YPTA92C)"
G_DeveloperIDInstaller = "Developer ID Installer: Jiangsu Yunxuetang Network Technology  Co.,Ltd (6F8YPTA92C)"
G_SIGN_PW = "qyeh-djkd-kgbx-ooip"


class LineInfo:
    def __init__(self):
        self.valid = False
        self.lib_name = ''
        self.lib_path = ''

    def __str__(self):
        return ('valid={},lib_name={},lib_path={}'.format(
            self.valid, self.lib_name, self.lib_path))


class PathInfo:
    def __init__(self, fp):
        (file_path, file_name) = os.path.split(fp)
        (shot_name, suffix) = os.path.splitext(file_name)
        self.path = file_path
        self.name = file_name
        self.short_name = shot_name
        self.subffix = suffix

    def __str__(self):
        return ('name={},short_name={},subffix={},path={}'.format(
            self.subffix, self.name, self.short_name, self.path))


def parse_otool_line(line):
    line_info = LineInfo()
    index = line.find(' (compatibility')
    if index >= 0:
        line_info.lib_path = line[0:index]
        line_info.valid = True
        info = PathInfo(line_info.lib_path)
        line_info.lib_name = info.name
    return line_info


def isSystemPath(line):
    for valid in g_check_line_valid_list:
        if line.startswith(valid):
            return True
    return False


def getLibDeps(fp):
    res = zwutil.run_cmd("otool -L \"{}\"".format(fp))
    lineList = res.split("\n")
    dep_list = []
    print('fp={}'.format(fp))
    for line in lineList[1:]:
        line = line.lstrip()
        if line == '':
            continue
        if isSystemPath(line) and ('ssl' not in line
                                   and 'libcrypto' not in line):
            continue
        dep_list.append(parse_otool_line(line))
    return dep_list


def getLibRpaths(fp):
    res = zwutil.run_cmd("otool -l \"{}\" |grep \"LC_RPATH\" -A2".format(fp))
    lineList = res.split("\n")
    obj_list = []
    # print('fp={}'.format(fp))
    key = 'path '
    for line in lineList[1:]:
        line = line.lstrip()
        if not line.startswith(key):
            continue
        # print('line={}'.format(line))
        index = line.find(' (offset ')
        if index >= 0:
            obj_list.append(line[len(key):index])
    return obj_list


def deleteRpath(fp, old):
    os.system("install_name_tool -delete_rpath \"{}\" \"{}\"".format(old, fp))


def addRpath(fp, new):
    os.system("install_name_tool -add_rpath \"{}\" \"{}\"".format(new, fp))


def getDefaultRpathList():
    return ['@loader_path/', '@executable_path/']


def addDefaultRpath(fp):
    for obj in getDefaultRpathList():
        addRpath(fp, obj)


def resetRpath(fp):
    obj_list = getLibRpaths(fp)
    for obj in obj_list:
        deleteRpath(fp, obj)

    addDefaultRpath(fp)


def adjuestRapth(fp, new_list):
    old_list = getLibRpaths(fp)
    exists_list = []
    for old in old_list:
        if old in new_list:
            exists_list.append(old)
            continue
        deleteRpath(fp, old)
    for new in new_list:
        if new in exists_list:
            continue
        addRpath(fp, new)


def changeDPath(fp, old, new):
    cmd = "install_name_tool -change \"{}\" \"{}\" \"{}\"".format(old, new, fp)
    print("cmd='{}'".format(cmd))
    os.system(cmd)


def copyD2D(dp, to_dp):
    if not os.path.exists(dp):
        print('dp={} is not exists'.format(dp))
        return False

    if not os.path.exists(to_dp):
        print('to_dp={} is not exists'.format(to_dp))
        return False

    cmd = 'cp -R \'{}\' \'{}\''.format(dp, to_dp)
    zwutil.run_cmd(cmd)
    return True


def copyDFS2D(dp, to_dp):
    if not os.path.exists(dp):
        print('dp={} is not exists'.format(dp))
        return False

    if not os.path.exists(to_dp):
        print('to_dp={} is not exists'.format(to_dp))
        return False

    cmd = 'cd \'{}\';cp -R ./* \'{}\''.format(dp, to_dp)
    zwutil.run_cmd(cmd)
    return True


def rename(dp, old_name, new_name):
    check_p = joinPathWithRaise(dp, old_name)
    cmd = 'cd \'{}\';mv \'{}\' \'{}\''.format(dp, old_name, new_name)
    zwutil.run_cmd(cmd)
    check_p = joinPathWithRaise(dp, new_name)


def deleteDP(dp, check=False):
    if os.path.exists(dp):
        cmd = "rm -rf \'{}\'".format(dp)
        zwutil.run_cmd(cmd)
    if check and os.path.exists(dp):
        raise Exception("remove {} is error ".format(dp))


def copyF2D(fp, to_dp):
    if not os.path.islink(fp):
        if not os.path.exists(fp):
            print('fp={} is not exists'.format(fp))
            return False

    if not os.path.exists(to_dp):
        print('to_dp={} is not exists'.format(to_dp))
        return False
    cmd = 'cd \'{}\';cp -a \'{}\' ./'.format(to_dp, fp)
    zwutil.run_cmd(cmd)
    return True


def createDs(root_dp, childs_dp):
    dp = os.path.join(root_dp, childs_dp)
    if os.path.exists(dp):
        return dp
    if not os.path.exists(root_dp):
        print('root_dp={} is not exists'.format(root_dp))
        return ''
    cmd = 'cd \'{}\';mkdir - \'{}\''.format(root_dp, childs_dp)
    zwutil.run_cmd(cmd)
    return dp


def getDirname(dp):
    if not os.path.exists(dp):
        print('dp={} is not exists'.format(dp))
        return ""
    p_dp = os.path.dirname(dp)
    index = 1
    if p_dp == '/':
        index = 0
    return dp[len(p_dp) + index:]


def getFiles(dp, subffix_list, ignore_list, deep=True):
    res_list = []
    for obj in subffix_list:
        obj_list = zwutil.getFileNamePaths(dp, obj, deep)
        for fp, name in obj_list:
            ignore = False
            for ig in ignore_list:
                if ig in fp:
                    ignore = True
                    break
            if ignore:
                continue
            res_list.append((fp, name))
    return res_list


def adjuestLibToRpath(fp, exists_name_list):

    obj_list = getLibDeps(fp)
    for obj in obj_list:
        if 'MacOS/XuanLive' in fp:
            print("obj.lib_path={}==============".format(obj.lib_path))

        if '@rpath/Qt' in obj.lib_path:
            continue
        if '@rpath/AgoraRtcKit.framework/AgoraRtcKit' in obj.lib_path:
            continue
        if isSystemPath(obj.lib_path):
            continue
        new = '@rpath/{}'.format(obj.lib_name)
        if new == obj.lib_path:
            continue
        changeDPath(fp, obj.lib_path, new)


def adjuestLibs(dp):
    obj_list = []
    for obj in ['.so', '.dylib']:
        obj_list += zwutil.getFileNamePaths(dp, obj, False)

    exists_name_list = []
    for fp, name in obj_list:
        if not name in exists_name_list:
            exists_name_list.append(name)
        else:
            print("name is repeat========todo")

    for fp, name in obj_list:
        adjuestLibToRpath(fp, exists_name_list)
        adjuestRapth(fp, ['@loader_path/'])


def getCmdFilePath(cmd, check=False):
    res = zwutil.run_cmd('which {}'.format(cmd))
    fp = res.split('\n')[0]
    if check:
        checkPathWithRaise(fp)
    return fp


def checkPathWithRaise(fp):
    if not os.path.exists(fp):
        error_string = 'path not exists \'{}\''.format(fp)
        print('error_string={}'.format(error_string))
        raise Exception(error_string)


def joinPath(parent_dp, child_dp, check=False):
    dp = os.path.join(parent_dp, child_dp)
    if check:
        checkPathWithRaise(dp)
    return dp


def joinPathWithRaise(parent_dp, child_dp):
    dp = os.path.join(parent_dp, child_dp)
    checkPathWithRaise(dp)
    return dp