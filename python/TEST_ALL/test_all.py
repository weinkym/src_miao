
import subprocess
import os
import xml.etree.ElementTree as ET

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
    declaration=' plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\"'
    tree.write(fp,encoding='UTF-8',xml_declaration=declaration)



# source_fp='/Users/avc/work/ljlive/ljobs/ljlive.plist'
# os.system('cp {} /Users/avc/Documents/TEMP'.format(source_fp))
# fp='/Users/avc/Documents/TEMP/ljlive.plist'
# test(fp)

loc = locals()
def get_variable_name(variable):
    print(loc)
    for key in loc:
        if loc[key] == variable:
            return key

def test3(a):
    print(a)
tttt=199
test3(tttt)
abc=199
ret = get_variable_name(abc)
print('ret={}'.format(ret))
