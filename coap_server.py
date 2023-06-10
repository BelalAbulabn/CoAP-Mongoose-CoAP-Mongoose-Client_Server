import socket
import random
from coapthon.resources.resource import Resource
from coapthon.server.coap import CoAP


class Tempreture(Resource):
    def __init__(self, name="tempreture", coap_server=None):
        super(Tempreture, self).__init__(name, coap_server, visible=True,
                                            observable=False, allow_children=True)
        self.resource_type= "tempreture [C]"
        
        

    def render_GET(self, request):
        self.payload = str(random.uniform(20.0,30.0))[:4] + " C"
        print(self.payload)
        return self


class Light(Resource):
    def __init__(self, name="light", coap_server=None):
        super(Light, self).__init__(name, coap_server, visible=True,
                                            observable=False, allow_children=True)
        self.payload = "off"
        

    def render_GET(self, request):

        return self

    def render_PUT(self, request):
        self.edit_resource(request)
        return self

    def render_POST(self, request):
        res = self.init_resource(request, Light())
        return res

class Button(Resource):
    def __init__(self, name="button", coap_server=None):
        super(Button, self).__init__(name, coap_server, visible=True,
                                            observable=False, allow_children=True)
        self.payload = "off"
        
    def render_GET(self, request):

        return self
    def render_PUT(self, request):
        self.edit_resource(request)
        return self

    def render_POST(self, request):
        res = self.init_resource(request, Light())
        return res

class CoAPServer(CoAP):
    def __init__(self, host, port, multicast=False):
        CoAP.__init__(self, (host, port), multicast)
        self.add_resource('temp/', Tempreture())
        self.add_resource('light/', Light())
        self.add_resource('button/',Button())
        print("CoAP Server start on " + host + ":" + str(port))
        print(self.root.dump())


def get_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        # doesn't even have to be reachable
        s.connect(('10.255.255.255', 1))
        IP = s.getsockname()[0]
    except Exception:
        IP = '127.0.0.1'
    finally:
        s.close()
    return IP


if __name__ == "__main__":
    # automatic IP select or hard code it below
    print("here is main!? ")
    ip = get_ip()
    print(ip)

    port = 5683
    multicast = False
    server = CoAPServer(ip, port, multicast)
    print("still executing !? ")
    server.listen(10)
    print("still executing after this !? ")
    server.close()
    print("close")
