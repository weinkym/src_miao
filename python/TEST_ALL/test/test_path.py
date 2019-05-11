##!/usr/local/bin/python3
import os
import platform
import sys

libname='.lib'
pdbname='.pdb'
dllname='.dll'
rundir_name='\\rundir\\'
plugins_name='\\plugins\\'
pdbs_name='pdbs'

class  ZItemPathData:
    lib=""
    dll=""
    pdb=""

class ZBuildData:
    l_lib_path = []
    l_lib_pdb_path = []
    l_dll_path = []
    l_plugins_dll_path = []
    l_plugins_pdb_path = []
    map_lib={}
    map_plugins={}

def setMapValue(map,key,item_name,value):
    if key not in map:
        map[key]=ZItemPathData()
        return True
    else:
        map[key][item_name] != ''
        return False
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

            if filename.endswith(libname) and is_plugins==False:
                result_data.l_lib_path.append(file_path)
                setMapValue(result_data.map_lib,filename,'lib',file_path)

            if filename.endswith(pdbname) and is_pdb:
                if is_plugins:
                    result_data.l_plugins_pdb_path.append(os.path.join(parent,filename))
                    setMapValue(result_data.map_plugins,filename,'pdb',file_path)
                else:
                    result_data.l_lib_pdb_path.append(os.path.join(parent,filename))
                    setMapValue(result_data.map_lib,filename,'pdb',file_path)

            if filename.endswith(dllname):
                if is_plugins:
                    result_data.l_plugins_dll_path.append(os.path.join(parent,filename))
                    setMapValue(result_data.map_plugins,filename,'dll',file_path)
                else:
                    result_data.l_dll_path.append(os.path.join(parent,filename))
                    setMapValue(result_data.map_lib,filename,'dll',file_path)

    return result_data

# print("中parent=")
# temp = sys.getdefaultencoding()  
print(platform.python_version())
# path="/Users/miaozw/Documents/TEMP/build-lj"
# path="/Users/miaozw/work/src_miao/python"
path='E:\\build-lj'
result=getList(path)

print("l_lib_path=",len(result.l_lib_path))
# for path in result.l_lib_path:
    # print("lib=",path)

print("l_dll_path=",len(result.l_dll_path))
# for path in result.l_dll_path:
#     print("lib=",path)

index=0
print("l_plugins_dll_path=",len(result.l_plugins_dll_path))
for path in result.l_plugins_dll_path:
    print("lib=",path,index)
    index=index+1

print("l_lib_pdb_path=",len(result.l_lib_pdb_path))
# for path in result.l_lib_pdb_path:
#     print("lib=",path)

index=0
print("l_plugins_pdb_path=",len(result.l_plugins_pdb_path))
for path in result.l_plugins_pdb_path:
    print("lib=",path,index)
    index=index+1