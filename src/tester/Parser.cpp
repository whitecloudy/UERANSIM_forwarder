#include "Parser.hpp"


Parser::Action*
Parser::set_inj_act(string act_name)
{
  inj_act = new Parser::Action(act_name);
  return inj_act;
}

string
Parser::set_recv_act_string(uint32_t id, ChannelType c, DirectionType d, string s)
{
  string ret;
  ret = s;

  if(c == DCCH && d == UL)
  {
    switch(id)
    {
      case ASN_RRC_UL_DCCH_MessageType__c1_PR_securityModeComplete:
        ret += "rrc_sm_complete";
        break;
      case ASN_RRC_UL_DCCH_MessageType__c1_PR_rrcSetupComplete:
        ret += "rrc_setup_complete";
        break;
      default:
          cout<<"Error not defined UL_DCCH Message type ID: "<<id<<endl;
    }
  }
  else if(c == DCCH && d == DL)
  {
    switch(id)
    {
      case ASN_RRC_DL_DCCH_MessageType__c1_PR_rrcRelease:
        ret += "rrc_release";
        break;
      case ASN_RRC_DL_DCCH_MessageType__c1_PR_securityModeCommand:
        ret += "rrc_sm_command";
        break;
      default:
          cout<<"Error not defined DL_DCCH Message type ID: "<<id<<endl;
    }
  }
  else if(c == CCCH && d == UL)
  {
    switch(id)
    {
      case ASN_RRC_UL_CCCH_MessageType__c1_PR_rrcResumeRequest:
        ret += "rrc_resume_req";
        break;
      default:
          cout<<"Error not defined UL_CCCH Message type ID: "<<id<<endl;
    }
  }
  else if(c == CCCH && d == DL)
  {
    switch(id)
    {
      case ASN_RRC_DL_CCCH_MessageType__c1_PR_rrcSetup:
        ret += "rrc_setup";
        break;
      default:
          cout<<"Error not defined DL_CCCH Message type ID: "<<id<<endl;
    }
  }
  else if(c == PCCH)
  {
    switch(id)
    {
      case ASN_RRC_PCCH_MessageType__c1_PR_paging:
        ret += "paging_tmsi";
        break;
    }
  }
  else
    cout<<"Error in msg channel & direction type: "<<c<<" " <<d<<endl;

  return ret;
}

void
Parser::set_inj_msg_data()
{
  if(inj_act->ch_type == DCCH && inj_act->dir_type == DL && inj_act->msg_type_id == ASN_RRC_DL_DCCH_MessageType__c1_PR_securityModeCommand)
    inj_act->msg_data["security_mode"] = 0;

  if(data_recorder.find("tmsi") != data_recorder.end())
    inj_act->msg_data["tmsi"] = data_recorder["tmsi"];

}

void
Parser::record_recv_msg_data(string data_name, uint64_t data_value)
{
  data_recorder[data_name] = data_value;
}
