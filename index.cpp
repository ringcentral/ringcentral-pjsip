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
  // init
  Endpoint endpoint;
  endpoint.libCreate();
  EpConfig epConfig;
  endpoint.libInit(epConfig);

  // Create transport
  TransportConfig transportConfig;
  transportConfig.port = 5060;
  endpoint.transportCreate(PJSIP_TRANSPORT_UDP, transportConfig);

  // Start the library
  endpoint.libStart();

  // Configure an AccountConfig
  AccountConfig accountConfig;
  accountConfig.idUri = "sip:" + userName + "@sip.ringcentral.com";
  accountConfig.regConfig.registrarUri = "sip:sip.ringcentral.com";
  accountConfig.sipConfig.proxies = {"sip:" + outboundProxy};
  AuthCredInfo authCredInfo("digest", "*", authorizationId, 0, password);
  accountConfig.sipConfig.authCreds.push_back(authCredInfo);

  // Create the account
  MyAccount *myAccount = new MyAccount;
  myAccount->create(accountConfig);

  // Wait for an hour
  pj_thread_sleep(36000000);

  // Delete the account. This will unregister from server
  delete myAccount;
  // This will implicitly shutdown the library
  return 0;
}
