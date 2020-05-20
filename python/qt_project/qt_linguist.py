import xlrd
import sys
import os
import xml.etree.ElementTree as ET
from enum import Enum
import time
import zhconv
import copy

sys.path.append(os.getcwd())
import subprocess
from zwpy import zwutil as UTils


class LogLevel(Enum):
    debug = 1
    info = 2
    warning = 3
    error = 4


def doLog(level, msg):
    print('{}: {}'.format(level, msg))


class TSObject:
    def __init__(self):
        self.version = ''
        self.language = ''
        self.sourcelanguage = ''
        self.tail = '\n'
        self.text = ''
        self.content_list = []

    def saveToFile(self, fp):
        # root = ET.Element('TS')
        root = createElement('TS')
        root.tail = self.tail
        # root.text = self.text
        root.set('version', self.version)
        root.set('language', self.language)
        root.set('sourcelanguage', self.sourcelanguage)
        for obj in self.content_list:
            element = ET.Element('TS1')
            root.append(obj.toElement())
        prettyXml(root, '\t', '\n', 0)
        tree = ET.ElementTree(root)
        tree.write(fp,
                   encoding='UTF-8',
                   xml_declaration=True,
                   short_empty_elements=False)
        # tree.write(fp,encoding='UTF-8', xml_declaration=True,method='html')


#参考 https://www.cnblogs.com/huzixia/p/10391987.html
def prettyXml(element, indent, newline, level=0):
    if element:
        if element.text == None or element.text.isspace():
            element.text = newline + indent * (level + 1)
        else:
            element.text = newline + indent * (level + 1) + element.text.strip(
            ) + newline + indent * (level + 1)
    temp = list(element)
    for subelement in temp:
        if temp.index(subelement) < (len(temp) - 1):
            subelement.tail = newline + indent * (level + 1)
        else:
            subelement.tail = newline + indent * level
        prettyXml(subelement, indent, newline, level=level + 1)


def createElement(name, parent=None):
    element = ET.Element(name)
    if parent != None:
        parent.append(element)
    return element


class TSMessage:
    def __init__(self, other=None):
        if other is None:
            self.filename = ''
            self.line = ''
            self.source = ''
            self.translation = ''
            self.type = ''
            self.tail = '\n'
            self.location_tail = '\n'
            self.source_tail = '\n'
            self.translation_tail = '\n'
            self.text = ''
            self.location_text = ''
            self.source_text = ''
            self.translation_text = ''
        else:
            self.filename = other.filename
            self.line = other.line
            self.source = other.source
            self.translation = other.translation
            self.type = other.type
            self.tail = other.tail
            self.location_tail = other.location_tail
            self.source_tail = other.source_tail
            self.translation_tail = other.translation_tail
            self.text = other.text
            self.location_text = other.location_text
            self.source_text = other.source_text
            self.translation_text = other.translation_text

    def __str__(self):
        return 'source={},line={},filename={},translation={},type={}'.format(
            self.source, self.line, self.filename, self.translation, self.type)

    def __copy__(self):
        return self

    def toElement(self):
        element = createElement('message')

        location_element = createElement('location', element)
        location_element.set('filename', self.filename)
        location_element.set('line', self.line)

        source_element = createElement('source', element)
        source_element.text = self.source

        translation_element = createElement('translation', element)
        translation_element.text = self.translation
        if '' == self.translation or self.translation is None:
            translation_element.set('type', 'unfinished')
        return element


class TSContent:
    def __init__(self):
        self.name = ''
        self.tail = '\n'
        self.text = ''
        self.message_list = []

    def toElement(self):
        element = createElement('context')
        name_element = createElement('name', element)
        name_element.text = self.name
        for obj in self.message_list:
            element.append(obj.toElement())
        return element


def parseMessage(obj):
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
        message.type = tag.get('type', '')
        message.translation_tail = tag.tail
        message.translation_text = tag.text

    return message


def parseContent(obj):
    # print('obj.text={},len={}'.format(obj.text, len(obj.text)))
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
    obj.version = root.get('version', '')
    obj.language = root.get('language', '')
    obj.sourcelanguage = root.get('sourcelanguage', '')
    for tg_context in root.iter("context"):
        content = parseContent(tg_context)
        obj.content_list.append(content)
    return obj


