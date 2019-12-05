
from PIL import Image
from PIL import ImageColor
from PIL import ImageDraw
from PIL import ImageDraw2

img = Image.open("/Users/miaozw/Documents/test.jpg")
img = img.resize((136,80))
print(img.format)		 # 输出图片基本信息
print(img.mode)
print(img.size)
# img.setColor()
for i in range(img.size[0]):
    for j in range(img.size[1]):
        color=img.getpixel((i,j))
        # print(color)
        # img.putpixel((i,j),(0,color[1],0))
        # img.putpixel((i,j),(0,0,color[2]))
img.save("/Users/miaozw/Documents/rgb.PNG")

# wb.save(r'/Users/miaozw/Documents/TEMP/python/test.xlsx')
