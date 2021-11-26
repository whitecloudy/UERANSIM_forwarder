#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <cassert>
#include <cstdio>
#include <tester/Forwarder.hpp>
#include <tester/Parser.hpp>

const std::string IP_ADDR = "127.8.8.8";
const uint16_t PORT       = 4997;

const std::string GNB_ADDR = "127.0.0.1";
const uint16_t GNB_PORT    = 4997;

const unsigned int BUFSIZE = 16777216;


int main(int argc, char ** argv)
{
  Parser p;
  p.set_inj_act("adv_BU_rrc_release_suspend");
  /*
    Forwarder forwarder(GNB_ADDR, GNB_PORT, IP_ADDR, PORT);

    forwarder.do_work();
    */
    return 0;
}
