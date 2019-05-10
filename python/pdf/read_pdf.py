import sys
# sys.setdefaultencoding('utf-8')
import os
import os.path

from pdfminer.pdfparser import PDFParser,PDFDocument
from pdfminer.pdfinterp import PDFResourceManager, PDFPageInterpreter
from pdfminer.converter import PDFPageAggregator
from pdfminer.layout import LTTextBoxHorizontal,LAParams
from pdfminer.pdfinterp import PDFTextExtractionNotAllowed



def getFileNames(file_path,suffix):
    file_list=[]
    for root,dirs,files in os.walk(file_path,False):
        if root != file_path :
            continue

        for name in files:
            print('文件名：%s' % name)
            path=os.path.join(root, name)
            print('文件完整路径：%s\n' % path)
            fp = open(path, 'rb')  # 以二进制读模式打开
            # 用文件对象来创建一个pdf文档分析器
            praser = PDFParser(fp)
            # 创建一个PDF文档
            doc = PDFDocument()
            # 连接分析器 与文档对象
            praser.set_document(doc)
            doc.set_parser(praser)

            # 提供初始化密码
            # 如果没有密码 就创建一个空的字符串
            doc.initialize('15858239032')

            # 创建PDf 资源管理器 来管理共享资源
            rsrcmgr = PDFResourceManager()
            # 创建一个PDF设备对象
            laparams = LAParams()
            device = PDFPageAggregator(rsrcmgr, laparams=laparams)
            # 创建一个PDF解释器对象
            interpreter = PDFPageInterpreter(rsrcmgr, device)
            # print(len(doc.get_pages()))
            for page in doc.get_pages():
                interpreter.process_page(page)
                # 接受该页面的LTPage对象
                layout = device.get_result()
                # 这里layout是一个LTPage对象 里面存放着 这个page解析出的各种对象 一般包括LTTextBox, LTFigure, LTImage, LTTextBoxHorizontal 等等 想要获取文本就获得对象的text属性，
                for x in layout:
                    if (isinstance(x, LTTextBoxHorizontal)):
                        results = x.get_text()
                        print(results)
                        # with open(r'../../data/pdf/1.txt', 'a') as f:
                        #     results = x.get_text()
                        #     print(results)

            # 检测文档是否提供txt转换，不提供就忽略
            # if not doc.is_extractable:
            #     print("aaaaaaaa")
            #     # raise PDFTextExtractionNotAllowed

            break




getFileNames("/Users/miaozw/Documents/TEMP/201705687108","")
