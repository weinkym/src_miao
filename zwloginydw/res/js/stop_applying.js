//=================取消宜享
function stopApplying()
{
	var zw_param_01 = "%1";

	var obj_opt = document.getElementsByClassName("opt")[0];
	if(!isValid(obj_opt))
	{
		onError("obj_opt not isValid");
		return false;
	}

	//可能是提前结标或者撤销
	var obj_a = obj_opt.getElementsByTagName("a")[0];
	if(!isValid(obj_a))
	{
		onError("obj_a not isValid");
		return false;
	}
	//弹窗撤销窗口
	obj_a.onclick();
	var obj_tradePassword = document.getElementById("tradePassword");
	if(!isValid(obj_tradePassword))
	{
		onError("obj_tradePassword not isValid");
		return false;
	}
	//支付密码
	obj_tradePassword.value=zw_param_01;

	var obj_cancelborrow = document.getElementById('cancelborrow');
	g_return_data_obj.has_money=0;
	if (!(obj_cancelborrow.value))
	{
		g_return_data_obj.has_money=1;
  		obj_cancelborrow = document.getElementById('finishborrow');
	}
	if(!isValid(obj_cancelborrow))
	{
		onError("obj_cancelborrow not isValid");
		return false;
	}
	//确定取消
	//alert(obj_cancelborrow.value);
	obj_cancelborrow.click();
	onJSResultCallabk();

	return true;
}

function run()
{
	onDebug("stopApplying");

	var res = stopApplying();
	onDebug("stopApplying2");
	if(res != true)
	{
    	onError("doFun");
    	return;
	}
}
run();
