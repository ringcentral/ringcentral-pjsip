#include "credentials.hpp"
#include <iostream>
#include <pjsua2.hpp>

using namespace pj;

class MyAccount : public Account {
public:
  virtual void onRegState(OnRegStateParam &params) {
    AccountInfo acountInfo = getInfo();
    std::cout << (acountInfo.regIsActive ? "*** Register:" : "*** Unregister:")
              << " code=" << params.code << " reason=" << params.reason
              << std::endl;
  }

  virtual void onIncomingCall(OnIncomingCallParam &params) {
    Call *call = new Call(*this, params.callId);
    CallOpParam callOpParam;
    callOpParam.statusCode = PJSIP_SC_DECLINE;
    call->hangup(callOpParam);
    delete call;
  }
};

int main() {
  Endpoint endpoint;
  endpoint.libCreate();
  EpConfig epConfig;
  endpoint.libInit(epConfig);
  TransportConfig transportConfig;
  transportConfig.port = 5060;
  endpoint.transportCreate(PJSIP_TRANSPORT_UDP, transportConfig);
  endpoint.libStart();

  AccountConfig accountConfig;
  accountConfig.idUri = "sip:" + userName + "@sip.ringcentral.com";
  accountConfig.regConfig.registrarUri = "sip:sip.ringcentral.com";
  accountConfig.sipConfig.proxies = {"sip:" + outboundProxy};
  accountConfig.sipConfig.authCreds.push_back(
      AuthCredInfo("digest", "*", authorizationId, 0, password));
  MyAccount *myAccount = new MyAccount;
  myAccount->create(accountConfig);

  pj_thread_sleep(36000000);
  delete myAccount;
  return 0;
}
