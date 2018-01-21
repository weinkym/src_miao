 function zw_setCount()
 {
    var jsnump = document.getElementById('jsnump');
var items=jsnump.getElementsByTagName("li");
var n = items.length;

new QWebChannel(qt.webChannelTransport, function(channel) {
        var js_object = channel.objects.ZWWebengineView;
        js_object.setCount(n);});
 }
 zw_setCount();

