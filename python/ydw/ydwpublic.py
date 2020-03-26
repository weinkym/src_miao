import datetime
import time

class ToString:
    def getDescription(self):
        #利用str的format格式化字符串
        #利用生成器推导式去获取key和self中key对应的值的集合
        return ",".join("{}={}".format(key,getattr(self,key)) for key in self.__dict__.keys())
    #重写__str__定义对象的打印内容
    def __str__(self):
        return "{}->({})".format(self.__class__.__name__,self.getDescription())

class YDWLoansData(ToString):
    def __init__(self):
        self.m_datetime_expire = datetime.datetime(1970, 1, 1, 0, 0, 0)
        self.m_datetime_loan = datetime.datetime(1970, 1, 1, 0, 0, 0)
        self.m_name = ""
        self.m_project_code = ""
        self.m_month = 0
        self.m_day = 0
        self.m_amount = 0
        self.m_rate = 0.0
        self.m_aduest_day = 0


def line_to_datetime(line_string):
    data_list = line_string.split('.')
    if len(data_list) != 3:
        return

    t = time.strptime(line_string, "%Y.%m.%d")
    return datetime.datetime(t.tm_year,t.tm_mon,t.tm_mday,0,0,0)

def ydw_datetime_add(src_datetime,add_month,add_day):
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


def data_list_to_obj(data_list):
    if len(data_list) < 9:
        return
    obj = YDWLoansData()
    obj.m_project_code = data_list[1]
    obj.m_datetime_loan = line_to_datetime(data_list[2])
    # print(obj.m_datetime_loan)
    obj.m_amount = (float)(data_list[5].replace(',',''))
    key_day = "天"
    key_month = "个月"
    data_line = data_list[6]
    if key_day in data_line:
        obj.m_day = int(data_line.replace(key_day, ""))
    elif key_month in data_line:
        obj.m_month = int(data_line.replace(key_month, ""))
    else:
        print('data has error ',data_line)
        return
    obj.m_rate = (float)(data_list[7].replace('%',''))

    temp_var = ydw_datetime_add(obj.m_datetime_loan, obj.m_month, obj.m_day)
    obj.m_datetime_expire = temp_var[0]
    obj.m_aduest_day = temp_var[1]
    return obj


def ydw_read_user_data(filepath):
    file_object = open(filepath, 'r')
    obj_list = []
    line_row = 0
    while True:
        line_row = line_row + 1
        # print(type(line_string))
        line_string = file_object.readline()
        if not line_string:
            break
        # print(line_string)
        data_list = line_string.split('@')
        # print(len(data_list))
        if len(data_list) < 9:
            continue
        # print(type(data_list))
        obj = data_list_to_obj(data_list)
        if not isinstance(obj, YDWLoansData):
            print('line string is error,row=%d,str=%s' % (line_row, line_string))
            continue

        obj_list.append(obj)

    return obj_list


# p = YDWLoansData()
# print(p)