#excel==========================================================================================
def isEmpty(obj):
    if obj == None or '' == obj:
        return True
    return False


lj_g_remove_string_list = ["\n", "\t", " "]


def adjuestString(string):
    str = string
    has_error = False
    if str == "":
        return str, has_error
    while (1):
        last_str = str[-1]
        if last_str == "":
            break
        if last_str in lj_g_remove_string_list:
            has_error = True
            str = str[0:-2]
        else:
            break

    if str == "":
        return str, has_error
    while (1):
        first_str = str[0]
        if first_str == "":
            break
        if first_str in lj_g_remove_string_list:
            has_error = True
            str = str[1:-1]
        else:
            break
    return str, has_error


def getTableCellValue(tb, row, col):
    value = tb.cell_value(row, col)
    # doLog(LogLevel.debug,
    #       "table={},row={},col={},value={}".format(tb.name, row, col, value))
    result = (adjuestString(str(value)))
    if result[1]:
        doLog(
            LogLevel.warning,
            'table value is invalid,check \"empty\" \"\\t\" on both ends ,table={},row={},col={},value={}'
            .format(tb.name, row, col, value))
    return result[0]


class ETSData:
    def __init__(self, table_name, row):
        self.source_id = ''
        self.cn = ''
        self.eg_short = ''
        self.eg_all = ''
        self.row = row
        self.table_name = table_name

    def __str__(self):
        return 'table={},row={},source_id={},cn={},eg_short={},eg_all={}'.format(
            self.table_name, self.row, self.source_id, self.cn, self.eg_short,
            self.eg_all)

    def getEG(self):
        if isEmpty(self.eg_short):
            return self.eg_all
        return self.eg_short

    def __doInvalidKeyLog(self, key):
        doLog(
            LogLevel.warning,
            "{} is invalid,table={},row={}".format(key, self.table_name,
                                                   self.row))

    def checkValid(self, is_source_data):
        if isEmpty(self.cn):
            if is_source_data:
                self.__doInvalidKeyLog('cn')
            return False

        if isEmpty(self.source_id) and is_source_data:
            self.__doInvalidKeyLog('source_id')
            return False

        if isEmpty(self.getEG()) and not is_source_data:
            self.__doInvalidKeyLog('eg')
            return False
        return True


def parseSummaryTable(tb):
    num_col = tb.ncols
    num_row = tb.nrows
    tb_dict = {}
    for row in range(1, num_row):
        obj = ETSData(tb.name, row)
        obj.cn = getTableCellValue(tb, row, 0)
        obj.eg_all = getTableCellValue(tb, row, 1)
        obj.eg_short = getTableCellValue(tb, row, 2)
        if not obj.checkValid(False):
            continue
        if obj.cn in tb_dict.keys():
            doLog(
                LogLevel.warning,
                '{} at row({},{})'.format(obj.cn, obj.row,
                                          tb_dict[obj.cn].row))
            continue

        tb_dict[obj.cn] = obj
        # doLog(LogLevel.info, row)
        # doLog(LogLevel.info, obj.cn)
    return tb_dict


def parseSpecialTable(tb):
    num_col = tb.ncols
    num_row = tb.nrows
    tb_dict = {}
    for row in range(1, num_row):
        obj = ETSData(tb.name, row)
        obj.cn = getTableCellValue(tb, row, 0)
        obj.source_id = getTableCellValue(tb, row, 1)
        obj.eg_all = getTableCellValue(tb, row, 2)
        obj.eg_short = getTableCellValue(tb, row, 2)

        if not obj.checkValid(False):
            continue
        if not obj.checkValid(True):
            continue
        if obj.source_id in tb_dict.keys():
            doLog(
                LogLevel.warning,
                '{} at row({},{})'.format(obj.source_id, obj.row,
                                          tb_dict[obj.cn].row))
            continue
        tb_dict[obj.source_id] = obj
    return tb_dict


