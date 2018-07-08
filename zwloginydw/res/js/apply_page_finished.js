
function readData()
{
	g_return_data_obj.apply_count=0;

	var obj_table = document.getElementsByClassName("tab-3")[0];
	if(!isValid(obj_table))
	{
		onError("obj_table not isValid");
		return fasle;
	}
	var obj_tbody = obj_table.getElementsByTagName("tobody")[0];
	if(!isValid(obj_tbody))
	{
		onError("obj_tbody not isValid");
		return fasle;
	}
	var array_tr = obj_tbody.getElementsByTagName("tr");
	if(!isValid(array_tr))
	{
		onError("array_tr not isValid");
		return fasle;
	}
	g_return_data_obj.apply_count = array_tr.length - 1;
	
	onJSResultCallabk();
	return true;
}

function run()
{
	var res = readData();
	if(res != true)
	{
    	onError("doFun");
    	return;
	}
}
run();

