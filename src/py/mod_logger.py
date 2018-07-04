class LogLevel:
    INFO = 0
    WARNING = 1
    ERROR = 2


def MOD_LOG(message, level = LogLevel.INFO):
    if level is LogLevel.WARNING:
        log_warning(message)

    elif level is LogLevel.ERROR:
        log_error(message)

    else:
        log_info(message)

def log_info(message):
    print 'MOD_LOG - [INFO]: ' + message

def log_warning(message):
    print 'MOD_LOG - [WARNING]: ' + message

def log_error(message):
    print 'MOD_LOG - [ERROR]: ' + message