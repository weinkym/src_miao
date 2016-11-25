# CIN_WOOK_ROOT=/Users/bitbrothers/work/others/src/workroot/src_miao/cscreenshot
# CIN_BUNDLE_OUT_PATH=/Users/bitbrothers/work
# CIN_QT_BIN_PATH=/Users/bitbrothers/Qt5.6.0/5.6/clang_64/bin
CIN_WOOK_ROOT=$1
CIN_BUNDLE_OUT_PATH=$2
CIN_QT_BIN_PATH=$3


P_QMAKE_BIN_PATH=${CIN_QT_BIN_PATH}/qmake
P_MACDEPLOQT_PATH=${CIN_QT_BIN_PATH}/macdeployqt

P_LIB_PATH=${CIN_WOOK_ROOT}/lib/mac/release
P_LIBS_LIST+=(libcscreenshot.1.dylib)

P_BUNDLE_NAME=cscreenshotbundle

P_BUNDLE_SOURCE_PATH=${CIN_WOOK_ROOT}/mac/${P_BUNDLE_NAME}
P_BUNDLE_PROJECT_PATH=${P_BUNDLE_SOURCE_PATH}/${P_BUNDLE_NAME}.pro
P_BUNDLE_PATH=${P_BUNDLE_SOURCE_PATH}/release/${P_BUNDLE_NAME}.app

P_ECHO_PREFIX="====================="

echo "${P_ECHO_PREFIX}检测文件夹路径"
P_CHECT_PATH_LIST+=($CIN_WOOK_ROOT)
P_CHECT_PATH_LIST+=($CIN_BUNDLE_OUT_PATH)
P_CHECT_PATH_LIST+=($CIN_QT_BIN_PATH)
P_CHECT_PATH_LIST+=($P_LIB_PATH)
P_CHECT_PATH_LIST+=($P_BUNDLE_SOURCE_PATH)
P_CHECK_PARAM=-d
for P_CHECK_PATH in ${P_CHECT_PATH_LIST[@]}; do
	if [ ! $P_CHECK_PARAM "$P_CHECK_PATH" ]; then
		echo "$P_CHECK_PATH is not exist"
		exit
	fi
done

echo "${P_ECHO_PREFIX}检测文件"
P_CHECT_FILE_LIST+=($P_QMAKE_BIN_PATH)
P_CHECT_FILE_LIST+=($P_MACDEPLOQT_PATH)
P_CHECT_FILE_LIST+=($P_BUNDLE_PROJECT_PATH)
P_CHECT_FILE_LIST+=($P_MACDEPLOQT_PATH)
P_CHECK_PARAM=-f
for P_CHECK_PATH in ${P_CHECT_FILE_LIST[@]}; do
	if [ ! $P_CHECK_PARAM "$P_CHECK_PATH" ]; then
		echo "$P_CHECK_PATH is not exist"
		exit
	fi
done

echo "${P_ECHO_PREFIX}检测依赖库文件"
P_CHECK_PARAM=-f
for LIB_NAME in ${P_LIBS_LIST[@]}; do
	P_CHECK_PATH=${P_LIB_PATH}/${LIB_NAME}
	if [ ! $P_CHECK_PARAM "${P_CHECK_PATH}" ]; then
		echo "$P_CHECK_PATH is not exist"
		exit
	fi
done



echo "${P_ECHO_PREFIX}删除原来的bundle"
if [ -d "$P_BUNDLE_PATH" ]; then
	echo "${P_BUNDLE_PATH}"
	rm -rf ${P_BUNDLE_PATH}
fi

if [ -d "$P_BUNDLE_PATH" ]; then
		echo "${P_BUNDLE_PATH} is exist"
		exit
fi


echo "${P_ECHO_PREFIX}执行 QMAKE and MAKE"
cd ${P_BUNDLE_SOURCE_PATH}
make clean
${P_QMAKE_BIN_PATH} ${P_BUNDLE_PROJECT_PATH} -r -spec macx-clang CONFIG+=x86_64
make

if [ ! -d "$P_BUNDLE_PATH" ]; then
	echo "${P_BUNDLE_PATH} is not exist"
	exit
fi

P_APP_PATH=${P_BUNDLE_PATH}/Contents/MacOS/${P_BUNDLE_NAME}
if [ ! -f "$P_APP_PATH" ]; then
	echo "${P_APP_PATH} is not exist"
	exit
fi
for newlibname in ${P_LIBS_LIST[@]}; do
	echo "change ${newlibname}"
	install_name_tool -change "${newlibname}" "${P_LIB_PATH}/${newlibname}" ${P_APP_PATH}
done

# macdeployqt ${P_BUNDLE_PATH}  -verbose=3 -codesign=${C_DEVELOPER_ID}
macdeployqt ${P_BUNDLE_PATH}
# open ${P_BUNDLE_PATH} --args c8a55807-c95a-490e-91c0-74bdbc129904


echo "${P_ECHO_PREFIX}删除原来的bundle"
T_PATH=${CIN_BUNDLE_OUT_PATH}/${P_BUNDLE_NAME}.app
if [ -d "${T_PATH}" ]; then
	echo "rm ${T_PATH}"
	rm -rf ${T_PATH}
fi

if [ -d "${T_PATH}" ]; then
	echo "${T_PATH} is exist"
	exit
fi

cp -R ${P_BUNDLE_PATH} ${CIN_BUNDLE_OUT_PATH}

if [ ! -d "${T_PATH}" ]; then
	echo "${T_PATH} is not exist"
	exit
else
	echo "${T_PATH} is copy"
fi

# open ${T_PATH} --args c8a55807-c95a-490e-91c0-74bdbc129904

echo "finished `date +%Y-%m-%d` `date +%H:%k:%M`"





