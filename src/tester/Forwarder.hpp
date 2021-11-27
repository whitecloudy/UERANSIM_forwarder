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
#include "Parser.hpp"
#include "StateManager.hpp"

#include <asn/rrc/ASN_RRC_PCCH-MessageType.h>
#include <asn/rrc/ASN_RRC_DL-DCCH-MessageType.h>
#include <asn/rrc/ASN_RRC_UL-CCCH-MessageType.h>
#include <asn/rrc/ASN_RRC_UL-DCCH-MessageType.h>

#include <asn/rrc/ASN_RRC_RRCRelease.h>
#include <asn/rrc/ASN_RRC_RRCSetup.h>
#include <asn/rrc/ASN_RRC_RRCRelease.h>
#include <asn/rrc/ASN_RRC_Paging.h>

#include <asn/rrc/ASN_RRC_RRCSetupRequest.h>
#include <asn/rrc/ASN_RRC_SecurityModeComplete.h>
#include <asn/rrc/ASN_RRC_RRCResumeRequest.h>
#include <asn/rrc/ASN_RRC_RRCSetupComplete.h>
#include <asn/rrc/ASN_RRC_InitialUE-Identity.h>
#include <lib/asn/utils.hpp>


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

        Parser act_parser;
        StateManager state_manager;

        bool start_flag = false;

    public:
        Forwarder(const std::string GNB_IP, 
                const uint16_t GNB_PORT,
                const std::string IP,
                const uint16_t PORT);

        int do_work(void);

    private:
        int handle_UEs_packet(const uint8_t buf[], const int data_size, uint8_t rt_buf[], int& rt_size);
        int handle_gNBs_packet(const uint8_t buf[], const int data_size, uint8_t rt_buf[], int& rt_size);

    private:
        int handle_paging_tmsi(const ASN_RRC_Paging &msg);
        int handle_rrc_setup(const ASN_RRC_RRCSetup &msg);
        int handle_rrc_sm_command(const ASN_RRC_SecurityModeCommand &msg);
        int handle_rrc_release(const ASN_RRC_RRCRelease &msg);

        int handle_rrc_setup_request(const ASN_RRC_RRCSetupRequest &msg);
        int handle_rrc_sm_complete(const ASN_RRC_SecurityModeComplete &msg);
        int handle_rrc_resume_request(const ASN_RRC_RRCResumeRequest &msg);
        int handle_rrc_setup_complete(const ASN_RRC_RRCSetupComplete &msg);

    private:
        void makeRrcMessage(ASN_RRC_BCCH_BCH_Message *msg);
        void makeRrcMessage(ASN_RRC_BCCH_DL_SCH_Message *msg);
        void makeRrcMessage(int ueId, ASN_RRC_DL_CCCH_Message *msg);
        void makeRrcMessage(int ueId, ASN_RRC_DL_DCCH_Message *msg);
        void makeRrcMessage(ASN_RRC_PCCH_Message *msg);





};

static ASN_RRC_UL_CCCH_Message *ConstructSetupRequest(ASN_RRC_InitialUE_Identity_t initialUeId,ASN_RRC_EstablishmentCause_t establishmentCause);
void startConnectionEstablishment(ASN_RRC_InitialUE_Identity_t m_initialId, ASN_RRC_EstablishmentCause m_establishmentCause);
void sendRrcMessage(int cellId, ASN_RRC_UL_CCCH_Message *msg);



#endif
