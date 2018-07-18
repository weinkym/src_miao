
function setMonthParam(obj_timeLimit,month)
{
	var option ='';
	var dayOrMonth="月";
	for(var i = 1 ;i<= 3;i++)
	{
		option+='<option value='+i+'>'+ i+ dayOrMonth +'</option>';
	}
	$("#timeLimit").html(option);

	UC.select();
	if(month <= 0 || month > 3)
	{
		month = 1;
	}
	//月 0 1 2 分别表示1 2 3月
	obj_timeLimit.getElementsByTagName("option")[month - 1].selected=true;
}


function zwfun()
{
	var zw_param_date_str = "%1";
	var zw_param_borrow_apr = "%2";

	g_return_data_obj.need_select = false;

	var obj_div_table = document.getElementsByClassName("uc-tables eshare-tables")[0];
	if(!isValid(obj_div_table))
	{
		onError("obj_div_table not isValid");
		return false;
	}

	var obj_table = obj_div_table.getElementsByTagName("table")[0];
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
	if(array_tr.length != 2)
	{
		onError("array_tr.length != 2");
		return false;
	}
	var obj_tr = array_tr[1];
	if(!isValid(obj_tr))
	{
		onError("obj_tr not isValid");
		return false;
	}
	var array_td = obj_tr.getElementsByTagName("td");
	if(!isValid(array_td))
	{
		onError("array_td not isValid");
		return false;
	}
	var adIndex = 0;
	if(array_td.length == 7)
	{
		adIndex = 1;
	}
	else if(array_td.length != 6)
	{
		onError("array_tr.length != 6  " + array_td.length);
		return false;
	}

	var currentDateString = array_td[5+adIndex].innerHTML;
	onDebug("currentDateString=" + currentDateString +"  zw_param_date_str= "+zw_param_date_str);
	if(!(zw_param_date_str === currentDateString))
	{
		var message = ("currentDateString != currentDateString ("+zw_param_date_str+"  "+currentDateString+")");
		onDebug(message);

		var cn_reSelection_btn = document.getElementsByClassName("reSelection_btn")[0];
		if(!isValid(cn_reSelection_btn))
		{
			onError("cn_reSelection_btn not isValid");
			return false;
		}
		//重新选择弹窗
		cn_reSelection_btn.click();

		g_return_data_obj.need_select = true;
		onJSResultCallabk();
		return true;
	}

	var mount = array_td[2+adIndex].innerHTML;
	mount = mount.replace(/,/, "")
	mount = parseInt(parseFloat(mount) / 100) * 100;
	var maxMount=30000;
	if(mount > maxMount)
	{
		mount = maxMount;
	}
	onDebug("mount="+mount)


	//=================执行申请
	var obj_timeLimitType = document.getElementById('timeLimitType');
	if(!isValid(obj_timeLimitType))
	{
		onError("obj_timeLimitType not isValid");
		return false;
	}

	var obj_timeLimit = document.getElementById('timeLimit');
	if(!isValid(obj_timeLimit))
	{
		onError("obj_timeLimit not isValid");
		return false;
	}
	//选择按月，期限为3个月
	var isMonth = true;
	if(isMonth)
	{
		//类型：0是天 1是月
		obj_timeLimitType.getElementsByTagName("option")[1].selected=true;
		//设置月数
		setMonthParam(obj_timeLimit,3);
	}
	else
	{
		//月 0 1 2 分别表示1 2 3月 天：0-28 表示1-29天
		obj_timeLimit.getElementsByTagName("option")[2].selected=true;
	}

	//alert(obj_timeLimitType.value);
	var obj_account = document.getElementById('account');
	if(!isValid(obj_account))
	{
		onError("obj_account not isValid");
		return false;
	}
	obj_account.value = mount;

	onDebug("set account="+obj_account.value);
	var obj_borrow_apr = document.getElementById('borrow_apr');
	if(!isValid(obj_account))
	{
		onError("obj_borrow_apr not isValid");
		return false;
	}
    obj_borrow_apr.value = zw_param_borrow_apr;
	onDebug("set obj_borrow_apr.value="+obj_borrow_apr.value);

	//开始申请
	borroweshareapply();
	onJSResultCallabk();
	return true;
}

onDebug("apply_page_finished1");
zwfun();
onDebug("apply_page_finished2");
