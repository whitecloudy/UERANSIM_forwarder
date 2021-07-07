from socket import *
import select
from multiprocessing import Process, Event


HOST = '127.8.8.8'
PORT = 4997
BUFSIZE = 2**20
UE_GNB_ADDR = (HOST, PORT)
GNB_UE_ADDR = (HOST, 0)

GNB_ADDR = ('127.0.0.1',PORT)

def process_node(node, term_event):
    while not term_event.is_set():
        node.wait_relay()
        print("timeout occur")


class Node:
    def __init__(self, node_ip, node_port, gnb_addr):
        self.gnb_addr = gnb_addr
        self.node_ip = node_ip
        self.node_port = node_port

        self.ue_listen_sock = socket(AF_INET, SOCK_DGRAM)
        self.ue_listen_sock.bind((node_ip, node_port))

        self.ueAddr_sock = {}
        self.sock_ueAddr = {}

    def run(self):
        sock_for_read = [self.ue_listen_sock] + list(self.ueAddr_sock.values())
        r_sock_l, w_sock_l, x_sock_l = select.select(sock_for_read, [], [])

        for sock in r_sock_l:
            data, from_addr = sock.recvfrom(BUFSIZE)
            print(data, from_addr)
            # packet from UE
            if sock is self.ue_listen_sock:
                relay_sock = None

                if from_addr in self.ueAddr_sock:
                    relay_sock = self.ueAddr_sock[from_addr]
                else:   #new UE
                    relay_sock = socket(AF_INET, SOCK_DGRAM)
                    relay_sock.bind((self.node_ip, 0))

                    self.ueAddr_sock[from_addr] = relay_sock
                    self.sock_ueAddr[relay_sock] = from_addr

                relay_sock.sendto(data, self.gnb_addr)
            # packet from gNB
            else:
                relay_addr = self.sock_ueAddr[sock]
                self.ue_listen_sock.sendto(data, relay_addr)

        """
        try:
            data, from_addr = self.sock.recvfrom(BUFSIZE)
        except timeout:
            return

        if self.from_addr is None:
            self.from_addr = from_addr
        elif self.from_addr != from_addr:
            raise NameError("Address is not correct with this Node")

        if self.relay_Node is not None:
            self.relay_Node.do_relay(data)
        """






def main():
    # initialize Node
    node_obj = Node(HOST, PORT, GNB_ADDR)
    while True:
        node_obj.run()

if __name__=="__main__":
    main()
