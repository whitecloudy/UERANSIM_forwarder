#include "Forwarder.hpp"

#include <string>
#include <iostream>
#include <lib/rrc/encode.hpp>




int Forwarder::handle_rrc_setup_request(const ASN_RRC_RRCSetupRequest &msg)
{
    int64_t ue_id;
    bool isTMSI = false;

    if (msg.rrcSetupRequest.ue_Identity.present == ASN_RRC_InitialUE_Identity_PR_ng_5G_S_TMSI_Part1)
    {
        ue_id = asn::GetBitStringLong<39>(msg.rrcSetupRequest.ue_Identity.choice.ng_5G_S_TMSI_Part1);
        isTMSI = true;
    }
    else
    {
        ue_id = asn::GetBitStringLong<39>(msg.rrcSetupRequest.ue_Identity.choice.randomValue);
        isTMSI = false;

    }

    if(start_flag)
    {
        std::string parsed_str = act_parser.set_recv_act_string(ASN_RRC_DL_CCCH_MessageType__c1_PR_rrcSetup, CCCH, UL, "");
        std::cout << "parsed : " << parsed_str << std::endl;
        state_manager.receive_string(parsed_str);
    }
}

int Forwarder::handle_rrc_sm_complete(const ASN_RRC_SecurityModeComplete &msg)
{
    if(start_flag)
    {
        std::string parsed_str = act_parser.set_recv_act_string(ASN_RRC_UL_DCCH_MessageType__c1_PR_securityModeComplete, DCCH, UL, "");
        std::cout << "parsed : " << parsed_str << std::endl;
        state_manager.receive_string(parsed_str);
    }
}

int Forwarder::handle_rrc_resume_request(const ASN_RRC_RRCResumeRequest &msg)
{
    if(start_flag)
    {
        std::string parsed_str = act_parser.set_recv_act_string(ASN_RRC_UL_CCCH_MessageType__c1_PR_rrcResumeRequest, CCCH, UL, "");
        std::cout << "parsed : " << parsed_str << std::endl;
        state_manager.receive_string(parsed_str);
    }

}

int Forwarder::handle_rrc_setup_complete(const ASN_RRC_RRCSetupComplete &msg)
{
    if(start_flag)
    {
        std::string parsed_str = act_parser.set_recv_act_string(ASN_RRC_UL_DCCH_MessageType__c1_PR_rrcSetupComplete, DCCH, UL, "");
        std::cout << "parsed : " << parsed_str << std::endl;
        state_manager.receive_string(parsed_str);
    }
}

int Forwarder::handle_paging_tmsi(const ASN_RRC_Paging &msg)
{
    if(start_flag)
    {
        std::string parsed_str = act_parser.set_recv_act_string(ASN_RRC_PCCH_MessageType__c1_PR_paging, PCCH, DL, "");
        std::cout << "parsed : " << parsed_str << std::endl;
        state_manager.receive_string(parsed_str);
    }
}

int Forwarder::handle_rrc_setup(const ASN_RRC_RRCSetup &msg)
{
    if(start_flag)
    {
        std::string parsed_str = act_parser.set_recv_act_string(ASN_RRC_DL_CCCH_MessageType__c1_PR_rrcSetup, CCCH, DL, "");
        std::cout << "parsed : " << parsed_str << std::endl;
        state_manager.receive_string(parsed_str);
    }
}

int Forwarder::handle_rrc_sm_command(const ASN_RRC_SecurityModeCommand &msg)
{
    if(start_flag)
    {
        std::string parsed_str = act_parser.set_recv_act_string(ASN_RRC_DL_DCCH_MessageType__c1_PR_securityModeCommand, DCCH, DL, "");
        std::cout << "parsed : " << parsed_str << std::endl;
        state_manager.receive_string(parsed_str);
    }
}

int Forwarder::handle_rrc_release(const ASN_RRC_RRCRelease &msg)
{
    if(start_flag)
    {
        std::string parsed_str = act_parser.set_recv_act_string(ASN_RRC_DL_DCCH_MessageType__c1_PR_rrcRelease, DCCH, DL, "");
        std::cout << "parsed : " << parsed_str << std::endl;
        state_manager.receive_string(parsed_str);
    }
}


OctetString encodeRrcMsg(OctetString pdu, rrc::RrcChannel channel, uint64_t sti, uint32_t pduId)
{
    rls::RlsPduTransmission msg{sti};

    msg.pduType = rls::EPduType::RRC;
    msg.pdu = std::move(pdu);
    msg.payload = static_cast<uint32_t>(channel);
    msg.pduId = pduId;

    OctetString stream;

    rls::EncodeRlsMessage(msg, stream);

    return std::move(stream);
}


