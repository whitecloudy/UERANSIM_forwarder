#ifndef __UE_MSG_HANDLER__
#define __UE_MSG_HANDLER__

#include <asn/rrc/ASN_RRC_PCCH-MessageType.h>
#include <asn/rrc/ASN_RRC_DL-DCCH-MessageType.h>

#include <asn/rrc/ASN_RRC_RRCRelease.h>
#include <asn/rrc/ASN_RRC_RRCSetup.h>
#include <asn/rrc/ASN_RRC_RRCRelease.h>
#include <asn/rrc/ASN_RRC_Paging.h>

int handle_paging_tmsi(const ASN_RRC_Paging &msg);
int handle_rrc_setup(const ASN_RRC_RRCSetup &msg);
int handle_rrc_sm_command(const ASN_RRC_SecurityModeCommand &msg);
int handle_rrc_release(const ASN_RRC_RRCRelease &msg);


#endif
