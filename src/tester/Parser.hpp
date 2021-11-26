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
      Action(string act_name) {
        string token[TOKEN_NUM];

        for(int i=0; i<TOKEN_NUM; i++)
        {
          size_t pos = act_name.find("_");

          if(pos == string::npos)
          {
            cout<<"Error in action name: "<<act_name<<endl;
            break;
          }

          if(i == TOKEN_NUM - 1)
            token[i] = act_name;
          else
          {
            token[i] = act_name.substr(0, pos);
            act_name.erase(0, pos + 1);
          }

          cout<<"Parse Success: "<<i<<" "<<token[i]<<std::endl;
        }

        msg_type_id = msg_type_map[token[2]];

        size_t pos = token[2].find("rrc_release");
        if(pos != string::npos)
        {
          if(token[2] == "rrc_release_suspend")
            release_type = SUSPEND;
        }

        if(dcch_ul_set.find(token[2]) != dcch_ul_set.end())
        {
          ch_type = DCCH;
          dir_type = UL;
        }
        else if(dcch_dl_set.find(token[2]) != dcch_dl_set.end())
        {
          ch_type = DCCH;
          dir_type = DL;
        }
        else if(ccch_ul_set.find(token[2]) != ccch_ul_set.end())
        {
          ch_type = CCCH;
          dir_type = UL;
        }
        else if(ccch_dl_set.find(token[2]) != ccch_dl_set.end())
        {
          ch_type = CCCH;
          dir_type = DL;
        }
        else if(pcch_set.find(token[2]) != pcch_set.end())
        {
          ch_type = PCCH;
        }
        else
          cout<<"Error in msg channel & direction type: "<<token[2]<<endl;

        if(token[1] == "BU")
          dst = "UE";
        else if(token[1] == "UB")
          dst = "BS";
        else
          cout<<"Error in destination: "<<token[1]<<std::endl;
      }

      Action(uint32_t id, string d) {
        msg_type_id = id;
        dst = d;
      }

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

  Action* set_inj_act(string act_name);
  string set_recv_act_string(uint32_t id, ChannelType c, DirectionType d, string s);

  void set_inj_msg_data();
  void record_recv_msg_data(string data_name, uint64_t data_value); 

private:
  Action* inj_act;
  map<string, uint64_t> data_recorder;
};
