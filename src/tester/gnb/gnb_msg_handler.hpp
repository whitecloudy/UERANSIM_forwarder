#ifndef __GNB_MSG_HANDLER__
#define __GNB_MSG_HANDLER__

#include <asn/rrc/ASN_RRC_UL-CCCH-MessageType.h>
#include <asn/rrc/ASN_RRC_UL-DCCH-MessageType.h>


int handle_rrc_setup_request(const ASN_RRC_RRCSetupRequest &msg);
int handle_rrc_sm_complete(const ASN_RRC_SecurityModeComplete &msg);
int handle_rrc_resume_request(const ASN_RRC_RRCResumeRequest &msg);
int handle_rrc_setup_complete(const ASN_RRC_RRCSetupComplete &msg);

#endif
