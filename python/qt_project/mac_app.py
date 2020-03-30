#!/usr/local/bin/python3

import sys,os
sys.path.append(os.getcwd())
import subprocess
from zwpy import zwutil
import mac_utils as OTL

G_IDENT_APP='Developer ID Application: Jiangsu Yunxuetang Network Technology  Co.,Ltd (6F8YPTA92C)'
G_IDENT_INSTALL=''

def codesignApp(app_dp):
    framework_name=''
    for parent,dirnames,filenames in os.walk(path,  topdown=False):
        for obj in dirnames:
            # name=OTL.getDirname(obj)
            name=obj
            if name.endwith(framework_name):
                print("name={}".format(obj))

def lj_obs_archive(app_dp,plugins_dp,data_dp_list,entitlements_fp,third_lib_list,out_dp):
    build_dp=OTL.createDs(out_dp,'TEMP')
    OTL.copyD2D(app_dp,build_dp)
    app_name=OTL.getDirname(app_dp)
    contens_dp='{}/{}/Contents'.format(build_dp,app_name)
    # os.system("cd \"{}\";macdeployqt {}".format(build_dp,app_name))
    os.system("cd \"{}\";/Users/avc/Qt5.7.1/5.7/clang_64/bin/macdeployqt {}".format(build_dp,app_name))
    frameworks_dp = OTL.createDs(contens_dp,'Frameworks')
    for dp in data_dp_list:
        OTL.copyD2D(dp,contens_dp)
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
    python_data_dp=os.path.join(vendor_dp,'python/lib、python/site-packages')
    print('app_dp={}'.format(app_dp))
    out_dp="/Users/avc/Documents/TEMP"
    third_lib_list=OTL.getFiles(vendor_dp,['.so','.dylib'],['/obs-plugins/','debug'])
    exists_name_list=[]
    for fp,name in third_lib_list:
        if name in exists_name_list:
            print('name is repeat name={},fp={},fp={}'.format(name,fp,exists_name_list[name][1]))
        else:
            exists_name_list.append((name,fp))
    lj_obs_archive(app_dp,plugins_dp,data_dp_list,entitlements_fp,third_lib_list,out_dp)

run_lj_obs_archive()

def test():
    dep_dp='/Users/avc/work/ljlive/vendor'
    app_fp='/Users/avc/work/ljlive/bin/mac/release/ljlive.app/Contents/MacOS/ljlive'
    macos_dp=os.path.dirname(app_fp)
    contens_dp=os.path.dirname(macos_dp)
    # frameworks_dp=os.path.join(contens_dp,'Frameworks')
    frameworks_dp = createDs(contens_dp,'Frameworks')
    print('contens_dp={}'.format(contens_dp))
    print('frameworks_dp={}'.format(frameworks_dp))
    # lj_copyLibs(dep_dp,frameworks_dp)
    # lj_adjust_lib(frameworks_dp)

    lib_subfix_list=['.so','.dylib']
    lib_obj_list=getFiles(frameworks_dp,lib_subfix_list,'')
    exisit_name_list=[]
    for fp,name in lib_obj_list:
        exisit_name_list.append(name)

    lj_adjuestApp(app_fp,exisit_name_list)

# res = zwutil.run_cmd("/Users/avc/Qt5.7.1/5.7/clang_64/bin/macdeployqt")
# print(res)