def parseSourceTable(tb):
    num_col = tb.ncols
    num_row = tb.nrows
    tb_dict = {}
    for row in range(1, num_row):
        obj = ETSData(tb.name, row)
        obj.cn = getTableCellValue(tb, row, 1)
        obj.source_id = getTableCellValue(tb, row, 2)
        # obj.eg_short = getTableCellValue(tb, row, 2)
        if not obj.checkValid(True):
            continue
        if obj.cn in tb_dict.keys():
            doLog(
                LogLevel.warning,
                '{} at row({},{})'.format(obj.cn, obj.row,
                                          tb_dict[obj.cn].row))
            continue

        tb_dict[obj.cn] = obj

    return tb_dict


def parseTsExcelFile(fp, summary_table_name, source_table_name,
                     special_table_name):

    work_book = xlrd.open_workbook(fp)
    summary_table = work_book.sheet_by_name(summary_table_name)
    source_table = work_book.sheet_by_name(source_table_name)
    special_table = work_book.sheet_by_name(special_table_name)
    # releasenote_table = work_book.sheet_by_name(releasenote_table_name)
    summary_tb_dict = parseSummaryTable(summary_table)
    source_tb_dict = parseSourceTable(source_table)
    result_tb_dict = parseSpecialTable(special_table)

    # print('type={}'.format(type(source_tb_dict)))
    for (key, obj) in source_tb_dict.items():
        # print('key={}'.format(key))
        if obj.source_id in result_tb_dict:
            doLog(
                LogLevel.warning,
                'source_id repeat {},{}'.format(obj,
                                                result_tb_dict[obj.source_id]))
            continue
        if key not in summary_tb_dict:
            doLog(LogLevel.warning, 'not found TS info {}'.format(obj))
            continue
        summary_obj = summary_tb_dict[key]
        obj.eg_all = summary_obj.eg_all
        obj.eg_short = summary_obj.eg_short
        result_tb_dict[obj.source_id] = obj
    return result_tb_dict


def fileTime(file):
    return [
        time.ctime(os.path.getatime(file)),
        time.ctime(os.path.getmtime(file)),
        time.ctime(os.path.getctime(file))
    ]


def updateTs(pro_fp, check_ts_fp, lupdate_fp):
    if not os.path.exists(lupdate_fp):
        doLog(LogLevel.error,
              'lupdate file path is not exists fp={}'.format(lupdate_fp))
        return False

    if not os.path.exists(pro_fp):
        doLog(LogLevel.error,
              'pro file path is not exists fp={}'.format(pro_fp))
        return False

    old_time = 0
    if os.path.exists(check_ts_fp):
        old_time = os.path.getmtime(check_ts_fp)

    res = UTils.run_cmd('\"{}\" \"{}\"'.format(lupdate_fp, pro_fp))
    doLog(LogLevel.info, res)
    if not os.path.exists(lupdate_fp):
        doLog(LogLevel.error,
              'check file is not exists fp={}'.format(check_ts_fp))
        return False
    cur_time = os.path.getmtime(check_ts_fp)
    dx_time = cur_time - old_time
    if dx_time < 0.01:
        doLog(LogLevel.error,
              'check file is not updated fp={}'.format(check_ts_fp))
        return False
    doLog(
        LogLevel.info, "{} is updated at {}".format(
            check_ts_fp,
            time.strftime("%Y-%m-%d %H:%M:%S", time.localtime(cur_time))))
    return True


def update(ts_data_obj, pro_name, lrelease_fp, out_dp):
    fp = os.path.join(out_dp, '{}_{}.ts'.format(pro_name,
                                                ts_data_obj.language))
    ts_data_obj.saveToFile(fp)
    res = UTils.run_cmd('\"{}\" \"{}\"'.format(lrelease_fp, fp))
    doLog(LogLevel.info, res)


