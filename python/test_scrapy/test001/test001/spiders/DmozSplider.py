import scrapy
from test001.items import Test001Item

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
    
class DmozSplider(scrapy.Spider):
    name="dmoz"
    allowed_domains = ["sc.chinaz.com"]
    start_urls = [
        "http://sc.chinaz.com/tubiao/171018336930.htm"
        ]

    def parse(self,response):
        items = []
        for sel in response.xpath('//div/div[@class="downcon"]/div[@class="downbody"]/div[position()>2]/a'):
            item = Test001Item()
            item['title'] = sel.xpath('text()').extract()
            item['link'] = sel.xpath('@href').extract()
            # item['desc'] = sel.xpath('div[2]/div/div/div[1]/text()').extract()
            # item['desc'] = sel.xpath('div[@class="downbody"]/div[3]/a/@href').extract()
            print('TTTTTTTTTTTTTTTTTTTTT')
            # print(sel.xpath('div/a/img/@src').extract())
            items.append(item)
        return items