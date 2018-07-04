from abc import ABCMeta, abstractmethod


class IConnection(object):
    __metaclass__ = ABCMeta

    @abstractmethod
    def connect(self):
        raise NotImplementedError

    @abstractmethod
    def disconnect(self):
        raise NotImplementedError