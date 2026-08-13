import os


def get_home_dir():
    return os.getenv("HOME") or os.getenv("USERPROFILE")


def get_log_dir():
    return os.path.join(get_home_dir(), ".local", "logs")


def get_conf_name():
    return os.path.join(get_home_dir(), ".local", "conf")


# End of file