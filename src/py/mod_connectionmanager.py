from mod_streammasterconnection import StreamMasterConnection

class ConnectionManager(object):

    def __init__(self):
        self.__connections = []
        self.createConnection()

    def createConnection(self):
        self.__connections.append(StreamMasterConnection())

    @property
    def count(self):
        return len(self.__connections)

    def getConnectionByName(self, name):
        for connection in self.__connections:
            if connection.name is name:
                return connection
                
        return None