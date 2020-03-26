import xlrd
import time
import datetime
import math
import json


class ZWData:
    m_name = ""
    m_datetime = datetime.datetime(1970,1,1,0,0,0)
    m_content = ""
    m_total_amount=0.0

def readData(filename, sheet_name):
    work_book = xlrd.open_workbook(filename)
    table = work_book.sheet_by_name(sheet_name)
    num_col = table.ncols
    num_row = table.nrows
    print("num_row=%d,num_col=%d" % (num_row,num_col))
    result_data = {}
    # print("result_data=", type(result_data))

    for row in range(3,num_row):
        project_name = table.cell_value(row,1)
        if project_name.strip() == '':
            continue
        # print("project_name=",project_name)
        project_datetime = table.cell_value(row,13)
        project_status = table.cell_value(row,10)
        is_finished = math.isclose(project_status,1.0)
        project_amount = table.cell_value(row,12)
        if is_finished == True:
            continue
        if(type(project_datetime) != float):
            continue
        # excel 1 等于1900-1-1
        days = int(project_datetime) - 2
        sces = int((project_datetime - int(project_datetime)) * 24 * 60 * 60)
        bron_day = datetime.datetime(1900, 1, 1, 0, 0, 0)
        bron_day += datetime.timedelta(days=days,seconds=sces)
        print("TTT",table.cell_value(row,2))
        # print("TTT", bron_day.strftime("%Y-%m-%d %H:%M:%S"))
        print("TTT", bron_day.strftime("%Y-%m-%d"))
        key_name = project_name + "_" + bron_day.strftime("%Y-%m-%d")
        # obj = result_data[key_name]
        if key_name not in result_data:
            result_data[key_name] = ZWData()
        result_data[key_name].m_name = key_name
        result_data[key_name].m_datetime = bron_day
        result_data[key_name].m_total_amount += project_amount
        print("m_total_amount=", result_data[key_name].m_total_amount)
    return result_data

result_data = readData("/Users/miaozw/Documents/test.xlsx","sheet01")


# for key in result_data:
#     print("key = ",key)
#     print("amount=",result_data[key].m_total_amount)

project_list = []

for key in result_data:
    item_data = {}
    item_data['type'] = 1
    item_data['content'] = result_data[key].m_name + " 金额: "+str(int(result_data[key].m_total_amount))
    item_data['dateTime'] = result_data[key].m_datetime.strftime("%Y%m%d %H:%M:%S")
    project_list.append(item_data)

print("=================")
print(project_list)

message_data = {}
message_data['add_message_list'] = project_list

print("=================")
print(message_data)
print("=================")

print(json.dumps(message_data))

file_object = open('/Users/miaozw/Documents/test.txt','w')
file_object.write(json.dumps(message_data))
file_object.close()

#
#
#
#
#
#
#
#
# data = xlrd.open_workbook("/Users/miaozw/Documents/test.xlsx")
# length = len(data.sheets())
# print('len=',length)
#
# for i in range(0,length):
#     table = data.sheets()[i]
#     print(table.nrows)
#     print(table.ncols)
#     print(table.name)
#     for row in range(0,table.nrows):
#         for col in range(0,table.ncols):
#             cell_value = table.cell(row,col).value
#             print(type(cell_value))
#             print(cell_value)
#             if(type(cell_value) == float):
#                 print("GGGGGGGGGG")
#                 sces = int((cell_value - int(cell_value)) * 24 * 60 * 60)
#                 print("sces=", sces)
#
#                 bron_day = datetime.datetime(1900, 1, 1,0,0,0)
#                 bron_day += datetime.timedelta(days=cell_value)
#                 print("TTT", bron_day.strftime("%Y-%m-%d %H:%M:%S"))
#                 # time_day = datetime.time(0, 0, 0)
#                 # bron_day += time.timedelta(days=cell_value)
#                 # print("TTT", time_day.strftime("%Y-%m-%d %H:%M:%S"))
#                 # bron_day += datetime.timedelta(seconds=sces)
#                 # print("TTT", bron_day.strftime("%Y-%m-%d %H:%M:%S"))
#
#                 # time_day = datetime.time(0,0,0)
#                 #
#                 # time_day += time_day.timedelta(seconds=sces)
#                 #
#                 # print("TTT", time_day.strftime("%Y-%m-%d %H:%M:%S"))
#
# print('end')

#result.group(0,0)
