

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
	js_callback_object.onError(value,g_type);
}

