var demo = document.getElementById('login_button');
document.getElementById('keywords').value=13616511205;
document.getElementById('password').value="miao1280";
demo.click();

//========//
window.location.href="https://www.yidai.com/borrowesharemy/index/"; 

//========//
borrowesharemy.searchborrowesharemy(2,-1,0);

//========//
page_jump("this","8");

//========//
var jsnump = document.getElementById('jsnump'); 
var items=jsnump.getElementsByTagName("li");
var n = items.length; 
alert("test");
//========//
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
	var tds = items[i].getElementsByTagName("td")
	var item0 = tds[0]
	var title = item0.getElementsByTagName("a")[0]innerHTML;
	var code = item0.getElementsByTagName("span")[0]innerHTML;
	test = test+title+"@";
	test = test+code+"@";
	// for(var j = 1;j <tds.length;j++)
	for(var j = 1;j <7;j++)
	{
		var td = tds[j];
		test = test+td.innerHTML+"@";
	}
	test = test+"\n";
	// break;
}
// alert("testasdfasdf"+n+"TTTTTV\n"+test);
//========//

