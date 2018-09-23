import threading
import rpyc
from rpyc.utils.server import ThreadedServer
from rpyc.utils.server import OneShotServer
import qttester

class Unbuffered(object):
   def __init__(self, stream):
       self.stream = stream
   def write(self, data):
       self.stream.write(data)
       self.stream.flush()
   def writelines(self, datas):
       self.stream.writelines(datas)
       self.stream.flush()
   def __getattr__(self, attr):
       return getattr(self.stream, attr)

import sys
sys.stdout = Unbuffered(sys.stdout)
sys.sterr = Unbuffered(sys.stderr)

server = None
class MyService(rpyc.Service):
    def on_connect(self, conn):
        # code that runs when a connection is created
        # (to init the service, if needed)
        print("client connected")

    def on_disconnect(self, conn):
        # code that runs after the connection has already closed
        # (to finalize the service, if needed)
        # print("client disconnected")
        try:
            qttester.end_test()
            server.close()
            print("client disconnected")
        except Exception as e:
            print("exception while disconnecting: " + str(e))

    def exposed_application(self):
        return qttester.ScriptableApplication()


# server = ThreadedServer(MyService, port=18861, protocol_config={
server = OneShotServer(MyService, port=18861, protocol_config={
    'allow_public_attrs': True,
    'allow_setattr': True,
})
server.start()

def close():
    try:
        server.close()
        sys.stdout.flush()
        sys.stderr.flush()
    except Exception as e:
        print("Exception while closing:", str(e))
