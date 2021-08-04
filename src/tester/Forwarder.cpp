#include "Forwarder.hpp"
#include <memory>
#include <cstdio>
#include <cstring>
#include <lib/rls/rls_pdu.hpp>
#include <utils/octet_view.hpp>
#include <lib/rrc/rrc.hpp>

const unsigned int BUFSIZE = 0x1000000;

int Forwarder::addr_n_sock::ID_cur = 0;

Forwarder::Forwarder(const std::string GNB_IP,
                     const uint16_t GNB_PORT,
                     const std::string IP,
                     const uint16_t PORT) : gNB_addr{GNB_IP, GNB_PORT}, IP{IP}, PORT{PORT}

{
    false_gnb_sock = Socket::CreateAndBindUdp({IP, PORT});
    listen_socks.push_back(false_gnb_sock);
}

int Forwarder::do_work(void)
{
    uint8_t * buffer = new uint8_t[BUFSIZE];
    uint8_t * rt_buf = new uint8_t[BUFSIZE];
    int recv_size;
    int rt_size;
    
    std::vector<Socket> respond_socks;
    std::vector<Socket> dummy;
    InetAddress recv_addr;

    while(true)
    {
        respond_socks.clear();
        Socket::Select(listen_socks, dummy, respond_socks, dummy);

        for (unsigned int sock_idx = 0; sock_idx < respond_socks.size(); sock_idx++)
        {
            Socket recv_sock = respond_socks[sock_idx];
            recv_size = recv_sock.receive(buffer, BUFSIZE, 0, recv_addr);

            int id;
            Socket send_sock;
            InetAddress send_addr;

            if(recv_sock == false_gnb_sock) //from UEs
            {
                send_addr = gNB_addr; 
                bool find_flag = false;

                for(unsigned int i = 0; i < addr_sock_pair.size(); i++)
                {
                    if(addr_sock_pair[i].addr == recv_addr)
                    {
                        id = addr_sock_pair[i].ID;
                        send_sock = addr_sock_pair[i].sock;
                        find_flag = true;
                        break;
                    }
                }
                if(!find_flag)  //no Socket pair found
                {
                    Socket new_ue2gnb{Socket::CreateAndBindUdp({IP, 0})};
                    addr_sock_pair.push_back(addr_n_sock(recv_addr, new_ue2gnb));
                    listen_socks.push_back(new_ue2gnb);
                    send_sock = new_ue2gnb;
                    id = addr_sock_pair[addr_sock_pair.size()-1].ID;
                }

                std::cout << id << ") UE -> gNB" <<std::endl;

                handle_UEs_packet(buffer, recv_size, rt_buf, rt_size);
            }else //from gNB
            {
                send_sock = false_gnb_sock;

                for(unsigned int i = 0; true; i++)
                {
                    if(addr_sock_pair[i].sock == recv_sock)
                    {
                        id = addr_sock_pair[i].ID;
                        send_addr = addr_sock_pair[i].addr;
                        break;
                    }
                    assert(("Can't find addr_sock_pair", i != (addr_sock_pair.size() - 1)));
                }
                std::cout << id << ") gNB -> UE"<<std::endl;

                handle_gNBs_packet(buffer, recv_size, rt_buf, rt_size);
            }
            send_sock.send(send_addr, rt_buf, recv_size);
        }
    }

    delete(buffer);
    delete(rt_buf);
}


