
function readData()
{
	var obj_div_table = document.getElementsByClassName("uc-tables eshare-tables")[0];
	if(!isValid(obj_div_table))
	{
		onError("obj_div_table not isValid");
		return fasle;
	}

	var obj_table = obj_div_table.getElementsByTagName("table")[0];
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
	if(array_tr.length != 2)
	{
		onError("array_tr.length != 2");
		return fasle;
	}
	var obj_tr = array_tr[1];
	if(!isValid(obj_tr))
	{
		onError("obj_tr not isValid");
		return fasle;
	}
	var array_td = obj_tr.getElementsByTagName("td");
	if(!isValid(array_td))
	{
		onError("array_td not isValid");
		return fasle;
	}
	if(array_td.length != 6)
	{
		onError("array_tr.length != 6");
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

