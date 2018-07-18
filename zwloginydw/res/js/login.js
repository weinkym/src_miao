var zw_param_01 = "%1";
var zw_param_02 = "%2";
var obj_btn_login = document.getElementById('login_button');
if(!isValid(obj_btn_login))
{
    onError("obj_btn_login is not valid");
    return;
}

document.getElementById('keywords').value=zw_param_01;
document.getElementById('password').value=zw_param_02;
// document.getElementById('keywords').value=13616511205;
// document.getElementById('password').value="miao1280";
obj_btn_login.click();
