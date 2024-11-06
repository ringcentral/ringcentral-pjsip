#include "../credentials.hpp"
#include <iostream>
#include <pjsua2.hpp>

using namespace pj;

class MyCall : public Call
{
public:
  AudioMediaPlayer player;

  MyCall(Account &acc, int call_id = PJSUA_INVALID_ID)
      : Call(acc, call_id)
  {
  }

  ~MyCall()
  {
  }

  virtual void onCallState(OnCallStateParam &params) {
    CallInfo callInfo = getInfo();
    cout << "******" << callInfo.stateText << "******" << "\n";
    if(callInfo.stateText == "CONFIRMED") { // call answered
      pj_thread_sleep(2000); // 2 seconds
      for (unsigned i = 0; i < callInfo.media.size(); i++)
      {
        if (callInfo.media[i].type == PJMEDIA_TYPE_AUDIO && getMedia(i))
        {
          AudioMedia *audioMedia = (AudioMedia *)getMedia(i);
          player.createPlayer("demos/survey/audios/question.wav", PJMEDIA_FILE_NO_LOOP);
          player.startTransmit(*audioMedia);
          break;
        }
      }
    } else if (callInfo.stateText == "DISCONNCTD") {
      exit(0);
    }
  }
};

class MyAccount : public Account
{
};

int main()
{
  Endpoint endpoint;
  endpoint.libCreate();
  EpConfig epConfig;
  endpoint.libInit(epConfig);
  TransportConfig transportConfig;
  transportConfig.port = 5060;
  endpoint.transportCreate(PJSIP_TRANSPORT_TLS, transportConfig);
  endpoint.libStart();

  Endpoint::instance().audDevManager().setNullDev();

  AccountConfig accountConfig;
  accountConfig.mediaConfig.srtpUse = PJMEDIA_SRTP_MANDATORY;
  accountConfig.idUri = "sip:" + userName + "@sip.ringcentral.com";
  accountConfig.regConfig.registrarUri = "sip:sip.ringcentral.com";
  accountConfig.sipConfig.proxies = {"sip:" + outboundProxy + ";transport=tls"};
  accountConfig.sipConfig.authCreds.push_back(
      AuthCredInfo("digest", "*", authorizationId, 0, password));
  MyAccount *myAccount = new MyAccount;
  myAccount->create(accountConfig);

  pj_thread_sleep(2000); // 2 seconds
  // Make outbound call
  Call *call = new MyCall(*myAccount);
  CallOpParam prm(true); // Use default call settings

  // add a customer header
  // header added, but caller id not working
  SipHeader customHeader;
  customHeader.hName = "P-Asserted-Identity";
  customHeader.hValue = "sip:12099213248@sip.ringcentral.com";
  prm.txOption.headers.push_back(customHeader);

  call->makeCall("sip:" + calleeNumber, prm);

  pj_thread_sleep(20000); // 20 seconds
  delete myAccount;
  return 0;
}
