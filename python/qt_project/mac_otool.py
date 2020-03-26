import sys,os
sys.path.append(os.getcwd())
import subprocess
import public_defines as PB
from zwpy import zwutil

g_check_line_valid_list = ["/System/Library/Frameworks/", "/usr/lib/"]

class LineInfo:
    def __init__(self):
        self.valid = False
        self.lib_name = ''
        self.lib_path = ''
    def __str__(self):
        return ('valid={},lib_name={},lib_path={}'.format(self.valid,self.lib_name,self.lib_path))

class PathInfo:
    def __init__(self,fp):
        (file_path,file_name) = os.path.split(fp)
        (shot_name,suffix) = os.path.splitext(file_name)
        self.path = file_path
        self.name = file_name
        self.short_name = shot_name
        self.subffix = suffix
    def __str__(self):
        return ('name={},short_name={},subffix={},path={}'.format(self.subffix,self.name,self.short_name,self.path))

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
    res=zwutil.run_cmd("otool -L \"{}\"".format(fp))
    lineList=res.split("\n")
    dep_list=[]
    print('fp={}'.format(fp))
    for line in lineList[1:]:
        line=line.lstrip()
        if line == '':
            continue
        if isSystemPath(line) and ('ssl' not in line and 'libcrypto' not in line):
            continue
        dep_list.append(parse_otool_line(line))
    return dep_list

def getLibRpaths(fp):
    res=zwutil.run_cmd("otool -l \"{}\" |grep \"LC_RPATH\" -A2".format(fp))
    lineList=res.split("\n")
    obj_list=[]
    # print('fp={}'.format(fp))
    key='path '
    for line in lineList[1:]:
        line=line.lstrip()
        if not line.startswith(key):
            continue
        # print('line={}'.format(line))
        index = line.find(' (offset ')
        if index >= 0:
            obj_list.append(line[len(key):index])
    return obj_list

def deleteRpath(fp,old):
    os.system("install_name_tool -delete_rpath \"{}\" \"{}\"".format(old,fp))

def addRpath(fp,new):
    os.system("install_name_tool -add_rpath \"{}\" \"{}\"".format(new,fp))

def getDefaultRpathList():
    return ['@loader_path/','@executable_path/']

def addDefaultRpath(fp):
    for obj in getDefaultRpathList():
        addRpath(fp,obj)

def resetRpath(fp):
    obj_list = getLibRpaths(fp)
    for obj in obj_list:
        deleteRpath(fp,obj)
    
    addDefaultRpath(fp)

def adjuestRapth(fp,new_list):
    old_list = getLibRpaths(fp)
    exists_list=[]
    for old in old_list:
        if old in new_list:
            exists_list.append(old)
            continue
        deleteRpath(fp,old)
    for new in new_list:
        if new in exists_list:
            continue
        addRpath(fp,new)
                
def changeDPath(fp,old,new):
    cmd="install_name_tool -change \"{}\" \"{}\" \"{}\"".format(old,new,fp)
    print("cmd='{}'".format(cmd))
    os.system(cmd)

def test():
    dp='/Users/miaozw/work/ljlive/ljobs/Release/ljlive.app/Contents/MacOS'
    fp='{}/ljlive'.format(dp)
    # obj_list = getLibDeps(fp)
    # os.system("install_name_tool -add_rpath \"@loader_path/../{}\" \"{}\"".format('Frameworks',fp))
    # for obj in obj_list:
    #     print(obj)
    #     if 'Qt' in obj.lib_path:
    #         continue
    #     new_path='@rpath/{}'.format(obj.lib_name)
    #     if new_path == obj.lib_path:
    #         continue
    #     os.system("install_name_tool -change \"{}\" \"{}\" \"{}\"".format(obj.lib_path,new_path,fp))
    obj_list = getLibRpaths(fp)
    for obj in obj_list:
        print(obj)
# test()