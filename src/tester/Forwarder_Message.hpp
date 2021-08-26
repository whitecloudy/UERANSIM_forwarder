#include <iostream>
#include <string>
#include <vector>
#include <lib/app/cli_cmd.hpp>
#include <memory>
#include <utils/nts.hpp>

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

#include "Forwarder.hpp"

void receiveMmMessage(const nas::PlainMmMessage& msg);
static ASN_RRC_UL_CCCH_Message *ConstructSetupRequest(ASN_RRC_InitialUE_Identity_t initialUeId, ASN_RRC_EstablishmentCause_t establishmentCause);
void sendRrcMessage(int cellId, ASN_RRC_UL_CCCH_Message *msg, Socket send_sock, InetAddress recv_addr);
void startConnectionEstablishment(ASN_RRC_InitialUE_Identity_t m_initialId, ASN_RRC_EstablishmentCause m_establishmentCause, Socket send_sock, InetAddress recv_addr);