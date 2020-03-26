import xlrd
import time
import datetime
import math
import json
import xlwt

class ZWLoansData:
    m_datetime = datetime.datetime(1970, 1, 1, 0, 0, 0)
    m_datetime_loan = datetime.datetime(1970, 1, 1, 0, 0, 0)
    m_name = ""
    m_project_code = ""
    m_month = 0
    m_day = 0
    m_amount = 0
    m_rate = 0.0
    m_aduest_day = 0

def zw_datetime_add(src_datetime,add_month,add_day):
    year = src_datetime.year + (src_datetime.month + add_month + 1) // 12
    month = (src_datetime.month + add_month) % 12 + 1

    temp_datetime = datetime.datetime(year, month, 1, src_datetime.hour, src_datetime.minute, src_datetime.second)
    temp_datetime = temp_datetime + datetime.timedelta(days=-1)
    # print(temp_datetime)
    adjuest_day = 0
    dd = src_datetime.day - temp_datetime.day
    if dd > 0:
        adjuest_day = dd

    temp_datetime = temp_datetime + datetime.timedelta(days=(dd + add_day))
    return temp_datetime,adjuest_day

def read_xls(filename,sheet_name):
    work_book = xlrd.open_workbook(filename)
    table = work_book.sheet_by_name(sheet_name)
    num_col = table.ncols
    num_row = table.nrows
    start_col = 0
    key_day = "天"
    key_month = "个月"
    print("num_row=%d,num_col=%d" % (num_row, num_col))
    result_data = {}
    for row in range(0,num_row // 2):
        col_index = start_col
        obj = ZWLoansData()
        obj.m_name = sheet_name
        obj.m_project_code = table.cell_value(2 * row + 1,col_index)

        if obj.m_project_code.strip() == '':
            continue
        if obj.m_project_code in result_data:
            print("%s is exit" % obj.m_project_code)
            continue

        data_string = table.cell_value(2 * row,col_index+1)
        data_string_list = data_string.split(".")
        # print("data_string=%s" % (data_string))
        # print("data_string",data_string)
        if len(data_string_list) != 3:
            continue
        year = int(data_string_list[0],10)
        month = int(data_string_list[1],10)
        day = int(data_string_list[2], 10)

        obj.m_datetime_loan = datetime.datetime(year, month, day, 0, 0, 0)
        obj.m_amount = table.cell_value(2 * row,col_index+4)
        if (type(obj.m_amount) != float):
            continue
        if obj.m_amount <= 0.00001:
            continue

        date_len = table.cell_value(2 * row,col_index+5)

        if key_day in date_len:
            obj.m_day = int(date_len.replace(key_day,""))
        elif key_month in date_len:
            obj.m_month = int(date_len.replace(key_month, ""))
        else:
            continue

        obj.m_rate = table.cell_value(2 * row, col_index+6)
        if (type(obj.m_rate) != float):
            continue

        temp_var = zw_datetime_add(datetime.datetime(year, month, day, 0, 0, 0),obj.m_month,obj.m_day)
        obj.m_datetime = temp_var[0]
        obj.m_aduest_day = temp_var[1]
        # print(type(obj.m_project_code))
        # print("obj.m_rate",obj.m_rate)
        # print("datetime=%s" % obj.m_datetime.strftime("%Y%m%d %H:%M:%S"))
        # print("obj.m_day", obj.m_day)
        # print("obj.m_month", obj.m_month)
        # print("obj.m_amount", obj.m_amount)
        if obj.m_aduest_day != 0:
            print("project_code=%s aduest_day=%d" % (obj.m_project_code,obj.m_aduest_day))
        if obj.m_datetime < datetime.datetime.now():
            print("%s time is out " % obj.m_project_code)
            continue
        result_data[obj.m_project_code] = obj
    return result_data

def write_data(data_obj,path):
    wb = xlwt.Workbook()
    print("data_obj count=", len(data_obj))
    # return

    sheet = wb.add_sheet("xlwt3数据测试表")
    row = 0
    for i in range(0,len(data_obj)):
        dict = data_obj[i]
        for (d, obj) in dict.items():
            # print(type(obj))
            sheet.write(row, 0, obj.m_name)
            sheet.write(row, 1, obj.m_project_code)
            sheet.write(row, 2, obj.m_amount)
            sheet.write(row, 3, obj.m_datetime_loan)
            sheet.write(row, 4, obj.m_rate * 100)
            sheet.write(row, 5, obj.m_month)
            sheet.write(row, 6, obj.m_day)
            sheet.write(row, 7, obj.m_aduest_day)
            sheet.write(row, 8, obj.m_datetime)
            row = row + 1
    wb.save(path)



sheet_name_list = ["MZW","ZLL","ZP"]
result_data = []
for name in sheet_name_list:
    result_data.append(read_xls("/Users/miaozw/Documents/test.xlsx",name))
print("result_data count=", len(result_data))

write_data(result_data,"/Users/miaozw/Documents/0001.xls")

# temp_date = datetime.datetime(2017, 10, 31, 1, 2, 3)
# temp_date2 = zw_datetime_add(temp_date,1,0)
# print("datetime=%s" ,temp_date2)
# temp_date2 = temp_date + datetime.timedelta(month=add_mount)
# print(datetime.datetime.now())