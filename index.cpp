#include "credentials.hpp"
#include <iostream>
#include <pjsua2.hpp>

using namespace pj;

class MyAccount : public Account {
public:
  virtual void onRegState(OnRegStateParam &params) {
    AccountInfo acountInfo = getInfo();
    std::cout << (acountInfo.regIsActive ? "*** Register:" : "*** Unregister:")
              << " code=" << params.code << std::endl;
  }

  virtual void onIncomingCall(OnIncomingCallParam &params) {
    std::cout << "********" << std::endl
              << "Incoming call" << std::endl
              << "********" << std::endl;
  }
};

int main() {
  // endpoint
  Endpoint endpoint;
  endpoint.libCreate();
  EpConfig epConfig;
  endpoint.libInit(epConfig);
  TransportConfig transportConfig;
  transportConfig.port = 5060;
  endpoint.transportCreate(PJSIP_TRANSPORT_UDP, transportConfig);
  endpoint.libStart();

  // account
  AccountConfig accountConfig;
  accountConfig.idUri = "sip:" + userName + "@sip.ringcentral.com";
  accountConfig.regConfig.registrarUri = "sip:sip.ringcentral.com";
  accountConfig.sipConfig.proxies = {"sip:" + outboundProxy};
  AuthCredInfo authCredInfo("digest", "*", authorizationId, 0, password);
  accountConfig.sipConfig.authCreds.push_back(authCredInfo);
  MyAccount *myAccount = new MyAccount;
  myAccount->create(accountConfig);

  // dispose
  pj_thread_sleep(36000000);
  delete myAccount;
  return 0;
}
