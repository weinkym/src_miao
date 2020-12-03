#!/usr/local/bin/python3
import sys
import os
g_fp_path = os.path.abspath(os.path.dirname(__file__))
sys.path.append(g_fp_path)
sys.path.append(os.path.abspath(os.path.dirname(g_fp_path)))
sys.path.append(os.getcwd())
from zwpy import zwutil
from qt_project import mac_utils as OTL

G_IDENT_APP = 'Developer ID Application: Jiangsu Yunxuetang Network Technology  Co.,Ltd (6F8YPTA92C)'
# G_IDENT_INSTALL = ''


def codesignFile(fp, entitlements_fp, need_runtime=False):
    cmd = 'codesign -f -s \"{}\" \"{}\"'.format(G_IDENT_APP, fp)
    name = OTL.getDirname(fp)
    if (fp.endswith('.app') or need_runtime
            or ('.' not in name and ('obs-plugins' in fp or 'MacOS' in fp))):
        cmd = 'codesign -f -o runtime --entitlements \"{}\" --timestamp --deep -s \"{}\" -i "cn.yunxuetang.ljlivepc" \"{}\"'.format(
            entitlements_fp, G_IDENT_APP, fp)
    print('cmd={}'.format(cmd))
    res = zwutil.run_cmd(cmd)
    print('res={}'.format(res))


def run_lj_obs_archive_xcode2(project_dp, app_dp):
    print('log line={}'.format(sys._getframe().f_lineno))
    # macdeployqt_fp = OTL.getCmdFilePath('macdeployqt')
    macdeployqt_fp = '/Users/avc/Qt5.7.1/5.7/clang_64/bin/macdeployqt'
    print('log line={}'.format(sys._getframe().f_lineno))
    OTL.checkPathWithRaise(macdeployqt_fp)
    print('log line={}'.format(sys._getframe().f_lineno))
    contens_dp = OTL.joinPathWithRaise(app_dp, 'Contents')
    macos_dp = OTL.joinPathWithRaise(contens_dp, 'MacOS')

    print('log line={}'.format(sys._getframe().f_lineno))

    agora_avatar_push_from_fp = OTL.joinPathWithRaise(
        project_dp,
        'bin/mac/release/agora_avatar_push.app/Contents/MacOS/agora_avatar_push'
    )
    print('log line={}'.format(sys._getframe().f_lineno))
    OTL.copyF2D(agora_avatar_push_from_fp, macos_dp)
    agora_avatar_push_to_fp = OTL.joinPathWithRaise(
        macos_dp, OTL.getDirname(agora_avatar_push_from_fp))
    os.system('{} \"{}\"'.format(macdeployqt_fp, agora_avatar_push_to_fp))

    os.system('{} \"{}\"'.format(macdeployqt_fp, app_dp))
    print('log line={}'.format(sys._getframe().f_lineno))
    frameworks_dp = OTL.joinPathWithRaise(contens_dp, 'Frameworks')

    resources_dp = OTL.joinPathWithRaise(contens_dp, 'Resources')
    entitlements_fp = OTL.joinPathWithRaise(project_dp,
                                            'ljobs/xuanlive.entitlements')
    icns_fp = OTL.joinPathWithRaise(project_dp, 'ljobs/XuanLive.icns')
    print('log line={}'.format(sys._getframe().f_lineno))
    OTL.copyF2D(icns_fp, resources_dp)

    vendor_dp = OTL.joinPathWithRaise(project_dp, 'vendor')
    plugins_dp = OTL.joinPathWithRaise(vendor_dp, 'obs/mac/obs-plugins')
    print('log line={}'.format(sys._getframe().f_lineno))
    OTL.copyD2D(plugins_dp, contens_dp)
    print('log line={}'.format(sys._getframe().f_lineno))

    data_dp_list = []
    data_dp_list.append(OTL.joinPathWithRaise(vendor_dp, 'obs/mac/ljdata'))
    data_dp_list.append(OTL.joinPathWithRaise(vendor_dp, 'obs/mac/data'))
    print('log line={}'.format(sys._getframe().f_lineno))
    for dp in data_dp_list:
        OTL.copyD2D(dp, contens_dp)

    third_lib_list = OTL.getFiles(vendor_dp, ['.so', '.dylib'],
                                  ['/obs-plugins/', 'debug'])
    exists_name_list = []
    for fp, name in third_lib_list:
        if name in exists_name_list:
            print('name is repeat name={},fp={},fp={}'.format(
                name, fp, exists_name_list[name][1]))
        else:
            exists_name_list.append((name, fp))
    for fp, name in third_lib_list:
        OTL.copyF2D(fp, frameworks_dp)

    cef_dp = OTL.joinPathWithRaise(
        vendor_dp,
        'additional_install_files/cef/mac/Release/Chromium Embedded Framework.framework'
    )
    OTL.copyD2D(cef_dp, frameworks_dp)

    # #声网动态库
    # agora_dp = OTL.joinPathWithRaise(vendor_dp,
    #                                  'agora/libs/mac/AgoraRtcKit.framework')
    # OTL.copyD2D(agora_dp, frameworks_dp)

    name_list = OTL.adjuestLibs(frameworks_dp)
    new_plugins_dp = OTL.joinPathWithRaise(contens_dp,
                                           OTL.getDirname(plugins_dp))
    obj_list = OTL.getFiles(new_plugins_dp, ['.so', 'obs-browser-page'], [])
    for fp, name in obj_list:
        OTL.adjuestLibToRpath(fp, name_list)
        rp_list = ['@executable_path/../Frameworks']
        if 'obs-browser' in name:
            rp_list.append(
                '@executable_path/../Frameworks/Chromium Embedded Framework.framework'
            )
        OTL.adjuestRapth(fp, rp_list)

    app_name = OTL.getDirname(app_dp)
    app_process_name = app_name.replace('.app', '')
    app_fp = OTL.joinPathWithRaise(macos_dp, app_process_name)
    print("app_name={}".format(app_name))
    print("app_process_name={}".format(app_process_name))
    print("app_fp={}".format(app_fp))
    OTL.adjuestRapth(app_fp, ['@executable_path/../Frameworks'])
    OTL.adjuestLibToRpath(app_fp, name_list)

    OTL.adjuestRapth(agora_avatar_push_to_fp,
                     ['@executable_path/../Frameworks'])
    OTL.adjuestLibToRpath(agora_avatar_push_to_fp, name_list)

    runtime_list = []
    runtime_list.append(
        OTL.joinPathWithRaise(frameworks_dp, 'QtWebEngineCore.framework'))

    runtime_list.append(
        OTL.joinPathWithRaise(contens_dp, 'obs-plugins/obs-browser-page'))
    runtime_list.append(
        OTL.joinPathWithRaise(frameworks_dp, 'QtWebEngineWidgets.framework'))
    runtime_list.append(
        OTL.joinPathWithRaise(contens_dp,
                              'data/obs-plugins/obs-ffmpeg/ffmpeg-mux'))
    runtime_list.append(agora_avatar_push_to_fp)
    for fp in runtime_list:
        codesignFile(fp, entitlements_fp, True)


