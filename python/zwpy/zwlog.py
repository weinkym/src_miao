import logging
import logging.handlers
import threading


class ZWLog(object):
    _instance_lock = threading.Lock()

    def __init__(self):
        self.level = logging.DEBUG
        self.formatter = logging.Formatter(
            '[%(asctime)s] - %(name)s - %(levelname)s - %(message)s')
        self.log = logging.getLogger()
        self.log.setLevel(self.level)
        self.default_handler = logging.StreamHandler()
        self.default_handler.setLevel(self.level)

        self.default_handler.setFormatter(self.formatter)
        self.log.addHandler(self.default_handler)
        self.handlers = [self.default_handler]

    @classmethod
    def instance(cls, *args, **kwargs):
        if not hasattr(ZWLog, "_instance"):
            with ZWLog._instance_lock:
                if not hasattr(ZWLog, "_instance"):
                    ZWLog._instance = ZWLog(*args, **kwargs)
        return ZWLog._instance

    def debug(self, msg):
        self.log.debug(msg)

    def info(self, msg):
        self.log.info(msg)

    def warning(self, msg):
        self.log.warning(msg)

    def error(self, msg):
        self.log.error(msg)

    def addLogFile(self, fp):
        fh = logging.FileHandler(fp)
        fh.setLevel(self.level)
        fh.setFormatter(self.formatter)
        self.log.addHandler(fh)
        self.handlers.append(fh)


def debug(msg):
    ZWLog.instance().debug(msg)


def info(msg):
    ZWLog.instance().info(msg)


def warning(msg):
    ZWLog.instance().warning(msg)


def error(msg):
    ZWLog.instance().error(msg)
