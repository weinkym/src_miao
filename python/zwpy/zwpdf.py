import PyPDF2
import os
from . import zwutil as zw

def mergePDF(path,outpath):
    fileList=zw.getFilePaths(path,'.pdf')
    pdfFileWriter = PyPDF2.PdfFileWriter()
    for filePath in fileList:
        pdfReader = PyPDF2.PdfFileReader(filePath)
        num=pdfReader.numPages
        for i in range(0,num):
            pdfFileWriter.addPage(pdfReader.getPage(i))
        print("add {} pages {}".format(num,filePath))

    with open(outpath,'wb') as f:
        pdfFileWriter.write(f)
        print("save to {}".format(outpath))


if __name__ == '__main__':
    path="/Users/miaozw/Documents/TTTTTTTTTT"
    # path="/Users/miaozw/work/mzwdoc/对外经/19春/计算机网络"
    mergePDF(path,'/Users/miaozw/Documents/TTTTTTTTTT/管理信息系统_all.pdf')
