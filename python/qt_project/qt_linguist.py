import xlrd
import sys
import os
import xml.etree.ElementTree as ET

class TSObject:
    def __init__(self):
        self.version=''
        self.language=''
        self.sourcelanguage=''
        self.tail='\n'
        self.text=''
        self.content_list=[]
    def saveToFile(self,fp):
        # root = ET.Element('TS')
        root = createElement('TS')
        root.tail = self.tail
        # root.text = self.text
        root.set('version',self.version)
        root.set('language',self.language)
        root.set('sourcelanguage',self.sourcelanguage)
        for obj in self.content_list:
            element = ET.Element('TS1')
            root.append(obj.toElement())
        # ET.dump(root) 
        prettyXml(root,'\t','\n',0)
        tree = ET.ElementTree(root) 
        tree.write(fp,encoding='UTF-8', xml_declaration=True,short_empty_elements=True)
        # tree.write(fp,encoding='UTF-8', xml_declaration=True,method='html')

#参考 https://www.cnblogs.com/huzixia/p/10391987.html
def prettyXml(element, indent, newline, level = 0): # elemnt为传进来的Elment类，参数indent用于缩进，newline用于换行  
    if element:  # 判断element是否有子元素  
        if element.text == None or element.text.isspace(): # 如果element的text没有内容  
            element.text = newline + indent * (level + 1)    
        else:  
            element.text = newline + indent * (level + 1) + element.text.strip() + newline + indent * (level + 1)  
    #else:  # 此处两行如果把注释去掉，Element的text也会另起一行  
        #element.text = newline + indent * (level + 1) + element.text.strip() + newline + indent * level  
    temp = list(element) # 将elemnt转成list  
    for subelement in temp:  
        if temp.index(subelement) < (len(temp) - 1): # 如果不是list的最后一个元素，说明下一个行是同级别元素的起始，缩进应一致  
            subelement.tail = newline + indent * (level + 1)  
        else:  # 如果是list的最后一个元素， 说明下一行是母元素的结束，缩进应该少一个  
            subelement.tail = newline + indent * level  
        prettyXml(subelement, indent, newline, level = level + 1) # 对子元素进行递归操作  

    
def adjuestElement(element,level,has_child,final=False):
    pass
    # indent='\n{}'.format(' ' * 4 * level)
    # if has_child:
    #     element.text = '\n{}'.format(' ' * 4 * (level + 1))
    #     # element.tail = '\n{}'.format(' ' * 4 * level)
    #     if level > 1:
    #         element.tail='\n{}'.format(' ' * 4 * (level - 1))
    #     else:
    #         element.tail='\n'
    # else:
    #     if final:
    #         level = level - 1
    #    if level > 1:
    #         element.tail='\n{}'.format(' ' * 4 * (level - 1))
    #     else:
    #         element.tail='\n'


def createElement(name,parent=None):
    element = ET.Element(name)
    if parent != None:
        parent.append(element)
    return element

class TSMessage:
    def __init__(self):
        self.filename=''
        self.line=''
        self.source=''
        self.translation=''
        self.type=''
        self.tail='\n'
        self.location_tail='\n'
        self.source_tail='\n'
        self.translation_tail='\n'
        self.text=''
        self.location_text=''
        self.source_text=''
        self.translation_text=''

    def __str__(self):
        return 'filename={},line={},source={},translation={},type={}'.format(
            self.filename,self.line,self.source,self.translation,self.type)
    def toElement(self):
        element = createElement('message')
        # element.tail=self.tail
        # element.text=self.text

        location_element = createElement('location',element)
        location_element.set('filename',self.filename)
        location_element.set('line',self.line)
        # location_element.tail=self.location_tail
        # location_element.text=self.location_text

        source_element = createElement('source',element)
        source_element.text=self.source
        # source_element.tail=self.source_tail
        # source_element.text=self.source_text

        translation_element = createElement('translation',element)
        translation_element.text=self.translation
        translation_element.set('type','unfinished')
        # translation_element.tail=self.translation_tail
        return element

class TSContent:
    def __init__(self):
        self.name=''
        self.tail='\n'
        self.text=''
        self.message_list = []
    def toElement(self):
        # element = ET.Element('context')
        element = createElement('context')
        # element.tail = self.tail
        # element.text = self.text
        # name_element = ET.SubElement(element, 'name')
        name_element = createElement('name',element)
        name_element.text=self.name 
        for obj in self.message_list:
            element.append(obj.toElement())
        return element


def parseMessage(obj):
    # print('obj.tail={},len={}'.format(obj.tail,len(obj.tail)))
    message = TSMessage()
    message.tail = obj.tail
    message.text = obj.text

    for tag in obj.iter("location"):
        message.filename = tag.get('filename')
        message.line = tag.get('line')
        message.location_tail = tag.tail
        message.location_text = tag.text

    for tag in obj.iter("source"):
        message.source = tag.text
        message.source_tail = tag.tail
        message.source_text = tag.text

    for tag in obj.iter("translation"):
        message.translation = tag.text
        message.type = tag.get('type','')
        message.translation_tail = tag.tail
        message.translation_text = tag.text

    # print(message)
    return message

def parseContent(obj):
    print('obj.text={},len={}'.format(obj.text,len(obj.text)))
    content = TSContent()
    content.tail = obj.tail
    content.text = obj.text
    for tg_name in obj.iter("name"):
        content.name = tg_name.text
    for tg_message in obj.iter("message"):
        message = parseMessage(tg_message)
        if '!DOCTYPE HTML PUBLIC' in message.source:
            continue
        content.message_list.append(message)
    return content

def parseTsFile(fp):
    tree = ET.parse(fp)
    root = tree.getroot()
    obj = TSObject()
    obj.tail = root.tail
    obj.text = root.text
    obj.version=root.get('version','')
    obj.language=root.get('language','')
    obj.sourcelanguage=root.get('sourcelanguage','')
    for tg_context in root.iter("context"):
        content = parseContent(tg_context)
        obj.content_list.append(content)
    return obj

        

fp='/Users/avc/work/ljlive/ljobs/translations/ljlive_zh_CN.ts'
# fp='/Users/avc/Documents/TEMP/ljlive_zh_CN.ts'
fp_save='/Users/avc/Documents/TEMP/test_out.ts'
obj = parseTsFile(fp)
obj.saveToFile(fp_save)

