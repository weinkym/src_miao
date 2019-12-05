import xlrd
import shutil,os
import sys

import  xml.dom.minidom
from xml.dom.minidom import Document

class LJCN_EG_DATA:
    key_src = ""
    key_cn = ""
    key_eg = ""
    key_eg_short = ""
    key_count = 1
    key_row = 0
    key_table_name=""

class LJError(Exception):
    def __init__(self,value):
        self.value=value
    def __str__(self):
        return repr(self.value)

class LJDebugData:
    list_except_trans = []
    list_warning = []
    list_used_key = []

lj_g_remove_string_list = ["\n","\t"," "]
def lj_check_string(string):
    result_str=string
    if not isinstance(result_str,str):
        sys.stderr.write("key is not string type ,key={}\n".format(result_str))
        return result_str,True;

    has_error = False
    if result_str == "":
        return result_str,has_error
    while(1):
        last_str = result_str[-1]
        if last_str == "":
            break;
        if last_str in lj_g_remove_string_list:
            has_error = True
            result_str = result_str[0:-2]
        else:
            break

    if result_str == "":
        return result_str,has_error
    while(1):
        first_str = result_str[0]
        if first_str == "":
            break;
        if first_str in lj_g_remove_string_list:
            has_error = True
            result_str = result_str[1:-1]
        else:
            break
    return result_str,has_error


def lj_check_cn_eg_data(obj,identifier):
    has_error = False
    key_list = []
    result = (lj_check_string(obj.key_cn))
    obj.key_cn = result[0]
    if result[1]:
        has_error = True
        key_list.append("key_cn")

    result = (lj_check_string(obj.key_src))
    obj.key_src = result[0]
    if result[1]:
        has_error = True
        key_list.append("key_src")

    result = (lj_check_string(obj.key_eg))
    obj.key_eg = result[0]
    if result[1]:
        has_error = True
        key_list.append("key_eg")

    result = (lj_check_string(obj.key_eg_short))
    obj.key_eg_short = result[0]
    if result[1]:
        has_error = True
        key_list.append("key_eg_short")

    if has_error:
        error_str="identifier=%s data has error character in row %d  cn=%s keys=%s"%(identifier,obj.key_row,obj.key_cn,"****".join(key_list))
        return error_str
    return ""


def read_summary_table(table,out_data_map):
    cn_col=0
    en_col=1
    eg_short_col=2
    ignore_count=0
    for row in range(1,table.nrows):
        src_cn=table.cell_value(row,cn_col)
        if not isinstance(src_cn,str) or src_cn == '':
            ignore_count = ignore_count+1
            continue

        base_info_str="(row={},src_cn={},tablename={})".format(row,src_cn,table.name)
        obj = LJCN_EG_DATA()
        obj.key_cn=src_cn
        obj.key_eg=table.cell_value(row,en_col)
        obj.key_eg_short=table.cell_value(row,eg_short_col)
        obj.key_row=row
        obj.key_table_name=table.name
        error_str = lj_check_cn_eg_data(obj,base_info_str)
        if len(error_str) > 0:
            raise(LJError(error_str))
        
        if obj.key_cn in out_data_map:
            raise("key is repetition in row={} {}".format(out_data_map[obj.key_cn].key_row,base_info_str))
        out_data_map[obj.key_cn]=obj
    
    print("ignore_count={}".format(ignore_count))


def read_result_table(table,out_data_map,summary_data_map):
    num_row = table.nrows
    src_col=2
    cn_col=1
    # print(num_col)
    # print(num_row)
    for row in range(1,num_row):
        str_cn=table.cell_value(row,cn_col)
        str_src=table.cell_value(row,src_col)
        if str_cn == '':
            print('result row={}'.format(row-1))
            break

        base_info_str="(row={},str_cn={},str_src={}tablename={})".format(row,str_cn,str_src,table.name)
        if not isinstance(str_cn,str):
            raise(LJError("str_cn is not string type {}".format(base_info_str)))
        if not isinstance(str_src,str):
            raise(LJError("str_src is not string type {}".format(base_info_str)))

        obj = LJCN_EG_DATA()
        obj.key_src=str_src
        obj.key_cn=str_cn
        obj.key_table_name=table.name
        if obj.key_cn not in summary_data_map:
            raise(LJError("src_cn not found in summary_data_map {}".format(base_info_str)))
        
        sum_obj=summary_data_map[str_cn]
        obj.key_eg=sum_obj.key_eg
        obj.key_eg_short=sum_obj.key_eg_short
        obj.key_row=row
        # print("row={}".format(row))
        if obj.key_src in out_data_map:
            raise("key is repetition in row={} {}".format(out_data_map[obj.key_src].key_row,base_info_str))
        out_data_map[obj.key_src]=obj