OctetString Forwarder::makeRrcMessage(ASN_RRC_BCCH_BCH_Message *msg, uint64_t sti, uint32_t pduId)
{
    OctetString pdu = rrc::encode::EncodeS(asn_DEF_ASN_RRC_BCCH_BCH_Message, msg);
    if (pdu.length() == 0)
    {
        std::cerr << "PDU Encode Fail" << std::endl;
    }

    return std::move(encodeRrcMsg(std::move(pdu), rrc::RrcChannel::BCCH_BCH, sti, pduId));
}

OctetString Forwarder::makeRrcMessage(ASN_RRC_BCCH_DL_SCH_Message *msg, uint64_t sti, uint32_t pduId)
{
    OctetString pdu = rrc::encode::EncodeS(asn_DEF_ASN_RRC_BCCH_DL_SCH_Message, msg);
    if (pdu.length() == 0)
    {
        std::cerr << "PDU Encode Fail" << std::endl;
    }

    return std::move(encodeRrcMsg(std::move(pdu), rrc::RrcChannel::BCCH_DL_SCH, sti, pduId));
}

OctetString Forwarder::makeRrcMessage(ASN_RRC_DL_CCCH_Message *msg, uint64_t sti, uint32_t pduId)
{
    OctetString pdu = rrc::encode::EncodeS(asn_DEF_ASN_RRC_DL_CCCH_Message, msg);
    if (pdu.length() == 0)
    {
        std::cerr << "PDU Encode Fail" << std::endl;
    }

    return std::move(encodeRrcMsg(std::move(pdu), rrc::RrcChannel::DL_CCCH, sti, pduId));
}

OctetString Forwarder::makeRrcMessage(ASN_RRC_DL_DCCH_Message *msg, uint64_t sti, uint32_t pduId)
{
    OctetString pdu = rrc::encode::EncodeS(asn_DEF_ASN_RRC_DL_DCCH_Message, msg);
    if (pdu.length() == 0)
    {
        std::cerr << "PDU Encode Fail" << std::endl;
    }

    return std::move(encodeRrcMsg(std::move(pdu), rrc::RrcChannel::DL_DCCH, sti, pduId));
}

OctetString Forwarder::makeRrcMessage(ASN_RRC_PCCH_Message *msg, uint64_t sti, uint32_t pduId)
{
    OctetString pdu = rrc::encode::EncodeS(asn_DEF_ASN_RRC_PCCH_Message, msg);
    if (pdu.length() == 0)
    {
        std::cerr << "PDU Encode Fail" << std::endl;
    }

    return std::move(encodeRrcMsg(std::move(pdu), rrc::RrcChannel::PCCH, sti, pduId));

}


OctetString Forwarder::makeRrcMessage(ASN_RRC_UL_CCCH_Message *msg, uint64_t sti, uint32_t pduId)
{
    OctetString pdu = rrc::encode::EncodeS(asn_DEF_ASN_RRC_UL_CCCH_Message, msg);
    if (pdu.length() == 0)
    {
        std::cerr << "PDU Encode Fail" << std::endl;
    }

    return std::move(encodeRrcMsg(std::move(pdu), rrc::RrcChannel::UL_CCCH, sti, pduId));
}

OctetString Forwarder::makeRrcMessage(ASN_RRC_UL_CCCH1_Message *msg, uint64_t sti, uint32_t pduId)
{
    OctetString pdu = rrc::encode::EncodeS(asn_DEF_ASN_RRC_UL_CCCH1_Message, msg);
    if (pdu.length() == 0)
    {
        std::cerr << "PDU Encode Fail" << std::endl;
    }

    return std::move(encodeRrcMsg(std::move(pdu), rrc::RrcChannel::UL_CCCH1, sti, pduId));
}

OctetString Forwarder::makeRrcMessage(ASN_RRC_UL_DCCH_Message *msg, uint64_t sti, uint32_t pduId)
{
    OctetString pdu = rrc::encode::EncodeS(asn_DEF_ASN_RRC_UL_DCCH_Message, msg);
    if (pdu.length() == 0)
    {
        std::cerr << "PDU Encode Fail" << std::endl;
    }

    return std::move(encodeRrcMsg(std::move(pdu), rrc::RrcChannel::UL_DCCH, sti, pduId));

}
