#include <string>
#include <map>
#include <iostream>
#include <set>

#include <asn/rrc/ASN_RRC_UL-CCCH-Message.h>
#include <asn/rrc/ASN_RRC_UL-DCCH-Message.h>
#include <asn/rrc/ASN_RRC_BCCH-BCH-Message.h>
#include <asn/rrc/ASN_RRC_BCCH-DL-SCH-Message.h>
#include <asn/rrc/ASN_RRC_DL-CCCH-Message.h>
#include <asn/rrc/ASN_RRC_DL-DCCH-Message.h>
#include <asn/rrc/ASN_RRC_PCCH-Message.h>

#define TOKEN_NUM 3

using namespace std;

enum ReleaseType {SUSPEND};
enum ChannelType {DCCH, CCCH, PCCH};
enum DirectionType {UL, DL};

static set<string> dcch_ul_set = {"rrc_sm_complete", "rrc_setup_complete"};
static set<string> dcch_dl_set = {"rrc_release_suspend", "rrc_release", "rrc_sm_command"};
static set<string> ccch_ul_set = {"rrc_resume_req"};
static set<string> ccch_dl_set = {"rrc_setup"};
static set<string> pcch_set = {"paging_tmsi"};


static map<string, uint32_t> msg_type_map = {
  {"rrc_sm_complete", ASN_RRC_UL_DCCH_MessageType__c1_PR_securityModeComplete},
  {"rrc_release_suspend", ASN_RRC_DL_DCCH_MessageType__c1_PR_rrcRelease}, 
  {"rrc_release", ASN_RRC_DL_DCCH_MessageType__c1_PR_rrcRelease}, 
  {"rrc_setup", ASN_RRC_DL_CCCH_MessageType__c1_PR_rrcSetup},
  {"rrc_sm_command", ASN_RRC_DL_DCCH_MessageType__c1_PR_securityModeCommand},
  {"rrc_setup_complete", ASN_RRC_UL_DCCH_MessageType__c1_PR_rrcSetupComplete},
  {"rrc_resume_req", ASN_RRC_UL_CCCH_MessageType__c1_PR_rrcResumeRequest},
  {"pagig_tmsi", ASN_RRC_PCCH_MessageType__c1_PR_paging}
}; 
  
class Parser {

  class Action {

    public:
      Action(string act_name);
      ~Action() {
        msg_data.clear();
      }

    public:
      ChannelType ch_type;
      DirectionType dir_type;
      uint32_t msg_type_id;
      ReleaseType release_type;
      string dst;
      map<string, uint64_t> msg_data;
  };

public:
  ~Parser() {
    delete inj_act;
  }

  void set_inj_act(string act_name);
  void set_inj_msg_data();
  
  ChannelType get_ch_type() {
    return inj_act->ch_type;
  }
  DirectionType get_dir_type(){
    return inj_act->dir_type;
  }
  uint32_t get_msg_type_id()
  {
    return inj_act->msg_type_id;
  }
  ReleaseType get_release_type()
  {
    return inj_act->release_type;
  }
  string get_dst()
  {
    return inj_act->dst;
  }
  map<string, uint64_t>* get_msg_data_map()
  {
    return &inj_act->msg_data;
  }

  void record_recv_msg_data(string data_name, uint64_t data_value); 
  string set_recv_act_string(uint32_t id, ChannelType c, DirectionType d, string s);

private:
  Action* inj_act;
  map<string, uint64_t> data_recorder;
};