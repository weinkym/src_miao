// function isValid(obj)
// {
// 	return !(typeof(obj) == "undefined" || (!obj && obj != 0))
// }

// function onWarning(value)
// {
// 	alert(value);
// }

// function onError(value)
// {
// 	alert(value);
// }


function getAmount()
{
	var obj_optimize = document.getElementsByClassName("item item-4 optimize-item-4")[0];
	if(!isValid(obj_optimize))
	{
		onWarning("obj_optimize not isValid");
		return;
	}
	var obj_div_amount = obj_optimize.getElementsByClassName("amount")[0];
	if(!isValid(obj_div_amount))
	{
		onWarning("obj_div_amount not isValid");
		return;
	}
	var obj_p_amount = obj_div_amount.getElementsByTagName("p")[0];
	if(!isValid(obj_p_amount))
	{
		onWarning("obj_p_amount not isValid");
		return;
	}
	var obj_span_amount = obj_p_amount.getElementsByTagName("span")[0];
	if(!isValid(obj_span_amount))
	{
		onWarning("obj_span_amount not isValid");
		return;
	}
	var amount = obj_span_amount.innerHTML;
	// alert("amount="+amount);
	return amount;
	// AccountBalance

}

function getAccountBalance()
{
	var obj_optimize = document.getElementsByClassName("uc-mod uc-mod-asset-profile clearfix mrt20")[0];
	if(!isValid(obj_optimize))
	{
		onWarning("obj_optimize not isValid");
		return;
	}
	var obj_div_bd = obj_optimize.getElementsByClassName("bd")[0];
	if(!isValid(obj_div_bd))
	{
		onWarning("obj_div_bd not isValid");
		return;
	}
	var obj_div_item = obj_div_bd.getElementsByClassName("item item-3")[0];
	if(!isValid(obj_div_item))
	{
		onWarning("obj_div_item not isValid");
		return;
	}
	
	var obj_div_p_amount = obj_div_item.getElementsByClassName("amount")[0];
	if(!isValid(obj_div_p_amount))
	{
		onWarning("obj_div_p_amount not isValid");
		return;
	}
	var obj_div_span = obj_div_p_amount.getElementsByTagName("span")[0];
	if(!isValid(obj_div_span))
	{
		onWarning("obj_div_span not isValid");
		return;
	}
	var obj_div_span_a = obj_div_span.getElementsByTagName("a")[0];
	if(!isValid(obj_div_span_a))
	{
		onWarning("obj_div_span_a not isValid");
		return;
	}
	var amount = obj_div_span_a.innerHTML;
	// alert("obj_div_span_a="+amount);
	return amount;

}
function getRecentAmountObject()
{
	var obj_optimize = document.getElementsByClassName("uc-mod uc-mod-calendar-result fr")[0];
	if(!isValid(obj_optimize))
	{
		onWarning("obj_optimize not isValid");
		return;
	}
	var obj_div_bd = obj_optimize.getElementsByClassName("bd")[0];
	if(!isValid(obj_div_bd))
	{
		onWarning("obj_div_bd not isValid");
		return;
	}
	var obj_div_items = obj_div_bd.getElementsByClassName("items")[0];
	if(!isValid(obj_div_items))
	{
		onWarning("obj_div_items not isValid");
		return;
	}
	var obj_div_ul = obj_div_items.getElementsByTagName("ul")[0];
	if(!isValid(obj_div_ul))
	{
		onWarning("obj_div_ul not isValid");
		return;
	}
	var array_div_li = obj_div_ul.getElementsByTagName("li");
	if(!isValid(array_div_li))
	{
		onWarning("array_div_li not isValid");
		return;
	}
	return array_div_li;
}
function readData()
{

	var amount = getAmount();
	if(!isValid(amount))
	{
		onWarning("getAmount not isValid");
		return false;
	}
	var accountBalance = getAccountBalance();
	if(!isValid(accountBalance))
	{
		onWarning("getAccountBalance not isValid");
		return false;
	}
	var array_div_li = getRecentAmountObject();
	if(!isValid(array_div_li))
	{
		onWarning("array_div_li not isValid");
		return false;
	}
	//==================
	if(array_div_li.length != 5)
	{
		onWarning("array_div_li.length != 5 len="+array_div_li.length);
		onNeedReload();
		return false;
	}

	var obj_li_recent_receivable_amount = array_div_li[4];
	if(!isValid(obj_li_recent_receivable_amount))
	{
		onWarning("obj_li_recent_receivable_amount not isValid");
		return false;
	}
	var recent_receivable_amount_date = obj_li_recent_receivable_amount.innerHTML;
	var obj_span_recent_receivable_amount = obj_li_recent_receivable_amount.getElementsByTagName("span")[0];
	if(!isValid(obj_span_recent_receivable_amount))
	{
		onWarning("obj_span_recent_receivable_amount not isValid");
		return false;
	}
	var recent_receivable_amount = obj_span_recent_receivable_amount.innerHTML;
	//==================
	var obj_li_recent_refund_amount = array_div_li[3];
    if(!isValid(obj_li_recent_refund_amount))
    {
        onWarning("obj_li_recent_refund_amount not isValid");
		return false;
    }
    var recent_refund_amount_date = obj_li_recent_refund_amount.innerHTML;
    var obj_span_recent_refund_amount = obj_li_recent_refund_amount.getElementsByTagName("span")[0];
    if(!isValid(obj_span_recent_refund_amount))
    {
        onWarning("obj_span_recent_refund_amount not isValid");
		return false;
    }
    var recent_refund_amount = obj_span_recent_refund_amount.innerHTML;

    // alert(recent_receivable_amount_date+recent_receivable_amount+recent_refund_amount_date+recent_refund_amount);
    js_callback_object.onAmountCallback(recent_receivable_amount,recent_receivable_amount_date,recent_refund_amount,recent_refund_amount_date);

	return true;
	//todo callback
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

