#include "Forwarder.hpp"
#include <memory>
#include <cstdio>
#include <cstring>
#include <lib/rls/rls_pdu.hpp>
#include <utils/octet_view.hpp>
#include <utils/common.hpp>
#include <lib/rrc/rrc.hpp>


#include <lib/rrc/encode.hpp>
#include <asn/rrc/ASN_RRC_RRCReject.h>
#include <asn/rrc/ASN_RRC_RRCSetup.h>
#include <asn/rrc/ASN_RRC_UL-CCCH-Message.h>
#include <asn/rrc/ASN_RRC_UL-DCCH-Message.h>


#include <asn/ngap/ASN_NGAP_FiveG-S-TMSI.h>
#include <asn/rrc/ASN_RRC_BCCH-BCH-Message.h>
#include <asn/rrc/ASN_RRC_BCCH-DL-SCH-Message.h>
#include <asn/rrc/ASN_RRC_CellGroupConfig.h>
#include <asn/rrc/ASN_RRC_DL-CCCH-Message.h>
#include <asn/rrc/ASN_RRC_DL-DCCH-Message.h>
#include <asn/rrc/ASN_RRC_DLInformationTransfer-IEs.h>
#include <asn/rrc/ASN_RRC_DLInformationTransfer.h>
#include <asn/rrc/ASN_RRC_PCCH-Message.h>
#include <asn/rrc/ASN_RRC_Paging.h>
#include <asn/rrc/ASN_RRC_PagingRecord.h>
#include <asn/rrc/ASN_RRC_PagingRecordList.h>
#include <asn/rrc/ASN_RRC_RRCRelease-IEs.h>
#include <asn/rrc/ASN_RRC_RRCRelease.h>
#include <asn/rrc/ASN_RRC_RRCSetup-IEs.h>
#include <asn/rrc/ASN_RRC_RRCSetup.h>
#include <asn/rrc/ASN_RRC_RRCSetupComplete-IEs.h>
#include <asn/rrc/ASN_RRC_RRCSetupComplete.h>
#include <asn/rrc/ASN_RRC_RRCSetupRequest.h>
// #include <asn/rrc/ASN_RRC_UL-CCCH-Message.h>
#include <asn/rrc/ASN_RRC_UL-CCCH1-Message.h>
// #include <asn/rrc/ASN_RRC_UL-DCCH-Message.h>
#include <asn/rrc/ASN_RRC_ULInformationTransfer-IEs.h>
#include <asn/rrc/ASN_RRC_ULInformationTransfer.h>

#include "gnb/gnb_msg_handler.hpp"
#include "ue/ue_msg_handler.hpp"


//#include <lib_open5gs/nas/5gs/ogs-nas-5gs.h> // open5GS message lib

const unsigned int BUFSIZE = 0x1000000;

int Forwarder::addr_n_sock::ID_cur = 0;
ASN_RRC_InitialUE_Identity_t m_initialId;
ASN_RRC_EstablishmentCause m_establishmentCause = ASN_RRC_EstablishmentCause_mt_Access;
Socket glob_sock;
InetAddress glob_addr;

Forwarder::Forwarder(const std::string GNB_IP,
        const uint16_t GNB_PORT,
        const std::string IP,
        const uint16_t PORT) : gNB_addr{ GNB_IP, GNB_PORT }, IP{ IP }, PORT{ PORT }

{
    false_gnb_sock = Socket::CreateAndBindUdp({ IP, PORT });
    listen_socks.push_back(false_gnb_sock);
    state_manager.set_testcase(std::string("TestCase1.txt"));
}