def read_special_table(table,out_data_map):
    num_row = table.nrows
    src_col=1
    cn_col=0
    eg_col=2
    count=0
    for row in range(1,num_row):
        str_cn=table.cell_value(row,cn_col)
        str_src=table.cell_value(row,src_col)
        str_eg=table.cell_value(row,eg_col)
        if not isinstance(str_cn,str) or str_cn == '':
            break

        base_info_str="(row={},str_cn={},str_src={},str_eg={},tablename={})".format(row,str_cn,str_src,str_eg,table.name)
        if not isinstance(str_src,str) or str_src == '':
            raise(LJError("str_src is invalid {}".format(base_info_str)))

        if not isinstance(str_eg,str) or str_eg == '':
            raise(LJError("str_eg is invalid {}".format(base_info_str)))
        
        obj = LJCN_EG_DATA()
        obj.key_src=str_src
        obj.key_cn=str_cn
        obj.key_eg=str_eg
        obj.key_row=row
        obj.key_table_name=table.name
        if obj.key_src in out_data_map:
            raise("key is repetition in row={} {}".format(out_data_map[obj.key_src].key_row,base_info_str))
        out_data_map[obj.key_src]=obj
        count = count+1
    print("add special record={}".format(count))

def read_add_new_table(table,out_data_map):
    num_row = table.nrows
    cn_col=0
    eg_col=1
    count=0
    for row in range(1,num_row):
        str_cn=table.cell_value(row,cn_col)
        str_eg=table.cell_value(row,eg_col)
        if not isinstance(str_cn,str) or str_cn == '':
            break

        base_info_str="(row={},str_cn={},str_src={},str_eg={},tablename={})".format(row,str_cn,str_eg,str_eg,table.name)

        if not isinstance(str_eg,str) or str_eg == '':
            raise(LJError("str_eg is invalid {}".format(base_info_str)))
        
        obj = LJCN_EG_DATA()
        obj.key_src=str_eg
        obj.key_cn=str_cn
        obj.key_eg=str_eg
        obj.key_row=row
        obj.key_table_name=table.name
        if obj.key_src in out_data_map:
            raise("key is repetition in row={} {}".format(out_data_map[obj.key_src].key_row,base_info_str))
        out_data_map[obj.key_src]=obj
        count = count+1
    print("add new record={}".format(count))

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

def create_codument(list_node_context,dict_ts_source,key_name,debug_data):
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
            if message_obj['source'] in dict_ts_source:
                ts_obj=dict_ts_source[message_obj['source']]
                if type(ts_obj) == dict:
                    ts_value=ts_obj[key_name]

            if ts_value == None:
                note_translation.setAttribute('type', 'unfinished')
                if not message_obj['source'].startswith('<!DOCTYPE HTML PUBLIC') and message_obj['source'] not in debug_data.list_except_trans:
                    from_str=''
                    from_dict={}
                    for i in range(len(message_obj['location'])):
                        f_str=message_obj['location'][i]['filename']
                        line_str=message_obj['location'][i]['line']
                        if f_str in from_dict:
                            from_dict[f_str] = from_dict[f_str]+',{}'.format(line_str)
                        else:
                            from_dict[f_str] = '{}'.format(line_str)
                    for (key,value) in from_dict.items():
                        from_str = from_str+'f={},line={};'.format(key,value)
                    str_error="not found source=%s,%s" % (message_obj['source'],from_str)
                    if str_error not in debug_data.list_warning:
                        debug_data.list_warning.append(str_error)
            else:
                note_translation_text = doc.createTextNode(ts_value)
                note_translation.appendChild(note_translation_text)
                if message_obj['source'] not in debug_data.list_used_key:
                    debug_data.list_used_key.append(message_obj['source']) 


            note_message.appendChild(note_translation)

            note_context.appendChild(note_message)
            message_count = message_count + 1

        if message_count <= 0:
            continue
        node_ts.appendChild(note_context)
    return doc

def update_ts(src_path,des_path,dict_ts_source,key_name,debug_data):
    dom = xml.dom.minidom.parse(src_path)
    list_node_context=[]
    itemlist = dom.getElementsByTagName('context')
    for obj in itemlist:
        list_node_context.append(parse_context(obj))

    doc = create_codument(list_node_context,dict_ts_source,key_name,debug_data)
    f = open(des_path, 'wb+')
    f.write(doc.toprettyxml(indent='\t', encoding='utf-8'))
    f.close()

