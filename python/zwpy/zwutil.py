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
    
def run_cmd(cmd):
    sub = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    sub.wait()
    content=sub.stdout.read().decode("utf-8")
    return content