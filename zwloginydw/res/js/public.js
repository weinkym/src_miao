

function isValid(obj)
{
	return !(typeof(obj) == "undefined" || (!obj && obj != 0))
}

function onDebug(value)
{
	js_callback_object.onDebug(value,g_type);
}

function onWarning(value)
{
	js_callback_object.onWarning(value,g_type);
}

function onError(value)
{
    g_result_obj.error = 1;
    g_result_obj.errorKey = value;

    onJSResultCallabk();
    //js_callback_object.onError(value,g_type,g_needReload);
}

function compareDate(date1,date2)
{
    js_callback_object.onDebug("run compareDate",g_type);
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

function onJSResultCallabk()
{
    g_result_obj.needReload = g_needReload;
    g_result_obj.data = g_return_data_obj;
    js_callback_object.onJSResultCallabk(JSON.stringify(g_result_obj),g_type);
}

function onNeedReload()
{
	g_needReload = 1;
}
