#include "../credentials.hpp"
#include <iostream>
#include <pjsua2.hpp>

using namespace pj;

class MyCall : public Call
{
public:
  AudioMediaPlayer player;
  bool playerCreated = false;

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
        AudioMedia *audioMedia = (AudioMedia *)getMedia(i);
        player.createPlayer("sample.wav", PJMEDIA_FILE_NO_LOOP);
        playerCreated = true;
        player.startTransmit(*audioMedia);
        break;
      }
    }
  }
};

class MyAccount : public Account
{
public:
  virtual void onRegState(OnRegStateParam &params)
  {
    AccountInfo acountInfo = getInfo();
    if(acountInfo.regIsActive) {
      // Make outbound call
      Call *call = new MyCall(*this);
      CallOpParam prm(true); // Use default call settings
      call->makeCall("sip:" + calleeNumber, prm);
    }
  }
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

  pj_thread_sleep(36000000); // 10 hours
  delete myAccount;
  return 0;
}
