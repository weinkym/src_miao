from ydw.ydwpublic import YDWLoansData,ydw_read_user_data

import datetime
import time
import xlwt
import os

def write_data(data_dict,path):
    wb = xlwt.Workbook()
    print("data_dict count=", len(data_dict))
    # return

    sheet = wb.add_sheet("data")
    row = 0
    for (d, obl_list) in data_dict.items():
        for obj in obl_list:
            sheet.write(row, 0, d)
            sheet.write(row, 1, obj.m_project_code)
            sheet.write(row, 2, obj.m_amount)
            sheet.write(row, 3, obj.m_datetime_loan)
            sheet.write(row, 4, obj.m_rate)
            sheet.write(row, 5, obj.m_month)
            sheet.write(row, 6, obj.m_day)
            sheet.write(row, 7, obj.m_aduest_day)
            sheet.write(row, 8, obj.m_datetime_expire)
            row = row + 1

    wb.save(path)


all_user_dict = {}
all_user_dict['MZW'] = '13616511205'
all_user_dict['ZLL'] = '15858239032'
all_user_dict['ZP'] = '15267080236'
data_dict = {}

for (name, num) in all_user_dict.items():
    path = '/Users/miaozw/Documents/TEST/%s.txt' % (num)
    # path = ''.format('/Users/miaozw/Documents/TEST/%s.txt',num)
    print(path)
    if not os.path.exists(path):
        print('path is not exists %s',path)
        continue
    obj_list = ydw_read_user_data(path)
    print(len(obj_list))
    data_dict[name] = obj_list

write_data(data_dict,'/Users/miaozw/Documents/TEST/temp.xls')



