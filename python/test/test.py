from xlutils.copy import copy
import xlrd
import xlwt

#xlutils:修改excel
file_path = '/Users/miaozw/Documents/TEMP/test.xls'
book1 = xlrd.open_workbook(file_path)
book2 = copy(book1)#拷贝一份原来的excel

print(type(book2))
# print(dir(book2))
sheet = book2.get_sheet(0)#获取第几个sheet页，book2现在的是xlutils里的方法，不是xlrd的

temp = sheet.last_used_row
temp2 = sheet.first_used_row
for row in range(3,sheet.last_used_row):
    sheet.write(row,3,'',)

book2.save(file_path)
