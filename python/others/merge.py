import PyPDF2
import os

def getFilePaths(path,suffix):
    fileList=[]
    for parent,dirnames,filenames in os.walk(path,  topdown=False):
        for filename in filenames:
            if filename.endswith(suffix):
                file_path=os.path.join(parent,filename)
                fileList.append(file_path)
                # print(file_path)
    return fileList


def mergePDF(path,outpath):
    fileList=getFilePaths(path,'.pdf')
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





path="/Users/miaozw/Documents/TTTTTTTTTT"
# path="/Users/miaozw/work/mzwdoc/对外经/19春/计算机网络"

mergePDF(path,'/Users/miaozw/Documents/TTTTTTTTTT/管理信息系统_all.pdf')
