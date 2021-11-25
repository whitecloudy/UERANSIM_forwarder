#include "Forwarder_Message.hpp"

void receiveMmMessage(const nas::PlainMmMessage& msg)
{
  switch (msg.messageType)
  {
  case nas::EMessageType::REGISTRATION_REQUEST:
	std::cout << "NAS-REGISTRATION_REQUEST" << std::endl;
	break;
  case nas::EMessageType::REGISTRATION_ACCEPT:
    std::cout << "NAS-REGISTRATION_ACCEPT" << std::endl;
    break;
  case nas::EMessageType::REGISTRATION_COMPLETE:
	std::cout << "NAS-REGISTRATION_COMPLETE" << std::endl;
	break;
  case nas::EMessageType::REGISTRATION_REJECT:
    std::cout << "NAS-REGISTRATION_REJECT" << std::endl;
    break;

  case nas::EMessageType::DEREGISTRATION_ACCEPT_UE_ORIGINATING:
    std::cout << "NAS-DEREGISTRATION_ACCEPT_UE_ORIGINATING" << std::endl;
    break;
  case nas::EMessageType::DEREGISTRATION_REQUEST_UE_TERMINATED:
    std::cout << "NAS-DEREGISTRATION_REQUEST_UE_TERMINATED" << std::endl;
    break;
  case nas::EMessageType::DEREGISTRATION_REQUEST_UE_ORIGINATING:
	std::cout << "NAS-DEREGISTRATION_REQUEST_UE_ORIGINATING" << std::endl;
	break;
  case nas::EMessageType::DEREGISTRATION_ACCEPT_UE_TERMINATED:
	std::cout << "NAS-DEREGISTRATION_ACCEPT_UE_TERMINATED" << std::endl;
	break;

  case nas::EMessageType::SERVICE_REJECT:
    std::cout << "NAS-SERVICE_REJECT" << std::endl;
    break;
  case nas::EMessageType::SERVICE_REQUEST:
	std::cout << "NAS-SERVICE_REQUEST" << std::endl;
	break;
  case nas::EMessageType::SERVICE_ACCEPT:
    std::cout << "NAS-SERVICE_ACCEPT" << std::endl;
    break;

  case nas::EMessageType::CONFIGURATION_UPDATE_COMMAND:
    std::cout << "NAS-CONFIGURATION_UPDATE_COMMAND" << std::endl;
    break;
  case nas::EMessageType::CONFIGURATION_UPDATE_COMPLETE:
	std::cout << "NAS-CONFIGURATION_UPDATE_COMPLETE" << std::endl;
	break;

  case nas::EMessageType::AUTHENTICATION_REQUEST:
    std::cout << "NAS-AUTHENTICATION_REQUEST" << std::endl;
    break;
  case nas::EMessageType::AUTHENTICATION_REJECT:
    std::cout << "NAS-AUTHENTICATION_REJECT" << std::endl;
    break;
  case nas::EMessageType::AUTHENTICATION_RESPONSE:
	std::cout << "NAS-AUTHENTICATION_RESPONSE" << std::endl;
	break;
  case nas::EMessageType::AUTHENTICATION_RESULT:
    std::cout << "NAS-AUTHENTICATION_RESULT" << std::endl;
    break;
  case nas::EMessageType::AUTHENTICATION_FAILURE:
    std::cout << "NAS-AUTHENTICATION_FAILURE " << std::endl;
    break;

  case nas::EMessageType::IDENTITY_REQUEST:
    std::cout << "NAS-IDENTITY_REQUEST" << std::endl;
    break;
  case nas::EMessageType::IDENTITY_RESPONSE:
	std::cout << "NAS-IDENTITY_RESPONSE" << std::endl;
	break;
  case nas::EMessageType::SECURITY_MODE_COMMAND:
    std::cout << "NAS-SECURITY_MODE_COMMAND" << std::endl;
    break;
  case nas::EMessageType::SECURITY_MODE_COMPLETE:
	std::cout << "NAS-SECURITY_MODE_COMPLETE" << std::endl;
	break;
  case nas::EMessageType::SECURITY_MODE_REJECT:
	std::cout << "NAS-SECURITY_MODE_REJECT" << std::endl;
	break;

  case nas::EMessageType::FIVEG_MM_STATUS:
    std::cout << "NAS-FIVEG_MM_STATUS" << std::endl;
    break;
  case nas::EMessageType::DL_NAS_TRANSPORT:
    std::cout << "NAS-DL_NAS_TRANSPORT" << std::endl;
    break;
  case nas::EMessageType::UL_NAS_TRANSPORT:
	std::cout << "NAS-UL_NAS_TRANSPORT" << std::endl;
	break;
  default:
    std::cout << "Unhandled NAS MM message received" << std::endl;
    break;
  }
}


