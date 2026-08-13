import os
import sys
import logging
from collections import deque
from rich.logging import RichHandler
from common.utils import get_log_dir
from logging.handlers import TimedRotatingFileHandler


class MemoryHandler(logging.Handler):
    def __init__(self, logfname=None, maxlen=5000):
        super(MemoryHandler, self).__init__()
        self.records = deque(maxlen=maxlen)
        self.logfname = f"{logfname.replace(".log", "")}_trace.log"
        self.setLevel(logging.DEBUG)

    def emit(self, record):
        logstr = self.format(record)
        self.records.append(logstr)
        if record.levelno >= logging.ERROR:
            with open(self.logfname, mode='a+') as ofd:
                ofd.write("\n".join(self.records))
                ofd.write("\n")
            self.records.clear()
        if len(self.records) > 1000:
            print("Memory Trace Handler: Dropping records")
            self.records = self.records[:-1000]

class Logger:
    logger = None
    def __init__(self, logfname, fmtstr, level=logging.INFO):
        self.logfname = os.path.join(get_log_dir(), logfname)
        self.level = level
        self.format_str = fmtstr
        self.formatter = None
        self.mh = None
        self.fh = None
        self.ch = None
        self.root_logger = logging.getLogger()

    def setup_format(self, fmtstr):
        self.formatter = logging.Formatter(fmtstr)
        self.ch.setFormatter(logging.Formatter('%(message)s'))
        self.fh.setFormatter(self.formatter)
        self.mh.setFormatter(self.formatter)

    def setup(self):
        self.mh = MemoryHandler(self.logfname)
        self.fh = TimedRotatingFileHandler(filename=self.logfname, when='D', interval=1)
        self.ch = RichHandler()
        self.setup_format(self.format_str)
        self.root_logger.handlers.clear()
        self.root_logger.addHandler(self.fh)
        self.root_logger.addHandler(self.ch)
        self.root_logger.addHandler(self.mh)
        self.root_logger.setLevel(logging.DEBUG)
        self.fh.setLevel(self.level)
        self.ch.setLevel(self.level)
        self.mh.setLevel(logging.DEBUG)

    def quiet(self):
        self.root_logger.removeHandler(self.ch)

def setup_logging(logfname=None,
                  fmtstr='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
                  level=logging.INFO):
    if logfname is None:
        basefname = sys.argv[0].split(os.path.sep)[-1]
        logfname = f"{'.'.join(basefname.split('.')[:-1])}.log"
    if Logger.logger is None:
        Logger.logger = Logger(logfname, fmtstr, level)
        Logger.logger.setup()
    return Logger.logger.root_logger

def setup_logging_cliargs(cliargs):
    logfname = cliargs.log_file or None
    level = logging.DEBUG if cliargs.debug else logging.INFO
    logger = setup_logging(logfname, level=level)
    if cliargs.quiet:
        Logger.logger.quiet()
    logger.info(f"Logging to {Logger.logger.logfname}")


def add_logger_args(parser):
    parser.add_argument('--log-file')
    parser.add_argument('--quiet', action='store_true')
    parser.add_argument('--debug', '--verbose', action='store_true')