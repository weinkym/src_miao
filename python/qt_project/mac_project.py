import os

from qt_project import mac_utils as OTL


class VersionInfo:
    def __init__(self):
        self.major = 0
        self.minor = 0
        self.patch = 0
        self.build = 0

    def __str__(self):
        return self.fullVersion()

    def fullVersion(self):
        return '{}.{}.{}.{}'.format(self.major, self.minor, self.patch,
                                    self.build)

    def shortVersion(self):
        return '{}.{}.{}'.format(self.major, self.minor, self.patch)


def getProjectVersionFP(project_dp, need_check=True):
    fp = '{}/ljobs/base/cversion_mac.cpp'.format(project_dp)
    if (need_check):
        OTL.checkPathWithRaise(fp)
    return fp


def getVersionInfo(fp):
    if not os.path.exists(fp):
        return False
    info = VersionInfo()
    with open(fp, 'r') as f:
        line_list = f.readlines()
        # print(line_list)

        for line in line_list:
            if 'C_VERSION_MAJOR' in line:
                info.major = getKeyNumber(line, 'C_VERSION_MAJOR')
            if 'C_VERSION_MINOR' in line:
                info.minor = getKeyNumber(line, 'C_VERSION_MINOR')
            if 'C_VERSION_PATCH' in line:
                info.patch = getKeyNumber(line, 'C_VERSION_PATCH')
            if 'C_VERSION_BUILD' in line:
                info.build = getKeyNumber(line, 'C_VERSION_BUILD')
    return info


def getKeyNumber(line, key):
    aduest_line = line.lstrip().replace(' ', '')
    # print('aduest_line={}'.format(aduest_line))
    res = ''
    index = aduest_line.find(key)
    if index > 0:
        i = index + 1 + len(key)
        while i < len(aduest_line):
            c = aduest_line[i]
            # print('c={}'.format(c))
            if c == ';':
                break
            res = res + c
            i = i + 1
    return res