def create_dict_source_map(result_data_map):
    result_data={}
    for (key,value) in result_data_map.items():
        d_obj={}
        d_obj['cn']=value.key_cn
        if value.key_eg_short == '':
            d_obj['eg']=value.key_eg
        else:
            d_obj['eg']=value.key_eg_short
        result_data[key]=d_obj
    return result_data

def read_file(path_source_file,path_ts,path_ts_out):
    try:
        work_book = xlrd.open_workbook(path_source_file)
        summary_table_name = "ALL"
        result_table_name = "结果"
        special_table_name = "特殊"
        releasenote_table_name = "releasenote"
        add_new_table_name = "本期新增"
        summary_table = work_book.sheet_by_name(summary_table_name)
        result_table = work_book.sheet_by_name(result_table_name)
        special_table = work_book.sheet_by_name(special_table_name)
        releasenote_table = work_book.sheet_by_name(releasenote_table_name)
        add_new_table = work_book.sheet_by_name(add_new_table_name)
        summary_data_map={}
        result_data_map={}
        read_summary_table(summary_table,summary_data_map)
        print("summary_data_map={}".format(len(summary_data_map)))
        read_result_table(result_table,result_data_map,summary_data_map)
        print("summary_data_map={},result_data_map={}".format(len(summary_data_map),len(result_data_map)))
        read_special_table(special_table,result_data_map)
        read_add_new_table(add_new_table,result_data_map)

        dict_source_map = create_dict_source_map(result_data_map)

        debug_data = LJDebugData()
        debug_data.list_except_trans=['Dialog','0%','*','|','45','(10%)','A、B、C、D','A、B、C、D、E','A、B、C、D、E、F','?']

        # dict_trans={'cn':'zh-CN','eg':'en-US','tw':'zh-TW'}
        dict_trans={'cn':'zh-CN','eg':'en-US'}
        # out_path='/Users/miaozw/work/ljlive/ljobs/translations'
        for (key,value) in dict_trans.items():
            des_path=os.path.join(path_ts_out,'ljlive_{}.ts'.format(value))
            update_ts(path_ts,des_path,dict_source_map,key,debug_data)

        for obj in debug_data.list_warning:
            sys.stderr.write("WARNING:{}\n".format(obj))

        for (key,value) in result_data_map.items():
            if key not in debug_data.list_used_key:
                sys.stderr.write("WARNING:key is not used {},row={},table={}\n".format(key,value.key_row,value.key_table_name))

    except LJError as e:
        sys.stderr.write("error is happend  error={}\n".format(e.value))
    # except:
    #     sys.stderr.write("error is happend\n")
    
def update(root_path):
    path_pro=os.path.join(root_path,"ljobs.pro")
    print('path_pro={}'.format(path_pro))
    if not os.path.exists(path_pro):
        sys.stderr.write("path is not exists path={}\n".format(path_pro))
        return
    path_trans=os.path.join(root_path,"translations")
    path_trans_source=os.path.join(path_trans,"all.xlsx")
    if not os.path.exists(path_trans_source):
        sys.stderr.write("path is not exists path={}\n".format(path_trans_source))
        return

    name_ts='ljlive_zh_CN.ts'
    name_ts_temp='ljlive_TEMP.ts'
    path_ts=os.path.join(path_trans,name_ts)
    path_ts_temp=os.path.join(path_trans,name_ts_temp)
    # if os.path.exists(path_ts):
    #     os.remove(path_ts)
    # if os.path.exists(path_ts):
    #     sys.stderr.write("path can not remove path={}\n".format(path_ts_temp))
    #     return

    # if os.path.exists(path_ts_temp):
    #     os.remove(path_ts_temp)
    # if os.path.exists(path_ts_temp):
    #     sys.stderr.write("path can not remove path={}\n".format(path_ts_temp))
    #     return
    
    # os.system('lupdate \'{}\''.format(path_pro))
    # if not os.path.exists(path_ts):
    #     sys.stderr.write("path is not exists path={}\n".format(path_ts))
    #     return
    # shutil.move(path_ts,path_ts_temp)
    if not os.path.exists(path_ts_temp):
        sys.stderr.write("path is not exists path={}\n".format(path_ts_temp))
        return
    read_file(path_trans_source,path_ts_temp,path_trans)
    # trans_path='/Users/miaozw/work/ljlive/ljobs/translations'
    # real_file(trans_path+'/all.xlsx')

update('/Users/miaozw/work/ljlive/ljobs')