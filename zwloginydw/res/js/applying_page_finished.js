

function zwfun()
{
	g_return_data_obj.apply_count=0;

	var obj_table = document.getElementsByClassName("tab-3")[0];
	if(!isValid(obj_table))
	{
		onError("obj_table not isValid");
		return false;
	}
	var obj_tbody = obj_table.getElementsByTagName("tbody")[0];
	if(!isValid(obj_tbody))
	{
		onError("obj_tbody not isValid");
		return false;
	}
	var array_tr = obj_tbody.getElementsByTagName("tr");
	if(!isValid(array_tr))
	{
		onError("array_tr not isValid");
		return false;
	}
	g_return_data_obj.apply_count = array_tr.length - 1;
	
	onJSResultCallabk();
	return true;
}

onDebug("applying_page_finished1");
zwfun();
onDebug("applying_page_finished2");



