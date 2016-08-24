TARGET = lecai
TEMPLATE = subdirs
#CONFIG += ordered

#include(global.prf)
SUBDIRS += test_QSystemTrayIcon


#SUBDIRS += cpublic \
#           cdbcache \
#           cscreenshot \
#           main \
#           bizlogic

#cdbcache.depends = cpublic
#cscreenshot.depends = cpublic
#main.depends = cpublic cdbcache cscreenshot bizlogic
#bizlogic.depends = cpublic cdbcache



#TEMPLATE = subdirs
