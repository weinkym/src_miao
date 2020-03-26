#!/usr/local/bin/python3

import sys,os
sys.path.append(os.getcwd())
import subprocess
from zwpy import zwutil

g_lib_subffix_list=['.dylib','.so']
g_valid_rp_list=['@executable_path/','@loader_path/']
def test(root_dp,removeRpath=False):
    lib_file_list=[]
    for subffix in g_lib_subffix_list:
        lib_file_list+=zwutil.getFileNamePaths(root_dp,subffix)
    
    endKey=' (offset'
    startKey='path '
    for fp,name in lib_file_list:
        if os.path.islink(fp):
            # print('link file,ignore {}'.format(fp))
            continue
        res=zwutil.run_cmd("otool -l \"{}\" | grep RPATH -A2 ".format(fp))
        lineList=res.split("\n")
        for line in lineList:
            line = line.lstrip()
            startIndex = line.find(startKey,0)
            if startIndex >= 0:
                endIndex = line.find(endKey,0)
                if endIndex >= 0:
                    rp=line[startIndex+len(startKey):endIndex]
                    if rp in g_valid_rp_list:
                        continue
                    print("name={},rp={},{}".format(name,rp,line))
                    if removeRpath:
                        cmd='install_name_tool \'{}\' -delete_rpath \'{}\''.format(fp,rp)
                        print('cmd={}'.format(cmd))
                        zwutil.run_cmd(cmd)
                        

                else:
                    print("error")
                    return False


            
        # print(name)
    # print(lib_file_list)
# test_str="path /Users/starfish/Qt5.7.1/5.7/clang_64/lib (offset 12)"
# endKey=' (offset'
# startKey='path '
# startIndex = test_str.find(startKey,0)
# endIndex = test_str.find(endKey,0)

# print(startIndex)
# print(endIndex)
# print(test_str[startIndex+len(startKey):endIndex])
root_dp='/Users/miaozw/work/ljlive/vendor'
test(root_dp,True)