int Forwarder::do_work(void)
{
    uint8_t* buffer = new uint8_t[BUFSIZE];
    uint8_t* rt_buf = new uint8_t[BUFSIZE];
    int recv_size;
    int rt_size;
    int rt_val;

    std::vector<Socket> respond_socks;
    std::vector<Socket> dummy;
    InetAddress recv_addr;

    Socket target_ue_handle_sock, target_gnb_handle_sock;
    InetAddress target_ue_addr, target_gnb_addr;

    while (true)
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

            if (recv_sock == false_gnb_sock) //from UEs
            {
                send_addr = gNB_addr;
                glob_addr = send_addr;
                bool find_flag = false;

                for (unsigned int i = 0; i < addr_sock_pair.size(); i++)
                {
                    if (addr_sock_pair[i].addr == recv_addr)
                    {
                        id = addr_sock_pair[i].ID;
                        send_sock = addr_sock_pair[i].sock;
                        find_flag = true;
                        break;
                    }
                }
                if (!find_flag)  //no Socket pair found
                {
                    Socket new_ue2gnb{ Socket::CreateAndBindUdp({IP, 0}) };
                    addr_sock_pair.push_back(addr_n_sock(recv_addr, new_ue2gnb));
                    listen_socks.push_back(new_ue2gnb);
                    send_sock = new_ue2gnb;
                    //
                    glob_sock = send_sock;
                    id = addr_sock_pair[addr_sock_pair.size() - 1].ID;
                }

                target_gnb_handle_sock = send_sock;
                target_gnb_addr = gNB_addr;
                //std::cout << id << ") UE -> gNB" << std::endl;

                rt_val = handle_UEs_packet(buffer, recv_size, rt_buf, rt_size);
            }
            else //from gNB
            {
                send_sock = false_gnb_sock;

                for (unsigned int i = 0; true; i++)
                {
                    if (addr_sock_pair[i].sock == recv_sock)
                    {
                        id = addr_sock_pair[i].ID;
                        send_addr = addr_sock_pair[i].addr;
                        break;
                    }
                    assert(("Can't find addr_sock_pair", i != (addr_sock_pair.size() - 1)));
                }
                //std::cout << id << ") gNB -> UE" << std::endl;

                target_ue_handle_sock = send_sock;
                target_ue_addr = send_addr;

                rt_val = handle_gNBs_packet(buffer, recv_size, rt_buf, rt_size);
            }
            send_sock.send(send_addr, rt_buf, recv_size);

            if(rt_val == 5)
                start_flag = true;

            if(rt_val == 0 && start_flag)
            {
                state_manager.change_state();
                std::cout << state_manager.state << std::endl;
            } 
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
    switch (msg->msgType)
    {
        case rls::EMessageType::PDU_TRANSMISSION:
            {
                std::cout << "PDU transmission" << std::endl;

                rls::RlsMessage& ref_msg = *msg;
                auto& m = (rls::RlsPduTransmission&)ref_msg;
                
                ue_sti = m.sti;
                ue_pduId = m.pduId;

                if (m.pduType == rls::EPduType::DATA){
                    std::cout << "DATA Message" << std::endl;
                    rt_size = data_size;
                    return 5;
                }else if (m.pduType == rls::EPduType::RRC)
                {
                    std::cout << "RRC Message" << std::endl;
                    rrc::RrcChannel channel = static_cast<rrc::RrcChannel>(m.payload);
                    OctetString rrcPdu = std::move(m.pdu);

                    switch (channel)
                    {
                        case rrc::RrcChannel::BCCH_BCH:
                            {
                                auto* pdu = rrc::encode::Decode<ASN_RRC_BCCH_BCH_Message>(asn_DEF_ASN_RRC_BCCH_BCH_Message, rrcPdu);

                                std::cout << "UERANSIM Not Yet, CH: BCCH_BCH" << std::endl;
                                break;
                            }
                        case rrc::RrcChannel::BCCH_DL_SCH:
                            break;
                        case rrc::RrcChannel::DL_CCCH:
                            break;

                        case rrc::RrcChannel::UL_CCCH:
                            {
                                auto* pdu = rrc::encode::Decode<ASN_RRC_UL_CCCH_Message>(asn_DEF_ASN_RRC_UL_CCCH_Message, rrcPdu);
                                if (pdu->message.present != ASN_RRC_UL_CCCH_MessageType_PR_c1)
                                    break;

                                auto& c1 = pdu->message.choice.c1;
                                switch (c1->present)
                                {
                                    case ASN_RRC_UL_CCCH_MessageType__c1_PR_NOTHING:
                                        break;
                                    case ASN_RRC_UL_CCCH_MessageType__c1_PR_rrcSetupRequest:
                                        std::cout << "Receive RRC Setup Request, ";           
                                        handle_rrc_setup_request(*c1->choice.rrcSetupRequest);
                                        break;
                                    case ASN_RRC_UL_CCCH_MessageType__c1_PR_rrcResumeRequest:
                                        std::cout << "Receive RRC Resume Request, ";
                                        handle_rrc_resume_request(*c1->choice.rrcResumeRequest);
                                        break; // todo
                                    case ASN_RRC_UL_CCCH_MessageType__c1_PR_rrcReestablishmentRequest:
                                        std::cout << "Receive RRC Reestablishment Request, ";
                                        break; // todo
                                    case ASN_RRC_UL_CCCH_MessageType__c1_PR_rrcSystemInfoRequest:
                                        std::cout << "Receive RRC System Info Request, ";
                                        break; // todo
                                }
                                std::cout << "CH: UL_CCCH" << std::endl;
                                break;
                            }
                        case rrc::RrcChannel::UL_CCCH1:
                            {
                                auto* pdu = rrc::encode::Decode<ASN_RRC_UL_CCCH1_Message>(asn_DEF_ASN_RRC_UL_CCCH1_Message, rrcPdu);
                                std::cout << "UERANSIM Not Yet, CH: UL_CCCH1" << std::endl;
                                break;
                            }
                        case rrc::RrcChannel::DL_DCCH:
                            break;
                        case rrc::RrcChannel::UL_DCCH:
                            {
                                auto* pdu = rrc::encode::Decode<ASN_RRC_UL_DCCH_Message>(asn_DEF_ASN_RRC_UL_DCCH_Message, rrcPdu);

                                if (pdu->message.present != ASN_RRC_UL_DCCH_MessageType_PR_c1)
                                    break;
                                auto& c1 = pdu->message.choice.c1;
                                switch (c1->present)
                                {
                                    case ASN_RRC_UL_DCCH_MessageType__c1_PR_NOTHING:
                                        break;
                                    case ASN_RRC_UL_DCCH_MessageType__c1_PR_measurementReport:
                                        break; // TODO
                                    case ASN_RRC_UL_DCCH_MessageType__c1_PR_rrcReconfigurationComplete:
                                        break; // TODO
                                    case ASN_RRC_UL_DCCH_MessageType__c1_PR_rrcSetupComplete:
                                        std::cout << "Receive RRC Setup Complete, ";
                                        handle_rrc_setup_complete(*c1->choice.rrcSetupComplete);
                                        break;
                                    case ASN_RRC_UL_DCCH_MessageType__c1_PR_rrcReestablishmentComplete:
                                        break; // TODO
                                    case ASN_RRC_UL_DCCH_MessageType__c1_PR_rrcResumeComplete:
                                        break; // TODO
                                    case ASN_RRC_UL_DCCH_MessageType__c1_PR_securityModeComplete:
                                        std::cout << "Receive RRC Security Mode Complete, ";
                                        handle_rrc_sm_complete(*c1->choice.securityModeComplete);
                                        break; // TODO
                                    case ASN_RRC_UL_DCCH_MessageType__c1_PR_securityModeFailure:
                                        break; // TODO
                                    case ASN_RRC_UL_DCCH_MessageType__c1_PR_ulInformationTransfer:
                                        {
                                            std::cout << "Receive Uplink Information Transfer\n";

                                            ASN_RRC_ULInformationTransfer& nasmsg = *c1->choice.ulInformationTransfer;
                                            OctetString nasPdu = asn::GetOctetString(*nasmsg.criticalExtensions.choice.ulInformationTransfer->dedicatedNAS_Message);

                                            auto* m = new NmUeRrcToNas(NmUeRrcToNas::NAS_DELIVERY);
                                            m->nasPdu = std::move(nasPdu);
                                            OctetView buffer{ m->nasPdu };
                                            auto nasMessage = nas::DecodeNasMessage(buffer);
                                            auto& mmMsg = (const nas::MmMessage&)*nasMessage;
                                            if (mmMsg.sht == nas::ESecurityHeaderType::NOT_PROTECTED)
                                            {
                                                // If any NAS signalling message is received as not integrity protected even though the secure exchange of NAS
                                                // messages has been established by the network, then the NAS shall discard this message
                                                std::cout << "Receive Not Protected NAS Message" << std::endl;
                                                receiveMmMessage((const nas::PlainMmMessage&)mmMsg);
                                            }
                                            else if (mmMsg.sht == nas::ESecurityHeaderType::INTEGRITY_PROTECTED_WITH_NEW_SECURITY_CONTEXT)
                                            {
                                                auto& securedMm = (const nas::SecuredMmMessage&)mmMsg;
                                                auto smcMsg = nas::DecodeNasMessage(OctetView{ securedMm.plainNasMessage });

                                                if (smcMsg->epd != nas::EExtendedProtocolDiscriminator::MOBILITY_MANAGEMENT_MESSAGES ||
                                                        (((const nas::MmMessage&)(*smcMsg)).sht != nas::ESecurityHeaderType::NOT_PROTECTED) ||
                                                        (((const nas::PlainMmMessage&)(*smcMsg)).messageType != nas::EMessageType::SECURITY_MODE_COMMAND))
                                                {
                                                    std::cout << "A valid Security Mode Command expected for given SHT. Ignoring received NAS message" << std::endl;
                                                }

                                                ((nas::SecurityModeCommand&)(*smcMsg))._macForNewSC = securedMm.messageAuthenticationCode;
                                                ((nas::SecurityModeCommand&)(*smcMsg))._originalPlainNasPdu = securedMm.plainNasMessage.copy();

                                                receiveMmMessage((const nas::PlainMmMessage&)(*smcMsg));
                                            }
                                            else if (mmMsg.sht == nas::ESecurityHeaderType::INTEGRITY_PROTECTED_AND_CIPHERED_WITH_NEW_SECURITY_CONTEXT)
                                            {
                                                std::cout << "INTEGRITY_PROTECTED_AND_CIPHERED_WITH_NEW_SECURITY_CONTEXT" << std::endl;
                                            }
                                            else
                                            {
                                                std::cout << "Receive Protected NAS Message" << std::endl;
                                            }

                                            break;
                                        }
                                    case ASN_RRC_UL_DCCH_MessageType__c1_PR_locationMeasurementIndication:
                                        break; // TODO
                                    case ASN_RRC_UL_DCCH_MessageType__c1_PR_ueCapabilityInformation:
                                        break; // TODO
                                    case ASN_RRC_UL_DCCH_MessageType__c1_PR_counterCheckResponse:
                                        break; // TODO
                                    case ASN_RRC_UL_DCCH_MessageType__c1_PR_ueAssistanceInformation:
                                        break; // TODO
                                    case ASN_RRC_UL_DCCH_MessageType__c1_PR_failureInformation:
                                        break; // TODO
                                    case ASN_RRC_UL_DCCH_MessageType__c1_PR_ulInformationTransferMRDC:
                                        break; // TODO
                                    case ASN_RRC_UL_DCCH_MessageType__c1_PR_scgFailureInformation:
                                        break; // TODO
                                    case ASN_RRC_UL_DCCH_MessageType__c1_PR_scgFailureInformationEUTRA:
                                        break; // TODO
                                }

                                std::cout << "CH: UL_DCCH" << std::endl;
                                break;
                            }
                        case rrc::RrcChannel::PCCH:
                            std::cout << "PCCH" << std::endl;
                            break;
                        default:
                            std::cout << "What???" << std::endl;
                    }
                }
                rt_size = data_size;
                return 0;
            }
        case rls::EMessageType::PDU_TRANSMISSION_ACK:
            //std::cout << "PDU ack" << std::endl;
            rt_size = data_size;
            return 1;
        case rls::EMessageType::HEARTBEAT:
            //std::cout << "Heartbeat" << std::endl;
            rt_size = data_size;
            return 2;
        case rls::EMessageType::HEARTBEAT_ACK:
            //std::cout << "Heartbeat ack" << std::endl;
            rt_size = data_size;
            return 3;
        default:
            std::cout << "Why default?" << std::endl;
            rt_size = data_size;
            return -1;
    }

    //std::cout << "\n\n";
    return -2;
}