int Forwarder::handle_UEs_packet(const uint8_t buf[], const int data_size, uint8_t rt_buf[], int& rt_size)
{
    memcpy(rt_buf, buf, data_size);
    
    OctetView octet_buf(buf, data_size);

    auto msg = rls::DecodeRlsMessage(octet_buf);
    switch(msg->msgType)
    {
        case rls::EMessageType::PDU_TRANSMISSION:
            {
                std::cout << "PDU transmission"<<std::endl;
                
                rls::RlsMessage &ref_msg = *msg;
                auto &m = (rls::RlsPduTransmission &)ref_msg;
                if (m.pduType == rls::EPduType::DATA)
                    std::cout << "It is DATA"<<std::endl;
                else if (m.pduType == rls::EPduType::RRC)
                {
                    std::cout << "It is RRC" << std::endl;
                    rrc::RrcChannel channel = static_cast<rrc::RrcChannel>(m.payload);
                    switch(channel)
                    {
                        case rrc::RrcChannel::BCCH_BCH:
                        case rrc::RrcChannel::BCCH_DL_SCH:
                            std::cout << "BCCH"<<std::endl;
                            break;
                        case rrc::RrcChannel::DL_CCCH:
                        case rrc::RrcChannel::UL_CCCH:
                        case rrc::RrcChannel::UL_CCCH1:
                            std::cout << "CCCH" <<std::endl;
                            break;
                        case rrc::RrcChannel::DL_DCCH:
                        case rrc::RrcChannel::UL_DCCH:
                            std::cout << "DCCH"<<std::endl;
                            break;
                        case rrc::RrcChannel::PCCH:
                            std::cout << "PCCH"<<std::endl;
                            break;
                        default:
                            std::cout << "What???"<<std::endl;
                    }
                }
                break;
            }
        case rls::EMessageType::PDU_TRANSMISSION_ACK:
            std::cout << "PDU ack"<<std::endl;
            break;
        case rls::EMessageType::HEARTBEAT:
            std::cout << "Heartbeat"<<std::endl;
            break;
        case rls::EMessageType::HEARTBEAT_ACK:
            std::cout << "Heartbeat ack" << std::endl;
            break;
        default:
            std::cout << "Why default?" <<std::endl;
    }

    rt_size = data_size;
    return 0;
}


int Forwarder::handle_gNBs_packet(const uint8_t buf[], const int data_size, uint8_t rt_buf[], int& rt_size)
{
    memcpy(rt_buf, buf, data_size);

    OctetView octet_buf(buf, data_size);

    auto msg = rls::DecodeRlsMessage(octet_buf);
    switch(msg->msgType)
    {
        case rls::EMessageType::PDU_TRANSMISSION: 
            {
                std::cout << "PDU transmission"<<std::endl;
                
                rls::RlsMessage &ref_msg = *msg;
                auto &m = (rls::RlsPduTransmission &)ref_msg;
                if (m.pduType == rls::EPduType::DATA)
                    std::cout << "It is DATA"<<std::endl;
                else if (m.pduType == rls::EPduType::RRC)
                {
                    std::cout << "It is RRC" << std::endl;
                    rrc::RrcChannel channel = static_cast<rrc::RrcChannel>(m.payload);
                    switch(channel)
                    {
                        case rrc::RrcChannel::BCCH_BCH:
                        case rrc::RrcChannel::BCCH_DL_SCH:
                            std::cout << "BCCH"<<std::endl;
                            break;
                        case rrc::RrcChannel::DL_CCCH:
                        case rrc::RrcChannel::UL_CCCH:
                        case rrc::RrcChannel::UL_CCCH1:
                            std::cout << "CCCH" <<std::endl;
                            break;
                        case rrc::RrcChannel::DL_DCCH:
                        case rrc::RrcChannel::UL_DCCH:
                            std::cout << "DCCH"<<std::endl;
                            break;
                        case rrc::RrcChannel::PCCH:
                            std::cout << "PCCH"<<std::endl;
                            break;
                        default:
                            std::cout << "What???"<<std::endl;
                    }
                }
                break;
            }
        case rls::EMessageType::PDU_TRANSMISSION_ACK:
            std::cout << "PDU ack"<<std::endl;
            break;
        case rls::EMessageType::HEARTBEAT:
            std::cout << "Heartbeat"<<std::endl;
            break;
        case rls::EMessageType::HEARTBEAT_ACK:
            std::cout << "Heartbeat ack" << std::endl;
            break;
        default:
            std::cout << "Why default?" <<std::endl;
    }

    rt_size = data_size;
    return 0;
}