def doStart():
    macdeployqt_fp = OTL.getCmdFilePath('macdeployqt')
    print("macdeployqt_fp======={}".format(macdeployqt_fp))
    ls_fp = OTL.getCmdFilePath('ls')
    print("ls_fp======={}".format(ls_fp))
    if len(sys.argv) < 3:
        print(sys.argv)
        print("=======param is invalid")
        return
    src_dp = sys.argv[1]

    project_dp = os.path.abspath(os.path.dirname(src_dp))
    app_dp = sys.argv[2]
    print("src_dp={}".format(src_dp))
    print("project_dp={}".format(project_dp))
    print("app_dp={}".format(app_dp))
    os.system('open \"{}\"'.format(project_dp))
    run_lj_obs_archive_xcode2(project_dp, app_dp)
    #  '/Users/avc/Documents/TEMP/ljlive.app')
    # os.system('open \"{}\"'.format(app_dp))


def test():
    for obj in sys.argv:
        print("argv======={}".format(obj))
    macdeployqt_fp = OTL.getCmdFilePath('macdeployqt')
    print("macdeployqt_fp======={}".format(macdeployqt_fp))
    res = zwutil.run_cmd('echo ${PATH}')
    print("res======={}".format(res))

    project_dp = '/Users/avc/work/ljlive222'
    app_dp = '/Users/avc/Documents/TEMP/TEMP/XuanLive.app'
    os.system('rm -rf {}'.format(app_dp))
    os.system(
        'cp -aR \"/Users/avc/work/ljlive222/bin/mac/release/XuanLive.app\" /Users/avc/Documents/TEMP/TEMP'
    )
    run_lj_obs_archive_xcode2(project_dp, app_dp)
    os.system('open {}'.format(app_dp))


doStart()
