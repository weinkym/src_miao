import xlrd
import sys

import  xml.dom.minidom
from xml.dom.minidom import Document

def get_tag_name_node_only(obj,name):
    itemlistName = obj.getElementsByTagName(name)
    if itemlistName.length != 1:
        print("error")
        return
    return itemlistName[0]

def get_tag_name_node_text_only(obj,name):
    node_obj = get_tag_name_node_only(obj,name)
    return node_obj.childNodes[0].data

def parse_context_message_location(node_obj):
    dict_res={}
    dict_res['filename'] = node_obj.getAttribute('filename')
    dict_res['line'] = node_obj.getAttribute('line')
    return dict_res

def parse_context_message(node_obj):
    dict_res={}
    list_location = []
    list_node_location = node_obj.getElementsByTagName('location')
    for obj in list_node_location:
        list_location.append(parse_context_message_location(obj))
    dict_res['location']=list_location

    dict_res['source'] = get_tag_name_node_text_only(node_obj,'source')

    node_translation = get_tag_name_node_only(node_obj,'translation')
    dict_res['translation'] = node_translation.getAttribute('type')
    # print(dict_res['source'])
    # print(dict_res['translation'])
    return dict_res

def parse_context(node_obj):
    nodeObjName = get_tag_name_node_only(node_obj,'name')
    dict_obj = {}
    dict_obj['name'] = nodeObjName.childNodes[0].data
    list_message=[]
    list_node_message = node_obj.getElementsByTagName('message')
    for obj in list_node_message:
        list_message.append(parse_context_message(obj))

    dict_obj['message']=list_message
    return dict_obj

class TSVerify(object):
    g_except_source_list = ('TextLabel', 'PushButton', 'Form', 'CheckBox')

    def verify_location(obj):
        if obj.startswith('../obs/UI'):
            return False
        return True

    def varify_source(obj):
        if obj in TSVerify.g_except_source_list:
            return False
        return True


def create_codument(list_node_context,dict_ts_source,key_name):
    doc = Document()  # 创建DOM文档对象
    # doc.doctype="TS"
    doctype = doc.implementation.createDocumentType("TS",'','')
    doctype.name="TS"
    doc.appendChild(doctype)

    node_ts = doc.createElement('TS')  # 创建根元素
    node_ts.setAttribute('version', "2.1")  # 设置命名空间
    node_ts.setAttribute('language', 'zh_CN')  # 引用本地XML Schema
    doc.appendChild(node_ts)
    for obj in list_node_context:
        note_context=doc.createElement('context')
        note_name=doc.createElement('name')
        note_name_text = doc.createTextNode(obj['name'])
        # print(obj['name'])
        # note_name_text = doc.createTextNode("aa")
        note_name.appendChild(note_name_text)
        note_context.appendChild(note_name)
        message_count=0

        for message_obj in obj['message']:
            if not TSVerify.varify_source(message_obj['source']):
                continue

            note_message = doc.createElement('message')
            locatioin_count=0
            for location_obj in message_obj['location']:
                if not TSVerify.verify_location(location_obj['filename']):
                    continue
                note_location = doc.createElement('location')
                note_location.setAttribute('filename',location_obj['filename'])
                note_location.setAttribute('line', location_obj['line'])
                note_message.appendChild(note_location)
                locatioin_count = locatioin_count + 1;

            if locatioin_count <= 0:
                continue
            note_source = doc.createElement('source')
            note_source_text = doc.createTextNode(message_obj['source'])
            note_source.appendChild(note_source_text)
            note_message.appendChild(note_source)

            note_translation = doc.createElement('translation')

            ts_value=None
            if message_obj['source']in dict_ts_source:
                ts_obj=dict_ts_source[message_obj['source']]
                if type(ts_obj) == dict:
                    ts_value=ts_obj[key_name]

            if ts_value == None:
                note_translation.setAttribute('type', 'unfinished')
                sys.stderr.write("not found source=%s\n" % (message_obj['source']))
            else:
                note_translation_text = doc.createTextNode(ts_value)
                note_translation.appendChild(note_translation_text)


            note_message.appendChild(note_translation)

            note_context.appendChild(note_message)
            message_count = message_count + 1

        if message_count <= 0:
            continue
        node_ts.appendChild(note_context)
    return doc

def update_ts(src_path,des_path,dict_ts_source,key_name):
    dom = xml.dom.minidom.parse(src_path)
    list_node_context=[]
    itemlist = dom.getElementsByTagName('context')
    for obj in itemlist:
        list_node_context.append(parse_context(obj))

    doc = create_codument(list_node_context,dict_ts_source,key_name)
    f = open(des_path, 'wb+')
    f.write(doc.toprettyxml(indent='\t', encoding='utf-8'))
    f.close()

#=================================

lj_g_remove_string_list = ["\n","\t"," "]
def lj_check_string(string):
    str=string
    has_error = False
    if str == "":
        return str,has_error
    while(1):
        last_str = str[-1]
        if last_str == "":
            break;
        if last_str in lj_g_remove_string_list:
            has_error = True
            str = str[0:-2]
        else:
            break

    if str == "":
        return str,has_error
    while(1):
        first_str = str[0]
        if first_str == "":
            break;
        if first_str in lj_g_remove_string_list:
            has_error = True
            str = str[1:-1]
        else:
            break
    return str,has_error

def read_table(table,key_index,list_value_key_index):
    result_data = {}
    num_col = table.ncols
    num_row = table.nrows
    # print(table.ncols)
    # print(table.nrows)
    for obj in list_value_key_index:
        if len(obj) != 2:
            sys.stderr.write("param list length is error obj.length=%d" % (len(obj)))

        if num_col < obj[1] or num_col < key_index:
            sys.stderr.write("col is invalid num_col=%d,key_index=%d,obj[1]=%d" % (key_index,num_col,obj[1]))

    for row in range(1,table.nrows):
        src_key=table.cell_value(row,key_index)
        result = (lj_check_string(src_key))
        if result[1]:
            has_error = True
            print("row=%d,src key has invalid character index=%d" % (row,key_index))

        src_key = result[0]
        dict_value={}
        for obj in list_value_key_index:
            value = table.cell_value(row, obj[1])
            result = (lj_check_string(value))
            if result[1]:
                has_error = True
                print("row=%d,src key has invalid character,index=%d" % (row, key_index))
            dict_value[obj[0]]=result[0]
        if src_key in result_data:
            print("row=%d,src key is repetition index=%d,src_key=%s" % (row,key_index,src_key))

        result_data[src_key]=dict_value

    return result_data

def read_ts_source(filename):
    print(filename)
    work_book = xlrd.open_workbook(filename)
    table_all = work_book.sheet_by_name('ALL')
    list_key_index=[['cn',1]]
    res = read_table(table_all,0,list_key_index)
    print(res)

    return res

def update():
    path="/Users/miaozw/work/mzwdoc/dclive/dcobs/translations"
    dict_ts_source = read_ts_source(path+'/all.xlsx')
    update_ts(path+"/dcobs_zh_CN.ts",path+"/out.ts",dict_ts_source,'cn')
    # in_files = r"/Users/miaozw/work/mzwdoc/dclive/dcobs/translations/dcobs_zh_CN.ts"
    # out_file = r"/Users/miaozw/work/mzwdoc/dclive/dcobs/translations/dcobs_zh_CN.ts"
# print(itemlist.length)
update()
# read_xml("/Users/miaozw/work/mzwdoc/dclive/dcobs/translations/dcobs_zh_CN.ts")