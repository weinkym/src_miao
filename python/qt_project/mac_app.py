#!/usr/local/bin/python3

import sys,os
sys.path.append(os.getcwd())
import subprocess
from zwpy import zwutil
import mac_utils as OTL
import biplist

G_IDENT_APP='Developer ID Application: Jiangsu Yunxuetang Network Technology  Co.,Ltd (6F8YPTA92C)'
G_IDENT_INSTALL=''

class VersionInfo:
    def __init__(self):
        self.major = 0
        self.minor = 0
        self.patch = 0
        self.build = 0
    def __str__(self):
        return self.fullVersion()
    def fullVersion(self):
        return '{}.{}.{}.{}'.format(self.major,self.minor,self.patch,self.build)
    def shortVersion(self):
        return '{}.{}.{}'.format(self.major,self.minor,self.patch)
        
def codesignFile(fp):
    entitlements_fp='/Users/avc/Documents/TEMP/TEMP/ljlive.app/Contents/ljlive.entitlements'
    cmd='codesign -s \"{}\" --options \"runtime\" \"{}\"'.format(G_IDENT_APP,fp)
    if(fp.endswith('.app')):
        cmd='codesign -f -o runtime --entitlements \"{}\" --timestamp --deep -s "{}" -i "cn.yunxuetang.ljlivepc" \"{}\"'.format(entitlements_fp,G_IDENT_APP,fp)
    
    print('cmd={}'.format(cmd))
    res=zwutil.run_cmd(cmd)
    print('res={}'.format(res))

def codesignApp(app_dp):
    framework_name='.framework'
    ignore_suffix_list=['.effect','.ini','.png','.qss','.txt','.icns','.conf','.lproj','.pcm','.inc','.json','.py','.pyc']
    obj_list=[]
    for parent,dirnames,filenames in os.walk(app_dp,  topdown=False):
        if framework_name in parent:
            for name in dirnames:
                if name.endswith('.app'):
                    codesignApp(os.path.join(parent,name))
            continue
        
        for name in dirnames:
            if name.endswith(framework_name):
                fp=os.path.join(parent,name)
                obj_list.append((fp,name))
            

        for name in filenames:
            ignore=False
            for obj in ignore_suffix_list:
                if obj in name:
                    ignore=True
            if ignore:
                continue
            fp=os.path.join(parent,name)
            obj_list.append((fp,name))
    for fp,name in obj_list:
        codesignFile(fp)
    codesignFile(app_dp)


def lj_obs_archive(app_dp,plugins_dp,data_dp_list,entitlements_fp,third_lib_list,out_dp,python_data_dp):
    build_dp=OTL.createDs(out_dp,'TEMP')
    OTL.copyD2D(app_dp,build_dp)
    app_name=OTL.getDirname(app_dp)
    contens_dp='{}/{}/Contents'.format(build_dp,app_name)
    # os.system("cd \"{}\";macdeployqt {}".format(build_dp,app_name))
    os.system("cd \"{}\";/Users/avc/Qt5.7.1/5.7/clang_64/bin/macdeployqt {}".format(build_dp,app_name))
    frameworks_dp = OTL.createDs(contens_dp,'Frameworks')
    for dp in data_dp_list:
        OTL.copyD2D(dp,contens_dp)
    
    OTL.copyD2D(python_data_dp,'{}/data'.format(contens_dp))
    # print('python_data_dp={}'.format(python_data_dp))
    # return
    OTL.copyF2D(entitlements_fp,contens_dp)

    for fp,name in third_lib_list:
        OTL.copyF2D(fp,frameworks_dp)
    name_list=OTL.adjuestLibs(frameworks_dp)

    OTL.copyD2D(plugins_dp,contens_dp)
    new_plugins_dp='{}/{}'.format(contens_dp,OTL.getDirname(plugins_dp))
    obj_list=OTL.getFiles(new_plugins_dp,['.so'],[])
    for fp,name in obj_list:
        OTL.adjuestLibToRpath(fp,name_list)
        OTL.adjuestRapth(fp,['@executable_path/../Frameworks'])

    app_process_name=app_name.replace('.app','')
    app_fp=contens_dp='{}/MacOS/{}'.format(contens_dp,app_process_name)
    OTL.adjuestRapth(app_fp,['@executable_path/../Frameworks'])
    OTL.adjuestLibToRpath(app_fp,name_list)

def getDmgList(name):
    obj_list=[]
    for obj in zwutil.run_cmd('cd /Volumes;ls').split('\n'):
        print(obj)
        if obj.startswith(name):
            obj_list.append(obj)
    return obj_list

