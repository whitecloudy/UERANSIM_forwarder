#include "Forwarder.hpp"

#include <string>
#include <iostream>


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

    std::string parsed_str = act_parser.set_recv_act_string(ASN_RRC_DL_CCCH_MessageType__c1_PR_rrcSetup, CCCH, UL, "");
    std::cout << "parsed : " << parsed_str << std::endl;
    state_manager.receive_string(parsed_str);
}

int Forwarder::handle_rrc_sm_complete(const ASN_RRC_SecurityModeComplete &msg)
{
    std::string parsed_str = act_parser.set_recv_act_string(ASN_RRC_UL_DCCH_MessageType__c1_PR_securityModeComplete, DCCH, UL, "");
    std::cout << "parsed : " << parsed_str << std::endl;
    state_manager.receive_string(parsed_str);
}

int Forwarder::handle_rrc_resume_request(const ASN_RRC_RRCResumeRequest &msg)
{
    std::string parsed_str = act_parser.set_recv_act_string(ASN_RRC_UL_CCCH_MessageType__c1_PR_rrcResumeRequest, CCCH, UL, "");
    std::cout << "parsed : " << parsed_str << std::endl;
    state_manager.receive_string(parsed_str);

}

int Forwarder::handle_rrc_setup_complete(const ASN_RRC_RRCSetupComplete &msg)
{
    std::string parsed_str = act_parser.set_recv_act_string(ASN_RRC_UL_DCCH_MessageType__c1_PR_rrcSetupComplete, DCCH, UL, "");
    std::cout << "parsed : " << parsed_str << std::endl;
    state_manager.receive_string(parsed_str);
}

int Forwarder::handle_paging_tmsi(const ASN_RRC_Paging &msg)
{
    std::string parsed_str = act_parser.set_recv_act_string(ASN_RRC_PCCH_MessageType__c1_PR_paging, PCCH, DL, "");
    std::cout << "parsed : " << parsed_str << std::endl;
    state_manager.receive_string(parsed_str);
}

int Forwarder::handle_rrc_setup(const ASN_RRC_RRCSetup &msg)
{
    std::string parsed_str = act_parser.set_recv_act_string(ASN_RRC_DL_CCCH_MessageType__c1_PR_rrcSetup, CCCH, DL, "");
    std::cout << "parsed : " << parsed_str << std::endl;
    state_manager.receive_string(parsed_str);
}

int Forwarder::handle_rrc_sm_command(const ASN_RRC_SecurityModeCommand &msg)
{
    std::string parsed_str = act_parser.set_recv_act_string(ASN_RRC_DL_DCCH_MessageType__c1_PR_securityModeCommand, DCCH, DL, "");
    std::cout << "parsed : " << parsed_str << std::endl;
    state_manager.receive_string(parsed_str);
}

int Forwarder::handle_rrc_release(const ASN_RRC_RRCRelease &msg)
{
    std::string parsed_str = act_parser.set_recv_act_string(ASN_RRC_DL_DCCH_MessageType__c1_PR_rrcRelease, DCCH, DL, "");
    std::cout << "parsed : " << parsed_str << std::endl;
    state_manager.receive_string(parsed_str);
}

