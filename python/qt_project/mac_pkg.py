import sys
import os
# import subprocess
sys.path.append(os.getcwd())
from zwpy import zwlog
from qt_project import mac_project as XLive
from qt_project import mac_utils as OTL
from zwpy import zwutil


def createPkg(app_dp, project_dp):
    version_fp = XLive.getProjectVersionFP(project_dp)
    version_info = XLive.getVersionInfo(version_fp)
    pkg_script_dp = OTL.joinPathWithRaise(project_dp, 'install/mac/pkg')
    pkg_script_fp = OTL.joinPathWithRaise(pkg_script_dp, 'xuanlive.pkgproj')
    packagesbuild_fp = OTL.getCmdFilePath("packagesbuild", True)
    zwlog.info('packagesbuild_fp={}'.format(packagesbuild_fp))

    app_name_eg = OTL.getDirname(app_dp)
    app_name_cn = "绚星直播.app"
    pkg_name = "XuanLive.pkg"
    pkg_name_out = "XuanLive-V{}.pkg".format(version_info.fullVersion())
    app_dp_to = OTL.joinPath(pkg_script_dp, app_name_cn)
    pkg_dp = OTL.joinPath(pkg_script_dp, pkg_name)
    pkg_name_out_fp = OTL.joinPath(pkg_script_dp, pkg_name_out)
    # OTL.deleteDP(app_dp_to)
    # OTL.deleteDP(pkg_dp)
    # OTL.deleteDP(pkg_name_out_fp)
    # OTL.copyD2D(app_dp, pkg_script_dp)
    # OTL.rename(pkg_script_dp, app_name_eg, app_name_cn)
    # cmd = "cd \"{}\";spctl -a -vvvv \"{}\"".format(pkg_script_dp, app_name_cn)
    # zwutil.run_cmd(cmd)
    # cmd = "packagesbuild --verbose --identity \"{}\" \"{}\"".format(
    #     OTL.G_DeveloperIDInstaller, pkg_script_fp)
    # zwutil.run_cmd(cmd)
    # OTL.checkPathWithRaise(pkg_dp)
    # OTL.rename(pkg_script_dp, pkg_name, pkg_name_out)
    # cmd = "spctl -a -v --type install \'{}\'".format(pkg_name_out_fp)
    # #TODO获取不到命令的结果
    # res = zwutil.run_cmd(cmd)
    # zwlog.info("res={}".format(res))

    # cmd = "xcrun altool --notarize-app --primary-bundle-id=\"cn.yunxuetang.ljlivepc\" -u \"yunxuetang@yunxuetang.cn\" -p \"{}\" --asc-provider 6F8YPTA92C -t osx --file \'{}\' ".format(
    #     OTL.G_SIGN_PW, pkg_name_out_fp)
    # res = zwutil.run_cmd(cmd)
    # zwlog.info("res={}".format(res))
    # print(version_info)
    zwutil.run_cmd('open \"{}\"'.format(pkg_script_dp))


if __name__ == '__main__':
    app_dp = "/Users/avc/Documents/release/XuanLive 2020-09-23 16-19-56/XuanLive.app"
    project_dp = "/Users/avc/work/ljlive222"
    createPkg(app_dp, project_dp)
    # res = zwutil.run_cmd("ls -l")