def createDmg(app_dp,dmg_fp,out_dp,dmg_name):
    dmg_dp=''
    cn_name='临境直播'
    volume_dp='/Volumes'
    obj_list=getDmgList(cn_name)
    for obj in obj_list:
        zwutil.run_cmd('cd /Volumes;hdiutil detach \"{}\" -force'.format(obj))
    obj_list=getDmgList(cn_name)
    if len(obj_list) > 0:
        return False
    
    zwutil.run_cmd('hdiutil attach \"{}\"'.format(dmg_fp))
    obj_list=getDmgList(cn_name)
    if len(obj_list)  != 1:
        return False
    if obj_list[0] != cn_name:
        return False
    app_name='{}.app'.format(cn_name)
    to_path='{}/{}'.format(volume_dp,cn_name)
    old_path='{}/{}'.format(to_path,app_name)
    if os.path.exists(old_path):
        zwutil.run_cmd('rm -rf  \"{}\"'.format(old_path))

    if os.path.exists(old_path):
        return False

    OTL.copyD2D(app_dp,to_path)
    new_name=OTL.getDirname(app_dp)
    if new_name != cn_name:
        cmd='cd \"{}\";mv \"{}\" \"{}\"'.format(to_path,new_name,app_name)
        print(cmd)
        zwutil.run_cmd(cmd)
    zwutil.run_cmd('hdiutil detach \"{}\" -force'.format(to_path))
    zwutil.run_cmd('cd \"{}\";hdiutil convert \"{}\" -format UDZO -o \"{}.dmg\" -ov'.format(out_dp,dmg_fp,dmg_name))
    print("finished")

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

def getVersionInfo(fp):
    if not os.path.exists(fp):
        return False
    info = VersionInfo()
    with open(fp, 'r') as f:
        line_list = f.readlines()
        print(line_list)
        
        for line in line_list:
            if 'C_VERSION_MAJOR' in line:
                info.major = getKeyNumber(line,'C_VERSION_MAJOR')
            if 'C_VERSION_MINOR' in line:
                info.minor = getKeyNumber(line,'C_VERSION_MINOR')
            if 'C_VERSION_PATCH' in line:
                info.patch = getKeyNumber(line,'C_VERSION_PATCH')
            if 'C_VERSION_BUILD' in line:
                info.build = getKeyNumber(line,'C_VERSION_BUILD')
    return info

def updatePlistVersion(fp,info):
    plist=biplist.readPlist(fp)
    plist['CFBundleShortVersionString']=info.shortVersion()
    plist['CFBundleVersion']=info.fullVersion()
    biplist.writePlist(plist,fp,binary=False)

def run_lj_obs_archive():
    project_dp='/Users/avc/work/ljlive'
    entitlements_fp=os.path.join(project_dp,'ljobs/ljlive.entitlements')
    version_fp=os.path.join(project_dp,'ljobs/base/cversion_mac.cpp')
    vendor_dp=os.path.join(project_dp,'vendor')
    app_dp=os.path.join(project_dp,'bin/mac/release/ljlive.app')
    plugins_dp=os.path.join(vendor_dp,'obs/mac/obs-plugins')
    data_dp_list=[]
    data_dp_list.append(os.path.join(vendor_dp,'obs/mac/ljdata'))
    data_dp_list.append(os.path.join(vendor_dp,'obs/mac/data'))
    python_data_dp=os.path.join(vendor_dp,'python/lib/site-packages')
    print('python_data_dp={}'.format(python_data_dp))
    out_dp="/Users/avc/Documents/TEMP"
    third_lib_list=OTL.getFiles(vendor_dp,['.so','.dylib'],['/obs-plugins/','debug'])
    exists_name_list=[]
    for fp,name in third_lib_list:
        if name in exists_name_list:
            print('name is repeat name={},fp={},fp={}'.format(name,fp,exists_name_list[name][1]))
        else:
            exists_name_list.append((name,fp))
    lj_obs_archive(app_dp,plugins_dp,data_dp_list,entitlements_fp,third_lib_list,out_dp,python_data_dp)


def test():
    # dep_dp='/Users/avc/Documents/TEMP/TEMP'
    # app_fp='/Users/avc/Downloads/TEMP/ljlive.app/Contents/MacOS/ljlive'
    obj_list = OTL.getFiles('/Users/avc/Documents/TEMP/TEMP/ljlive.app/Contents/Frameworks',['.so','.dylib'],[],False)
    for fp,name in obj_list:
        print(fp)
        rpath_list = OTL.getLibRpaths(fp)
        print(rpath_list)
    rpath_list = OTL.getLibRpaths('/Users/avc/Documents/TEMP/TEMP/ljlive.app/Contents/MacOS/ljlive')
    print(rpath_list)

# test()
app_dp='/Users/avc/Documents/TEMP/TEMP/ljlive.app'
verson_info=getVersionInfo('/Users/avc/work/ljlive/ljobs/base/cversion_mac.cpp')
dmg_name='ljlive{}'.format(verson_info.fullVersion())
plist_fp='/Users/avc/work/ljlive/ljobs/ljlive.plist'
updatePlistVersion(plist_fp,verson_info)
# run_lj_obs_archive()
# codesignApp(app_dp)
# createDmg(app_dp,'/Users/avc/work/release/ljlive-release.dmg','/Users/avc/Documents/TEMP',dmg_name)   
# res = zwutil.run_cmd("/Users/avc/Qt5.7.1/5.7/clang_64/bin/macdeployqt")
# print(res)
print('app_dp={}'.format(app_dp))
print('verson_info={}'.format(verson_info))
print('dmg_name={}'.format(dmg_name))
print('plist_fp={}'.format(plist_fp))