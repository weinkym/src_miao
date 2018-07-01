function isValid(obj)
{
	return !(typeof(obj) == "undefined" || (!obj && obj != 0))
}

function doError(value)
{
	alert(value);
}


function getAmount()
{
	var obj_optimize = document.getElementsByClassName("item item-4 optimize-item-4")[0];
	if(!isValid(obj_optimize))
	{
		doError("obj_optimize not isValid");
		return;
	}
	var obj_div_amount = obj_optimize.getElementsByClassName("amount")[0];
	if(!isValid(obj_div_amount))
	{
		doError("obj_div_amount not isValid");
		return;
	}
	var obj_p_amount = obj_div_amount.getElementsByTagName("p")[0];
	if(!isValid(obj_p_amount))
	{
		doError("obj_p_amount not isValid");
		return;
	}
	var obj_span_amount = obj_p_amount.getElementsByTagName("span")[0];
	if(!isValid(obj_span_amount))
	{
		doError("obj_span_amount not isValid");
		return;
	}
	var amount = obj_span_amount.innerHTML;
	alert("amount="+amount);
	return amount;
	// AccountBalance

}

function getAccountBalance()
{
	var obj_optimize = document.getElementsByClassName("uc-mod uc-mod-asset-profile clearfix mrt20")[0];
	if(!isValid(obj_optimize))
	{
		doError("obj_optimize not isValid");
		return;
	}
	var obj_div_bd = obj_optimize.getElementsByClassName("bd")[0];
	if(!isValid(obj_div_bd))
	{
		doError("obj_div_bd not isValid");
		return;
	}
	var obj_div_item = obj_div_bd.getElementsByClassName("item item-3")[0];
	if(!isValid(obj_div_item))
	{
		doError("obj_div_item not isValid");
		return;
	}
	
	var obj_div_p_amount = obj_div_item.getElementsByClassName("amount")[0];
	if(!isValid(obj_div_p_amount))
	{
		doError("obj_div_p_amount not isValid");
		return;
	}
	var obj_div_span = obj_div_p_amount.getElementsByTagName("span")[0];
	if(!isValid(obj_div_span))
	{
		doError("obj_div_span not isValid");
		return;
	}
	var obj_div_span_a = obj_div_span.getElementsByTagName("a")[0];
	if(!isValid(obj_div_span_a))
	{
		doError("obj_div_span_a not isValid");
		return;
	}
	var amount = obj_div_span_a.innerHTML;
	alert("obj_div_span_a="+amount);
	return amount;

}
function getRecentAmountObject()
{
	var obj_optimize = document.getElementsByClassName("uc-mod uc-mod-calendar-result fr")[0];
	if(!isValid(obj_optimize))
	{
		doError("obj_optimize not isValid");
		return;
	}
	var obj_div_bd = obj_optimize.getElementsByClassName("bd")[0];
	if(!isValid(obj_div_bd))
	{
		doError("obj_div_bd not isValid");
		return;
	}
	var obj_div_items = obj_div_bd.getElementsByClassName("items")[0];
	if(!isValid(obj_div_items))
	{
		doError("obj_div_items not isValid");
		return;
	}
	var obj_div_ul = obj_div_items.getElementsByTagName("ul")[0];
	if(!isValid(obj_div_ul))
	{
		doError("obj_div_ul not isValid");
		return;
	}
	var array_div_li = obj_div_ul.getElementsByTagName("li");
	if(!isValid(array_div_li))
	{
		doError("array_div_li not isValid");
		return;
	}
	return array_div_li;
}
function doFun()
{
	var amount = getAmount();
	if(!isValid(amount))
	{
		doError("getAmount not isValid");
		return;
	}
	var accountBalance = getAccountBalance();
	if(!isValid(accountBalance))
	{
		doError("getAccountBalance not isValid");
		return;
	}
	var array_div_li = getRecentAmountObject();
	if(!isValid(array_div_li))
	{
		doError("array_div_li not isValid");
		return;
	}
	//==================
	if(array_div_li.length != 5)
	{
		doError("array_div_li.length != 5 len="+array_div_li.length);
		return;
	}

	var obj_li_recent_receivable_amount = array_div_li[4];
	if(!isValid(obj_li_recent_receivable_amount))
	{
		doError("obj_li_recent_receivable_amount not isValid");
		return;
	}
	var recent_receivable_amount_date = obj_li_recent_receivable_amount.innerHTML;
	var obj_span_recent_receivable_amount = obj_li_recent_receivable_amount.getElementsByTagName("span")[0];
	if(!isValid(obj_span_recent_receivable_amount))
	{
		doError("obj_span_recent_receivable_amount not isValid");
		return;
	}
	var recent_receivable_amount = obj_span_recent_receivable_amount.innerHTML;
	//==================
	var obj_li_recent_refund_amount = array_div_li[3];
    if(!isValid(obj_li_recent_refund_amount))
    {
        doError("obj_li_recent_refund_amount not isValid");
        return;
    }
    var recent_refund_amount_date = obj_li_recent_refund_amount.innerHTML;
    var obj_span_recent_refund_amount = obj_li_recent_refund_amount.getElementsByTagName("span")[0];
    if(!isValid(obj_span_recent_refund_amount))
    {
        doError("obj_span_recent_refund_amount not isValid");
        return;
    }
    var recent_refund_amount = obj_span_recent_refund_amount.innerHTML;

    alert(recent_receivable_amount_date+recent_receivable_amount+recent_refund_amount_date+recent_refund_amount);


	//todo callback
}

doFun();