int Forwarder::handle_gNBs_packet(const uint8_t buf[], const int data_size, uint8_t rt_buf[], int& rt_size)
{
    memcpy(rt_buf, buf, data_size);

    OctetView octet_buf(buf, data_size);

    auto msg = rls::DecodeRlsMessage(octet_buf);
    switch (msg->msgType)
    {
        case rls::EMessageType::PDU_TRANSMISSION:
            {
                std::cout << "PDU transmission" << std::endl;

                rls::RlsMessage& ref_msg = *msg;
                auto& m = (rls::RlsPduTransmission&)ref_msg;
                
                gnb_sti = m.sti;
                gnb_pduId = m.pduId;

                if (m.pduType == rls::EPduType::DATA)
                {
                    std::cout << "DATA Message" << std::endl;
                    rt_size = data_size;
                    return 5;
                }else if (m.pduType == rls::EPduType::RRC)
                {
                    std::cout << "RRC Message" << std::endl;
                    rrc::RrcChannel channel = static_cast<rrc::RrcChannel>(m.payload);
                    OctetString rrcPdu = std::move(m.pdu);

                    switch (channel)
                    {
                        case rrc::RrcChannel::BCCH_BCH:
                            {
                                auto* pdu = rrc::encode::Decode<ASN_RRC_BCCH_BCH_Message>(asn_DEF_ASN_RRC_BCCH_BCH_Message, rrcPdu);
                                if (pdu->message.present == ASN_RRC_BCCH_BCH_MessageType_PR_mib)
                                    std::cout << "Receive Mib, CH: BCCH_BCH" << std::endl;
                                break;
                            }
                        case rrc::RrcChannel::BCCH_DL_SCH:
                            {
                                auto* pdu = rrc::encode::Decode<ASN_RRC_BCCH_DL_SCH_Message>(asn_DEF_ASN_RRC_BCCH_DL_SCH_Message, rrcPdu);
                                if (pdu->message.present != ASN_RRC_BCCH_DL_SCH_MessageType_PR_c1)
                                    break;
                                auto& c1 = pdu->message.choice.c1;
                                switch (c1->present)
                                {
                                    case ASN_RRC_BCCH_DL_SCH_MessageType__c1_PR_systemInformationBlockType1:
                                        std::cout << "Receive Sib1, CH: BCCH_DL_SCH" << std::endl;
                                        //
                                        //startConnectionEstablishment(m_initialId, m_establishmentCause, glob_sock, glob_addr);
                                        break;
                                    default:
                                        break;
                                }
                                break;
                            }
                        case rrc::RrcChannel::DL_CCCH:
                            {
                                auto* pdu = rrc::encode::Decode<ASN_RRC_DL_CCCH_Message>(asn_DEF_ASN_RRC_DL_CCCH_Message, rrcPdu);
                                if (pdu->message.present != ASN_RRC_DL_CCCH_MessageType_PR_c1)
                                    break;

                                auto& c1 = pdu->message.choice.c1;
                                switch (c1->present)
                                {
                                    case ASN_RRC_DL_CCCH_MessageType__c1_PR_rrcReject:
                                        std::cout << "Receive RRC Reject, ";
                                        break;
                                    case ASN_RRC_DL_CCCH_MessageType__c1_PR_rrcSetup:
                                        std::cout << "Receive RRC Setup, ";
                                        handle_rrc_setup(*c1->choice.rrcSetup);
                                        break;
                                    default:
                                        break;
                                }

                                std::cout << "CH: DL_CCCH" << std::endl;
                                break;
                            }
                        case rrc::RrcChannel::UL_CCCH:
                        case rrc::RrcChannel::UL_CCCH1:
                            break;
                        case rrc::RrcChannel::DL_DCCH:
                            {
                                auto* pdu = rrc::encode::Decode<ASN_RRC_DL_DCCH_Message>(asn_DEF_ASN_RRC_DL_DCCH_Message, rrcPdu);
                                auto& c1 = pdu->message.choice.c1;
                                switch (c1->present)
                                {
                                    case ASN_RRC_DL_DCCH_MessageType__c1_PR_dlInformationTransfer:
                                        {
                                            std::cout << "Receive DownLink Information Transfer\n";
                                            ASN_RRC_DLInformationTransfer& nasmsg = *c1->choice.dlInformationTransfer;
                                            OctetString nasPdu = asn::GetOctetString(*nasmsg.criticalExtensions.choice.dlInformationTransfer->dedicatedNAS_Message);

                                            auto* m = new NmUeRrcToNas(NmUeRrcToNas::NAS_DELIVERY);
                                            m->nasPdu = std::move(nasPdu);
                                            OctetView buffer{ m->nasPdu };
                                            auto nasMessage = nas::DecodeNasMessage(buffer);
                                            if (nasMessage->epd == nas::EExtendedProtocolDiscriminator::SESSION_MANAGEMENT_MESSAGES)
                                            {
                                                std::cout << "Bad constructed message received (SM)" << std::endl;
                                            }
                                            auto& mmMsg = (const nas::MmMessage&)*nasMessage;
                                            if (mmMsg.sht == nas::ESecurityHeaderType::NOT_PROTECTED)
                                            {
                                                // If any NAS signalling message is received as not integrity protected even though the secure exchange of NAS
                                                // messages has been established by the network, then the NAS shall discard this message
                                                std::cout << "Receive Not Protected NAS Message" << std::endl;
                                                receiveMmMessage((const nas::PlainMmMessage&)mmMsg);
                                            }
                                            else if (mmMsg.sht == nas::ESecurityHeaderType::INTEGRITY_PROTECTED_WITH_NEW_SECURITY_CONTEXT)
                                            {
                                                auto& securedMm = (const nas::SecuredMmMessage&)mmMsg;
                                                auto smcMsg = nas::DecodeNasMessage(OctetView{ securedMm.plainNasMessage });

                                                if (smcMsg->epd != nas::EExtendedProtocolDiscriminator::MOBILITY_MANAGEMENT_MESSAGES ||
                                                        (((const nas::MmMessage&)(*smcMsg)).sht != nas::ESecurityHeaderType::NOT_PROTECTED) ||
                                                        (((const nas::PlainMmMessage&)(*smcMsg)).messageType != nas::EMessageType::SECURITY_MODE_COMMAND))
                                                {
                                                    std::cout << "A valid Security Mode Command expected for given SHT. Ignoring received NAS message" << std::endl;
                                                }

                                                ((nas::SecurityModeCommand&)(*smcMsg))._macForNewSC = securedMm.messageAuthenticationCode;
                                                ((nas::SecurityModeCommand&)(*smcMsg))._originalPlainNasPdu = securedMm.plainNasMessage.copy();

                                                receiveMmMessage((const nas::PlainMmMessage&)(*smcMsg));
                                            }
                                            else if (mmMsg.sht == nas::ESecurityHeaderType::INTEGRITY_PROTECTED_AND_CIPHERED_WITH_NEW_SECURITY_CONTEXT)
                                            {
                                                std::cout << "Bad constructed message received (SHT)" << std::endl;
                                            }
                                            else
                                            {
                                                std::cout << "Receive Protected NAS Message" << std::endl;
                                            }
                                            break;
                                        }
                                    case ASN_RRC_DL_DCCH_MessageType__c1_PR_rrcRelease:
                                        std::cout << "Receive RRC Release, ";
                                        handle_rrc_release(*c1->choice.rrcRelease);
                                        break;
                                    case ASN_RRC_DL_DCCH_MessageType__c1_PR_securityModeCommand:
                                        std::cout << "Receive RRC SecurityModeCommand, ";
                                        handle_rrc_sm_command(*c1->choice.securityModeCommand);
                                        break;
                                    default:
                                        break;
                                }
                                std::cout << "CH: DL_DCCH" << std::endl;
                                break;
                            }
                        case rrc::RrcChannel::UL_DCCH:
                            break;
                        case rrc::RrcChannel::PCCH:
                            {
                                auto* pdu = rrc::encode::Decode<ASN_RRC_PCCH_Message>(asn_DEF_ASN_RRC_PCCH_Message, rrcPdu);

                                if (pdu->message.present != ASN_RRC_PCCH_MessageType_PR_c1)
                                    break;

                                auto& c1 = pdu->message.choice.c1;
                                switch (c1->present)
                                {
                                    case ASN_RRC_PCCH_MessageType__c1_PR_paging:
                                        std::cout << "Receive Paging, ";
                                        handle_paging_tmsi(*c1->choice.paging);
                                        break;
                                    default:
                                        break;
                                }
                                std::cout << "CH: PCCH" << std::endl;
                                break;
                            }
                        default:
                            std::cout << "What???" << std::endl;
                    }
                }
                rt_size = data_size;
                return 0;
            }
        case rls::EMessageType::PDU_TRANSMISSION_ACK:
            //std::cout << "PDU ack" << std::endl;
            rt_size = data_size;
            return 1;
        case rls::EMessageType::HEARTBEAT:
            //std::cout << "Heartbeat" << std::endl;
            rt_size = data_size;
            return 2;
        case rls::EMessageType::HEARTBEAT_ACK:
            //std::cout << "Heartbeat ack" << std::endl;
            rt_size = data_size;
            return 3;
        default:
            std::cout << "Why default?" << std::endl;
            return -1;
    }
    //std::cout << "\n\n";
    return -2;
}

