

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
