function compareDate(date1,date2)
{
	date1.setHours(0);
	date1.setMinutes(0);
	date1.setSeconds(0);

	date2.setHours(0);
	date2.setMinutes(0);
	date2.setSeconds(0);
	if(date1 > date2)
	{
		return 1;
	}
	else if(date1 < date2)
	{
		return -1;
	}
	else 
	{
		return 0;
	}

}
function isToday(date)
{
	var today = new Date();
	var res = compareDate(date,today);
	// alert(res);

	return res == 0;
}

function toDateWidthMonthDay(separator,str)
{
	var array_date_str = str.split(separator);
	if(array_date_str.length != 2)
	{
		return;
	}
	//月份从0开始到11
	var month = parseInt(array_date_str[0]) - 1;
	var day = parseInt(array_date_str[1]);
	if(month < 0 || month > 11)
	{
		return;
	}
	if(day > 31 || day < 1)
	{
		return;
	}
	var date = new Date();
	date.setMonth(month);
	date.setDate(day);
	return date;
}

function isValid(obj)
{
	return !(typeof(obj) == "undefined" || (!obj && obj != 0))
}

function test()
{
	var date = toDateWidthMonthDay("-","07-08");

	if(!isValid(date))
	{
		alert("date not isValid");
		return;
	}

// var today = new Date();
var res = isToday(date);
alert(res);
}
test();

