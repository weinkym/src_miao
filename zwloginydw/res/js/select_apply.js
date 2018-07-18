
function zwfun()
{
	var zw_param_date_str = "%1";
	
	g_return_data_obj.select_finished = false;

	var obj_eshare_btn = document.getElementById('eshare_btn');

	if(!isValid(obj_eshare_btn))
	{
		onError("obj_tobj_eshare_btnbody not isValid");
		return false;
	}

	var obj_eshare_selection_box = document.getElementsByClassName("eshare-selection-box")[0];
	if(!isValid(obj_eshare_selection_box))
	{
		onError("obj_eshare_selection_box not isValid");
		return false;
	}
	var obj_uc_tables = obj_eshare_selection_box.getElementsByClassName("uc-tables")[0];
	if(!isValid(obj_uc_tables))
	{
		onError("obj_uc_tables not isValid");
		return false;
	}

	var obj_table = obj_uc_tables.getElementsByTagName("table")[0];
	if(!isValid(obj_table))
	{
		onError("obj_table not isValid");
		return false;
	}
	var array_tbody = obj_table.getElementsByTagName("tbody");
	if(array_tbody.length <= 0)
	{
		onError("tbody not found");
		return false;
	}

	var obj_tbody = array_tbody[0];
	var array_tr = obj_tbody.getElementsByTagName("tr");
	if(array_tr.length < 2)
	{
		onError("tr length "+array_tr.length+"< 2");
		return;
	}
	var found = false;
	var date;
	var money;
	for(var i = 1; i < array_tr.length;++i)
	{
		var array_td = array_tr[i].getElementsByTagName("td");
		//alert(array_td.length);
		if(array_td.length !=7)
		{
			onWarning("array_td.length !=7  (" + array_td.length+")");
			continue;
		}
		date = array_td[6].innerHTML;
		money= array_td[3].innerHTML;
		onDebug("date=" + date +"  zw_param_date_str= "+zw_param_date_str);
		if(date === zw_param_date_str)
		{
			var array_input = array_td[0].getElementsByTagName("input");
			if(array_input.length != 1)
			{
				onError("array_input length"+array_tr.length+"!= 1");
				return false;
			}
			var obj_input = array_input[0];
			onDebug("obj_input.name=" + obj_input.name);
			obj_input.checked = true;
			found = true;
			break;
		}
	}
	if(!found)
	{
		onError("date not found");
		return false;
	}
	g_return_data_obj.select_finished = true;
	obj_eshare_btn.click();
	onJSResultCallabk();
	return true;
}

onDebug("apply_page_finished1");
zwfun();
onDebug("apply_page_finished2");
