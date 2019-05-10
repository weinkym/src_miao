

file_object = open('/Users/miaozw/Documents/TEMP/13616511205.txt', 'r')

line_string = file_object.readline()
print(type(line_string))
cpu_string_list = []
cpu_found = False
line = ''
while line_string:
    # print(type(line_string))
    line_string = file_object.readline()
    data_list = line_string.split('@')
    if len(data_list) < 9:
        continue
    print(data_list)

