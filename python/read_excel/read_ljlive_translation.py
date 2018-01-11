import xlrd

from langconv import *

class LJCN_EG_DATA:
    key_src = ""
    key_cn = ""
    key_eg = ""
    key_eg_short = ""
    key_count = 1
    key_row = 0

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
        print("identifier=%s data has error character in row %d  cn=%s keys=%s"%(identifier,obj.key_row,obj.key_cn,"****".join(key_list)))
    return obj


def lj_read_summary_sheet(table):
    result_data = {}
    num_col = table.ncols
    num_row = table.nrows
    if num_col < 3:
        print("col is invalid")
        return

    for row in range(0,num_row):
        obj = LJCN_EG_DATA()
        obj.key_cn = table.cell_value(row, 0)
        obj.key_eg = table.cell_value(row, 1)
        obj.key_eg_short = table.cell_value(row, 2)
        obj.key_row = row
        obj = lj_check_cn_eg_data(obj,"lj_read_summary_sheet")

        if obj.key_cn == "":
            continue
        if obj.key_eg == "" and obj.key_eg_short == "":
            print("obj.key_eg and obj.key_eg_short is empty in row ",row)
            continue

        if obj.key_cn in result_data:
            print("key_cn %s is exist in row = %d and %d"%(obj.key_cn,row,result_data[obj.key_cn].key_row))
            continue
        result_data[obj.key_cn] = obj

    return result_data


def lj_read_source_sheet(table,summary_data):
    result_data = {}
    num_col = table.ncols
    num_row = table.nrows
    if num_col < 3:
        print("col is invalid")
        return

    for row in range(0,num_row):
        obj = LJCN_EG_DATA()
        obj.key_cn = table.cell_value(row, 1)
        obj.key_src = table.cell_value(row, 2)
        obj.key_row = row
        obj = lj_check_cn_eg_data(obj,"lj_read_source_sheet")
        if obj.key_cn == "":
            continue
        if obj.key_src == "":
            print("key_src is empty in row ",row)
            continue

        if obj.key_src in result_data:
            print("key_src %s is exist in row = %d and %d"%(obj.key_src,row,result_data[obj.key_src].key_row))
            continue
        if obj.key_cn not in summary_data:
            print("key_cn=%s is not found in row=%d "%(obj.key_cn,obj.key_row))
            continue
        obj.key_eg = summary_data[obj.key_cn].key_eg
        obj.key_eg_short = summary_data[obj.key_cn].key_eg_short
        result_data[obj.key_src] = obj

    return result_data

def lj_read_special_sheet(table):
    result_data = {}
    num_col = table.ncols
    num_row = table.nrows
    if num_col < 3:
        print("col is invalid col=",num_col)
        return result_data

    for row in range(0, num_row):
        obj = LJCN_EG_DATA()
        obj.key_cn = table.cell_value(row, 0)
        obj.key_src = table.cell_value(row, 1)
        obj.key_eg = table.cell_value(row, 2)
        obj.key_row = row
        obj = lj_check_cn_eg_data(obj,"lj_read_special_sheet")
        if obj.key_cn == "":
            continue
        if obj.key_src == "":
            print("key_src is empty in row ", row)
            continue

        if obj.key_src in result_data:
            print("key_src %s is exist in row = %d and %d" % (obj.key_src, row, result_data[obj.key_src].key_row))
            continue

        result_data[obj.key_src] = obj

    return result_data

def lj_test(summary_table,source_table,special_table):
    summary_data = lj_read_summary_sheet(summary_table)
    source_data = lj_read_source_sheet(source_table,summary_data)
    special_data = lj_read_special_sheet(special_table)
    print(len(summary_data))
    print(len(source_data))
    print(len(special_data))

    file_object = open('/Users/miaozw/work/ljlive/ljobs/translations/language.txt', 'w')

    all_data = dict(source_data, **special_data)
    for (d, obj) in all_data.items():
        separator = "#----#"
        tw = obj.key_cn
        tw = Converter('zh-hant').convert(tw)
        tw.encode('utf-8')
        eg = obj.key_eg
        if obj.key_eg_short != "":
            eg = obj.key_eg_short

        mylist = [obj.key_src,obj.key_cn,tw,eg]

        file_object.write(separator.join(mylist) + "\n")
        # sprint("%s %s %s %s %s" % (obj.key_cn,obj.key_src,tw,obj.key_eg,obj.key_eg_short))
    file_object.close()


filename = "/Users/miaozw/work/ljlive/ljobs/translations/all.xlsx"
summary_table_name = "ALL"
source_table_name = "结果"
special_table_name = "特殊"

work_book = xlrd.open_workbook(filename)
summary_table = work_book.sheet_by_name(summary_table_name)
source_table = work_book.sheet_by_name(source_table_name)
special_table = work_book.sheet_by_name(special_table_name)

lj_test(summary_table,source_table,special_table)
# tempstr="asdfadf"
# tempstr = lj_check_string(tempstr)
# print(tempstr[0])
# print(tempstr[1])
# print("tttt")


