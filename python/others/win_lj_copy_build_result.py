##!/usr/local/bin/python3
import os
import platform
import sys
from shutil import copyfile
from shutil import copy

libname='.lib'
pdbname='.pdb'
dllname='.dll'
rundir_name='\\rundir\\'
plugins_name='\\plugins\\'
pdbs_name='pdbs'

class ZBuildData:
    dict_other_pdb={}
    map_lib={}
    map_plugins={}

def setMapValue(map,filename,item_name,value):
    key=filename[0:-len(item_name)-1]
    if key not in map:
        map[key]={}
    elif key in map[key]:
        print("error:file",key)
    
    map[key][item_name]=value
    return True

# encoding=utf8 
def getList(path):
    result_data = ZBuildData()
    for parent,dirnames,filenames in os.walk(path,  topdown=False):
        if rundir_name in parent:
            continue
        is_plugins=False
        if plugins_name in parent:
            is_plugins=True

        is_pdb=False
        if pdbs_name in parent:
            is_pdb=True

        for filename in filenames:
            file_path=os.path.join(parent,filename)

            if filename.endswith(pdbname):
                key=filename[0:-len(pdbname)-1]
                result_data.dict_other_pdb[key]=file_path


            if filename.endswith(libname) and is_plugins==False:
                setMapValue(result_data.map_lib,filename,libname,file_path)

            if filename.endswith(pdbname) and is_pdb:
                if is_plugins:
                    setMapValue(result_data.map_plugins,filename,pdbname,file_path)
                else:
                    setMapValue(result_data.map_lib,filename,pdbname,file_path)

            if filename.endswith(dllname):
                if is_plugins:
                    setMapValue(result_data.map_plugins,filename,dllname,file_path)
                else:
                    setMapValue(result_data.map_lib,filename,dllname,file_path)

    return result_data

def chekResultData(result_data):
    def checkDict(dict_data,dict_adjuest_data):
        for key,value in dict_data.items():
            if pdbname not in value and dllname in value:
                if key in dict_adjuest_data:
                    value[pdbname]=dict_adjuest_data[key]
                else:
                    print('lib:{} has not {}'.format(key,dllname))
                    return False
        return True

    return checkDict(result_data.map_lib,result_data.dict_other_pdb) and checkDict(result_data.map_plugins,result_data.dict_other_pdb)

def copyItemFile(key,libname,from_path,to_path):
    copy(from_path,to_path)
    print('copylib lib:{}{}  from {} to {}'.format(key,libname,from_path,to_path))

def copyReslutData(result_data,root_path):
    list_output_lib_path=[os.path.join(root_path,r'vendor\obs\win\lib\release')]
    list_output_plugins_path=[os.path.join(root_path,r'vendor\obs\win\obs-plugins\32bit'),os.path.join(root_path,r'release\obs-plugins\32bit')]
    list_output_plugins_pdb_path=[os.path.join(root_path,r'vendor\obs\win\pdb\obs-plugins')]
    list_output_lib_pdb_path=[os.path.join(root_path,r'vendor\obs\win\pdb\lib')]
    for key,value in result_data.map_lib.items():
        for path in list_output_lib_path:
            copyItemFile(key,libname,value[libname],path)
            if dllname in value:
                copyItemFile(key,dllname,value[dllname],path)
        for path in list_output_lib_pdb_path:
            if pdbname in value:
                copyItemFile(key,pdbname,value[pdbname],path)

    for key,value in result_data.map_plugins.items():
        for path in list_output_plugins_path:
            # print('copylib lib:{}{}  from {} to {}'.format(key,libname,value[libname],path))
            copyItemFile(key,dllname,value[dllname],path)
        for path in list_output_plugins_pdb_path:
            copyItemFile(key,pdbname,value[pdbname],path)
     
# temp = sys.getdefaultencoding()  
print(platform.python_version())
# path="/Users/miaozw/Documents/TEMP/build-lj"
# path="/Users/miaozw/work/src_miao/python"

if __name__ == '__main__':
    path='E:\\build-lj'
    ljlive_root_path='F:\wook_root\project\ljlive'
    result=getList(path)
    if chekResultData(result):
        copyReslutData(result,ljlive_root_path)

