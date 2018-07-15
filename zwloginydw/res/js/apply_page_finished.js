
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


function readData()
{
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
	if(array_td.length != 6)
	{
		onError("array_tr.length != 6");
		return false;
	}
	var mount = array_td[2].innerHTML;
	mount = mount.replace(/,/, "")
	mount = parseInt(parseFloat(mount) / 100) * 100;
	if(mount > 10000)
	{
		mount = 10000;
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
    obj_borrow_apr.value = 15;
	onDebug("set obj_borrow_apr.value="+obj_borrow_apr.value);

	//开始申请
	borroweshareapply();
	onJSResultCallabk();
	return true;
}

function run()
{
	onDebug("apply_page_finished1");

	var res = readData();
	onDebug("apply_page_finished2");
	if(res != true)
	{
    	onError("doFun");
    	return;
	}
}
run();
