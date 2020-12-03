#!/usr/local/bin python3
# -*- coding: utf-8 -*-
#
import subprocess
import os
import xml.etree.ElementTree as ET
import biplist
import openpyxl
from openpyxl.drawing.image import Image
from openpyxl.worksheet.worksheet import Worksheet
from PIL import Image, ImageDraw, ImageFont
import sys, os
sys.path.append(os.getcwd())
import subprocess
# import public_defines as PB
from zwpy import zwutil

width = 45
height = 20
row = 0
# height = height / 2


def doCrop360m(fp):
    # img = Image.open("/Users/avc/Downloads/360/13-39-45.png")
    img = Image.open(fp)
    print(img.size)
    x = 1178
    y = 968
    w = width
    h = height
    box = (x, y, x + w, y + h)
    cropped = img.crop(box)  # (left, upper, right, lower)
    draw = ImageDraw.Draw(cropped)
    # myfont = ImageFont.truetype(size=35)
    # draw.text((0, 0), "{}".format(row + 1), fill='red')
    return cropped


def doCrop720m(fp):
    # img = Image.open("/Users/avc/Downloads/360/13-39-45.png")
    img = Image.open(fp)
    print(img.size)
    x = 1064
    y = 963
    w = width
    h = height
    box = (x, y, x + w, y + h)
    cropped = img.crop(box)  # (left, upper, right, lower)
    draw = ImageDraw.Draw(cropped)
    # myfont = ImageFont.truetype(size=35)
    draw.text((0, 0), "{}".format(row + 1), fill='red')
    return cropped


def doCrop360cpu(fp):
    # img = Image.open("/Users/avc/Downloads/360/13-39-45.png")
    img = Image.open(fp)
    print(img.size)
    x = 1673
    y = 310
    w = width
    h = height
    box = (x, y, x + w, y + h)
    cropped = img.crop(box)  # (left, upper, right, lower)
    draw = ImageDraw.Draw(cropped)
    # myfont = ImageFont.truetype(size=35)
    draw.text((0, 0), "{}".format(row + 1), fill='red')
    return cropped


def doRun(dp):
    global row
    file_list = zwutil.getFilePaths(dp, ".png")
    file_list.sort()
    file_list = file_list[0:60]
    # target = Image.new('RGB',)
    for name in file_list:
        print(name)

    row_num = len(file_list)
    column = 1
    target = Image.new('RGB', (width * column, height * row_num))
    for fp in file_list:
        image = doCrop360m(fp)
        # image = doCrop360cpu(fp)
        # image.show()
        w = width
        h = height
        x = 0
        y = height * row
        box = (x, y, w, h)
        target.paste(image, (0, y, width, height * (row + 1)))
        row = row + 1
    target.show()
    target.save("/Users/avc/Documents/tttttt.jpg")


print(0xF1 | 0x03)
# doRun("/Users/avc/Downloads/360")
# doRun("/Users/avc/Downloads/720")

# file_list = zwutil.getFilePaths("/Users/avc/Downloads/360", ".png")
# file_list.sort()
# # file_list = file_list[0:60]
# img = Image.open(file_list[0])
# print(img.size)
# x = 1178
# y = 968
# w = 40
# h = 20
# box = (x, y, x + w, y + h)
# cropped = img.crop(box)  # (left, upper, right, lower)
# # # cropped.save("/Users/avc/Documents/TEMP/testpng/test2.jpg")
# cropped.show()
