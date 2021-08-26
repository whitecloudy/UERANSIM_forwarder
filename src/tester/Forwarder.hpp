#ifndef __FORWARDER__
#define __FORWARDER__

#include <iostream>
#include <string>
#include <vector>
#include <utils/network.hpp>
#include <lib/app/cli_cmd.hpp>
#include <memory>
#include <utils/nts.hpp>
#include "Forwarder_Nts.hpp"
#include "Forwarder_Message.hpp"

class Forwarder
{
    private:
        struct addr_n_sock
        {
            static int ID_cur;

            int ID;
            InetAddress addr;
            Socket sock;

            addr_n_sock(InetAddress addr_t, Socket sock_t) : addr{addr_t}, sock{sock_t}, ID{ID_cur++} {};
        };

    private:
        Socket false_gnb_sock;

        std::string IP;
        uint16_t PORT;

        std::vector<Socket> listen_socks;
        std::vector<struct addr_n_sock> addr_sock_pair;
        
        InetAddress gNB_addr;

    public:
        Forwarder(const std::string GNB_IP, 
                const uint16_t GNB_PORT,
                const std::string IP,
                const uint16_t PORT);

        int do_work(void);

    private:
        int handle_UEs_packet(const uint8_t buf[], const int data_size, uint8_t rt_buf[], int& rt_size);
        int handle_gNBs_packet(const uint8_t buf[], const int data_size, uint8_t rt_buf[], int& rt_size);
};

static ASN_RRC_UL_CCCH_Message *ConstructSetupRequest(ASN_RRC_InitialUE_Identity_t initialUeId,ASN_RRC_EstablishmentCause_t establishmentCause);
void startConnectionEstablishment(ASN_RRC_InitialUE_Identity_t m_initialId, ASN_RRC_EstablishmentCause m_establishmentCause);
void sendRrcMessage(int cellId, ASN_RRC_UL_CCCH_Message *msg);



#endif