def doUpdate(pro_fp, source_ts_fp, source_excel_fp, lupdate_fp, lrelease_fp):
    if not os.path.exists(lrelease_fp):
        doLog(LogLevel.error, '{} is not exists'.format(lrelease_fp))
        return False
    if not updateTs(pro_fp, source_ts_fp, lupdate_fp):
        return False

    ts_data_obj = parseTsFile(source_ts_fp)
    summary_table_name = "ALL"
    source_table_name = "已汉化"
    special_table_name = "特殊"
    # releasenote_table_name = "releasenote"
    result_tb_dict = parseTsExcelFile(source_excel_fp, summary_table_name,
                                      source_table_name, special_table_name)

    g_excep_source_id_list = [
        'TextLabel', 'Form', 'TextLabel', 'Dialog', 'PushButton', 'CheckBox'
    ]
    ts_data_obj_eg = TSObject()
    ts_data_obj_eg.version = ts_data_obj.version
    ts_data_obj_eg.language = 'en_US'
    ts_data_obj_eg.sourcelanguage = 'en'

    ts_data_obj_cn = TSObject()
    ts_data_obj_cn.version = ts_data_obj.version
    ts_data_obj_cn.language = 'zh_CN'
    ts_data_obj_cn.sourcelanguage = 'zh'

    ts_data_obj_tw = TSObject()
    ts_data_obj_tw.version = ts_data_obj.version
    ts_data_obj_tw.language = 'zh_TW'
    ts_data_obj_tw.sourcelanguage = 'zh'

    for content in ts_data_obj.content_list:
        content_eg = TSContent()
        content_eg.name = content.name

        content_cn = TSContent()
        content_cn.name = content.name

        content_tw = TSContent()
        content_tw.name = content.name

        for message in content.message_list:
            #todo 待完善
            if '../obs/UI' in message.filename and 'OBSBasic.ui' not in message.filename:
                continue
            if message.source in g_excep_source_id_list:
                continue
            if message.source not in result_tb_dict:
                doLog(LogLevel.error, 'source not found {}'.format(message))
                continue
            ts_data = result_tb_dict[message.source]

            message.translation = ts_data.getEG()
            content_eg.message_list.append(TSMessage(message))

            message.translation = ts_data.cn
            content_cn.message_list.append(TSMessage(message))

            message.translation = zhconv.convert(ts_data.cn, 'zh-tw')
            content_tw.message_list.append(TSMessage(message))

        ts_data_obj_eg.content_list.append(content_eg)
        ts_data_obj_cn.content_list.append(content_cn)
        ts_data_obj_tw.content_list.append(content_tw)

    pro_name = os.path.basename(pro_fp)
    pro_name = pro_name.replace('.pro', '')
    pro_name = 'ljlive'
    out_dp = os.path.dirname(source_ts_fp)

    update(ts_data_obj_eg, pro_name, lrelease_fp, out_dp)
    update(ts_data_obj_cn, pro_name, lrelease_fp, out_dp)
    update(ts_data_obj_tw, pro_name, lrelease_fp, out_dp)


def doUpdateReleasenote(fp, excel_fp):
    work_book = xlrd.open_workbook(excel_fp)
    tb = work_book.sheet_by_name("releasenote")
    if tb is None:
        doLog(LogLevel.info, 'releasenote table is none')
        return False
    num_col = tb.ncols
    num_row = tb.nrows
    with open(fp, 'w') as f:
        for row in range(2, num_row):
            key = getTableCellValue(tb, row, 0)
            cn = getTableCellValue(tb, row, 1)
            eg = getTableCellValue(tb, row, 2)
            if key == 'END':
                break
            f.write('CN{}{}\n'.format(key, cn))
            f.write('TW{}{}\n'.format(key, zhconv.convert(cn, 'zh-tw')))
            f.write('EG{}{}\n'.format(key, eg))


if __name__ == "__main__":
    # excel_fp = '/Users/avc/Documents/TEMP/all.xlsx'
    excel_fp = '/Users/avc/work/ljlive222/ljobs/translations/all.xlsx'
    # ts_fp = '/Users/avc/Documents/TEMP/ljlive_zh_CN.ts'
    pro_fp = '/Users/avc/work/ljlive222/ljobs/ljobs.pro'
    check_ts_fp = '/Users/avc/work/ljlive222/ljobs/translations/ljlive_zh_CN.ts'
    releasenote_fp = '/Users/avc/work/ljlive222/ljobs/translations/releasenote.txt'
    lupdate_fp = '/Users/avc/Qt5.7.1/5.7/clang_64/bin/lupdate'
    lrelease_fp = '/Users/avc/Qt5.7.1/5.7/clang_64/bin/lrelease'
    doUpdate(pro_fp, check_ts_fp, excel_fp, lupdate_fp, lrelease_fp)
    # doUpdateReleasenote(releasenote_fp, excel_fp)
