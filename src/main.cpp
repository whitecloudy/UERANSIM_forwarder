#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cassert>
#include <cstdio>
#include <utils/network.hpp>


const std::string IP_ADDR = "127.8.8.8";
const uint16_t PORT       = 4997;

const std::string GNB_ADDR = "127.0.0.1";
const uint16_t GNB_PORT    = 4997;

const unsigned int BUFSIZE = 16777216;


struct addr_n_sock
{
  InetAddress addr;
  Socket sock;

  addr_n_sock(InetAddress addr_t, Socket sock_t) : addr{addr_t}, sock{sock_t} {};
};

int main(int argc, char ** argv)
{
  Socket false_gnb_sock = Socket::CreateAndBindUdp({IP_ADDR, PORT});
  //Socket false_gnb_sock;

  std::vector<Socket> listen_socks;
  std::vector<Socket> dummy;

  std::vector<struct addr_n_sock> addr_sock_pair;

  listen_socks.push_back(false_gnb_sock);

  uint8_t * buffer = new uint8_t[BUFSIZE];

  InetAddress recv_addr;

  
  while(true)
  {
    Socket recv_sock = Socket::Select(listen_socks, dummy);

    int recv_size = recv_sock.receive(buffer, BUFSIZE, 0, recv_addr);

    Socket send_sock;
    InetAddress send_addr;

    if(recv_sock == false_gnb_sock) //from UEs
    {
      std::cout << "UE -> gNB" <<std::endl;
      send_addr = InetAddress(GNB_ADDR, GNB_PORT);

      bool find_flag = false;

      for(unsigned int i = 0; i < addr_sock_pair.size(); i++)
      {
        if(addr_sock_pair[i].addr == recv_addr)
        {
          send_sock = addr_sock_pair[i].sock;
          find_flag = true;
          break;
        }
      }
      if(!find_flag)
      {
        Socket new_ue2gnb{Socket::CreateAndBindUdp({IP_ADDR, 0})};
        addr_sock_pair.push_back(addr_n_sock(recv_addr, new_ue2gnb));
        listen_socks.push_back(new_ue2gnb);
        send_sock = new_ue2gnb;
      }
    }else //from gNB
    {

      std::cout << "gNB -> UE"<<std::endl;
      send_sock = false_gnb_sock;

      for(unsigned int i = 0; true; i++)
      {
        if(addr_sock_pair[i].sock == recv_sock)
        {
          send_addr = addr_sock_pair[i].addr;
          break;
        }
        if(i == (addr_sock_pair.size() - 1))
        {
          assert(("Can't find addr_sock_pair",false));
          break;
        }
      }

    }

    send_sock.send(send_addr, buffer, recv_size);
  }

  delete buffer;  

  return 0;
}
