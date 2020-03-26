
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
    path="/Users/miaozw/Documents/TEMP"
    delete_list=[]
    delete_suffix_list=['.o','.user','.pyc','Makefile','moc_']
    for parent,dirnames,filenames in os.walk(path,  topdown=False):
        for filename in filenames:
            found=False
            for obj in delete_suffix_list:
                if obj in filename:
                    found=True
                    break
            if found:
                delete_list.append(os.path.join(parent,filename))
    
    for obj in delete_list:
        os.system("rm -f \"{}\"".format(obj))
        print(obj)




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
