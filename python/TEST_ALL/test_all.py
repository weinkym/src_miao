
import subprocess
import os
import xml.etree.ElementTree as ET
import biplist

def test(fp):
    tree = ET.parse(fp)
    root = tree.getroot()
    print(root.tag)
    '''遍历所有数据'''
    for ta in root.iter("dict"):
        print(ta.tag,ta.attrib,ta.text)
        key=''
        for k in ta:
            if key == 'CFBundleShortVersionString':
                if k.tag == 'string':
                    print(k.text)
                    k.text='000000'
            key = k.text
            # print(k.tag,k.attrib,k.text)
    # declaration='<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">'
    declaration='<!DOCTYPE  plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">'
    tree.write(fp,encoding='UTF-8',xml_declaration=declaration)

def testPlist(fp):
    plist = biplist.readPlist(fp)
    plist['CFBundleVersion']='1.18.0'
    plist['CFBundleVersion']='1.18.0.2004092015'
    biplist.writePlist(plist,fp,binary=False)

source_fp='/Users/avc/work/ljlive/ljobs/ljlive.plist'
os.system('cp {} /Users/avc/Documents/TEMP'.format(source_fp))
fp='/Users/avc/Documents/TEMP/ljlive.plist'
# test(fp)
testPlist(fp)
a = 10
b = a
a=11
print(b)

# loc = locals()
# def get_variable_name(variable):
#     print(loc)
#     for key in loc:
#         if loc[key] == variable:
#             return key

# def test3(**name):
#     print(type(name))
#     print(name)

# tttt=199
# test3(tttt=tttt)
# abc=199
# ret = get_variable_name(abc)
# print('ret={}'.format(ret))
