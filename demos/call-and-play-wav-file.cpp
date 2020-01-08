#include "../credentials.hpp"
#include <iostream>
#include <pjsua2.hpp>

using namespace pj;

class MyCall : public Call
{
public:
  AudioMediaPlayer player;
  bool playerCreated = false;
  AudioMedia *audioMedia;

  MyCall(Account &acc, int call_id = PJSUA_INVALID_ID)
      : Call(acc, call_id)
  {
  }

  ~MyCall()
  {
  }

  virtual void onCallMediaState(OnCallMediaStateParam &params)
  {
    if(playerCreated) {
      return;
    }
    CallInfo callInfo = getInfo();
    for (unsigned i = 0; i < callInfo.media.size(); i++)
    {
      if (callInfo.media[i].type == PJMEDIA_TYPE_AUDIO && getMedia(i))
      {
        audioMedia = (AudioMedia *)getMedia(i);
        player.createPlayer("demos/survey/question.wav", PJMEDIA_FILE_NO_LOOP);
        playerCreated = true;
        break;
      }
    }
  }

  virtual void onCallState(OnCallStateParam &params) {
    CallInfo callInfo = getInfo();
    if(callInfo.stateText == "CONFIRMED") {
      pj_thread_sleep(3000); // 3 seconds
      player.startTransmit(*audioMedia);
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
  endpoint.transportCreate(PJSIP_TRANSPORT_UDP, transportConfig);
  endpoint.libStart();

  Endpoint::instance().audDevManager().setNullDev();

  AccountConfig accountConfig;
  accountConfig.idUri = "sip:" + userName + "@sip.ringcentral.com";
  accountConfig.regConfig.registrarUri = "sip:sip.ringcentral.com";
  accountConfig.sipConfig.proxies = {"sip:" + outboundProxy};
  accountConfig.sipConfig.authCreds.push_back(
      AuthCredInfo("digest", "*", authorizationId, 0, password));
  MyAccount *myAccount = new MyAccount;
  myAccount->create(accountConfig);

  pj_thread_sleep(3000); // 3 seconds
  // Make outbound call
  Call *call = new MyCall(*myAccount);
  CallOpParam prm(true); // Use default call settings
  call->makeCall("sip:" + calleeNumber, prm);

  pj_thread_sleep(36000000); // 10 hours
  delete myAccount;
  return 0;
}
