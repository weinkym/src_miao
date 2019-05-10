import PyPDF2

fileName='/Users/miaozw/Documents/TEMP/201705687108/TEST.pdf'
pdfFile = open(fileName,'rb')

pdfReader = PyPDF2.PdfFileReader(pdfFile)
pdfReader.decrypt('15858239032')
print(pdfReader.numPages)
print(pdfReader.getDocumentInfo())

pdfFileMerger = PyPDF2.PdfFileMerger()


pdfFileWriter = PyPDF2.PdfFileWriter()
for i in range(0,pdfReader.numPages):
    if i % 2 == 0:
        pdfFileWriter.addPage(pdfReader.getPage(i))

with open('/Users/miaozw/Documents/TEMP/201705687108/MZW.pdf','wb') as f:
    pdfFileWriter.write(f)
# with open('/Users/miaozw/Documents/TEMP/201705687108/MZW.pdf','wb') as f:
#     pdfFileWriter.write(f)



