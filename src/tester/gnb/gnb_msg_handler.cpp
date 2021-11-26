#include "gnb_msg_handler.hpp"
#include <asn/rrc/ASN_RRC_RRCSetupRequest.h>
#include <asn/rrc/ASN_RRC_SecurityModeComplete.h>
#include <asn/rrc/ASN_RRC_RRCResumeRequest.h>
#include <asn/rrc/ASN_RRC_RRCSetupComplete.h>
#include <asn/rrc/ASN_RRC_InitialUE-Identity.h>
#include <lib/asn/utils.hpp>


int handle_rrc_setup_request(const ASN_RRC_RRCSetupRequest &msg)
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
}

int handle_rrc_sm_complete(const ASN_RRC_SecurityModeComplete &msg)
{

}

int handle_rrc_resume_request(const ASN_RRC_RRCResumeRequest &msg)
{

}

int handle_rrc_setup_complete(const ASN_RRC_RRCSetupComplete &msg)
{

}
