
from PIL import Image
from PIL import ImageColor
from PIL import ImageDraw
from PIL import ImageDraw2
import subprocess
import os

class LineInfo:
    valid = False
    lib_name = ''
    lib_path = ''

def getPathFileNameInfo(path):
    (file_path,file_name) = os.path.split(path)
    (shot_name,suffix) = os.path.splitext(file_name)
    return file_path,file_name,shot_name,suffix

def run_cmd(cmd):
    sub = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    sub.wait()
    content=sub.stdout.read().decode("utf-8")
    return content

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

def test():
    fp='/Users/miaozw/Documents/ffmpeg-3.4.7/build/bin/ffmpeg'
    res=run_cmd("otool -L \"{}\"".format(fp))
    lineList=res.split("\n")
    cmd_list=[]
    print('fp={}'.format(fp))
    for line in lineList[1:]:
        line=line.lstrip()
        if 'build' not in line:
            continue
        info = parse_otool_line(line)
        cmd = 'install_name_tool -change \"{}\" \"@executable_path/{}\" \"{}\"'.format(
            info.lib_path, info.lib_name, fp)
        cmd_list.append(cmd)
        
        print('libname={},path={}'.format(info.lib_name,info.lib_path))
    for cmd in cmd_list:
        print('cmd={}'.format(cmd))
        os.system(cmd)
test()
# img = Image.open("/Users/miaozw/Pictures/11.jpg")
# img = img.resize((136,80))
# print(img.format)		 # 输出图片基本信息
# print(img.mode)
# print(img.size)
# # img.setColor()
# for i in range(img.size[0]):
#     for j in range(img.size[1]):
#         color=img.getpixel((i,j))
#         # print(color)
#         # img.putpixel((i,j),(0,color[1],0))
#         # img.putpixel((i,j),(0,0,color[2]))
# img.save("/Users/miaozw/Documents/rgb.PNG")

# wb.save(r'/Users/miaozw/Documents/TEMP/python/test.xlsx')
