import os
import subprocess

def getFilePaths(path,suffix):
    fileList=[]
    for parent,dirnames,filenames in os.walk(path,  topdown=False):
        for filename in filenames:
            if filename.endswith(suffix):
                file_path=os.path.join(parent,filename)
                fileList.append(file_path)
                # print(file_path)
    return fileList

def getFileNamePaths(path,key,deep=True,is_prefix = False):
    fileList=[]
    for parent,dirnames,filenames in os.walk(path,  topdown=False):
        if not deep and parent != path:
            continue
        for filename in filenames:
            if (is_prefix and filename.startswith(key)) or (not is_prefix and filename.endswith(key)):
                file_path=os.path.join(parent,filename)
                fileList.append((file_path,filename))
                # print(file_path)
    return fileList

def getPathFileNameInfo(path):
    (file_path,file_name) = os.path.split(path)
    (shot_name,suffix) = os.path.splitext(file_name)
    return file_path,file_name,shot_name,suffix

def getDirPaths(path,key,deep=True,is_prefix = False):
    objList=[]
    print('=======')
    for parent,dirnames,filenames in os.walk(path,  topdown=False):
        if not deep and parent != path:
            continue
        for name in dirnames:
            if (is_prefix and name.startswith(key)) or (not is_prefix and name.endswith(key)):
                objList.append((os.path.join(parent,name),name))
    return objList

def run_cmd(cmd):
    sub = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    sub.wait()
    content=sub.stdout.read().decode("utf-8")
    return content