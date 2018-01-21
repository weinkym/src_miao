function zw_read_page(count)
{
var uc_tables = document.getElementsByClassName("uc-tables");
var uc_one_table = uc_tables[0];
// alert(typeof(uc_table));
// alert(uc_table.class)
var tables = uc_one_table.getElementsByTagName("table");
var table = tables[0];
var tbodys = table.getElementsByTagName("tbody");
var tbody = tbodys[0];
var items = tbody.getElementsByTagName("tr");
var n = items.length; 
var test = "";
for(var i = 1; i < items.length; i++)
{
    var tds = items[i].getElementsByTagName("td");
    var item0 = tds[0];
    var href = item0.getElementsByTagName("a")[0].getAttribute("href");
    var title = item0.getElementsByTagName("a")[0].getAttribute("title");
    var code = item0.getElementsByTagName("span")[0].innerHTML;
    test = test+title+"@";
    test = test+code+"@";
    // for(var j = 1;j <tds.length;j++)
    for(var j = 1;j <7;j++)
    {
        var td = tds[j];
        test = test+td.innerHTML+"@";
    }
    test = test+href+"@";
    test = test+"\n";
}

new QWebChannel(qt.webChannelTransport, function(channel) {
        var js_object = channel.objects.ZWWebengineView;
        js_object.appendText(test,count);});
}