static ASN_RRC_UL_CCCH_Message *ConstructSetupRequest(ASN_RRC_InitialUE_Identity_t initialUeId,
                                                      ASN_RRC_EstablishmentCause_t establishmentCause)
{
    auto *pdu = asn::New<ASN_RRC_UL_CCCH_Message>();
    pdu->message.present = ASN_RRC_UL_CCCH_MessageType_PR_c1;
    pdu->message.choice.c1 = asn::NewFor(pdu->message.choice.c1);
    pdu->message.choice.c1->present = ASN_RRC_UL_CCCH_MessageType__c1_PR_rrcSetupRequest;

    auto &r = pdu->message.choice.c1->choice.rrcSetupRequest = asn::New<ASN_RRC_RRCSetupRequest>();
    asn::DeepCopy(asn_DEF_ASN_RRC_InitialUE_Identity, initialUeId, &r->rrcSetupRequest.ue_Identity);
    r->rrcSetupRequest.establishmentCause = establishmentCause;
    asn::SetSpareBits<1>(r->rrcSetupRequest.spare);

    return pdu;
}


void sendRrcMessage(int cellId, ASN_RRC_UL_CCCH_Message *msg, Socket send_sock, InetAddress recv_addr)
{
    OctetString pdu = rrc::encode::EncodeS(asn_DEF_ASN_RRC_UL_CCCH_Message, msg);
    if (pdu.length() == 0)
    {
        std::cout<<"RRC UL-DCCH encoding failed."<< std::endl;
        return;
    }

    auto *m = new NmUeRrcToRls(NmUeRrcToRls::RRC_PDU_DELIVERY);
    m->cellId = cellId;
    m->channel = rrc::RrcChannel::UL_CCCH;
    m->pdu = std::move(pdu);

    /*

    auto *new_m = new NmUeRlsToRls(NmUeRlsToRls::UPLINK_RRC);
    new_m->cellId = m->cellId;
    new_m->rrcChannel = m->channel;
    new_m->pduId = 0;
    new_m->data = std::move(m->pdu);
    */

    //
    //rls::RlsPduTransmission new_msg{m_shCtx->sti};
    rls::RlsPduTransmission new_msg{utils::Random64()};
    new_msg.pduType = rls::EPduType::RRC;
    new_msg.pdu = std::move(m->pdu);
    new_msg.payload = static_cast<uint32_t>(m->channel);
    new_msg.pduId = 0;

    OctetString stream;
    rls::EncodeRlsMessage(new_msg, stream);
    printf("hihi\n");
    send_sock.send(recv_addr,stream.data(),sizeof(stream.data()) );


}

void startConnectionEstablishment(ASN_RRC_InitialUE_Identity_t m_initialId, ASN_RRC_EstablishmentCause m_establishmentCause, Socket send_sock, InetAddress recv_addr)
{
    int activeCell = 1;
    printf("activecell: %d\n",activeCell);

    if (m_initialId.present == ASN_RRC_InitialUE_Identity_PR_NOTHING)
    {
        printf("dfdsf\n");
        m_initialId.present = ASN_RRC_InitialUE_Identity_PR_randomValue;
        asn::SetBitStringLong<39>(static_cast<int64_t>(utils::Random64()), m_initialId.choice.randomValue);
    }
    //
    printf("num: %10x\n",m_initialId.choice.randomValue);

    std::cout<< "Sending RRC Setup Request" <<std::endl;

    auto *rrcSetupRequest =
        ConstructSetupRequest(m_initialId, static_cast<ASN_RRC_EstablishmentCause_t>(m_establishmentCause));
    sendRrcMessage(activeCell, rrcSetupRequest, send_sock, recv_addr);
    asn::Free(asn_DEF_ASN_RRC_UL_CCCH_Message, rrcSetupRequest);
}


