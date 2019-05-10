import scrapy
from qtcn_spider.items import QtcnSpiderItem

# class DmozSplider(scrapy.Spider):
#     name="dmoz"
#     allowed_domains = ["yxt.com"]
#     start_urls = [
#         "http://www.yxt.com/"
#         ]

#     def parse(self,response):
#         items = []
#         for sel in response.xpath('/html/body/div[7]/div[2]/ul/li'):
#             item = Test001Item()
#             item['title'] = sel.xpath('a/div/text()').extract()
#             item['link'] = sel.xpath('div/a/img/@src').extract()
#             # item['desc'] = sel.xpath('div[2]/div/div/div[1]/text()').extract()
#             item['desc'] = sel.xpath('div[@class="fl in-event-info"]/div/div/div[1]/text()').extract()
#             print('TTTTTTTTTTTTTTTTTTTTT')
#             print(sel.xpath('div/a/img/@src').extract())
#             items.append(item)
#         return items
    
class QtcnSplider(scrapy.Spider):
    name="qtcn"
    allowed_domains = ["qtcn.org"]
    start_urls = [
        "http://www.qtcn.org/bbs/i.php/"
        ]

    def parse(self,response):
        items = []
        for sel in response.xpath('//div/ul/li/div'):
            # autor = sel.xpath('a[@class="cls_f"]/text()').extract()
            autor = sel.xpath('a[1]/text()').extract()
            title = sel.xpath('a[2]/text()').extract()
            # item['link'] = sel.xpath('@href').extract()
            # item['desc'] = sel.xpath('div[2]/div/div/div[1]/text()').extract()
            # item['desc'] = sel.xpath('div[@class="downbody"]/div[3]/a/@href').extract()
            print(autor)
            print('TTTTTTTTTTTTTTTTTTTTT')
            print(len(autor))
            
            if (len(autor) and len(title)):
                print('QQQQQQQQQQQQQQ')
                item = QtcnSpiderItem()
                item['autor']=autor
                item['title']=title
                items.append(item) 
            
        return items