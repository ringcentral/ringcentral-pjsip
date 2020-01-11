#include "./index.hpp"
#include "../../credentials.hpp"
#include <iostream>
#include <pjsua2.hpp>

using namespace pj;

AudioMedia *audioMedia;
AudioMediaPlayer player2;

bool MyAudioMediaPlayer::onEof()
{
    std::cout << std::endl
              << "MyAudioMediaPlayer onEof" << std::endl;
    this->stopTransmit(*audioMedia);
    pj_thread_sleep(2000); // 2 seconds
    player2.createPlayer("demos/survey/audios/question.wav", PJMEDIA_FILE_NO_LOOP);
    player2.startTransmit(*audioMedia);
    return false;
}


MyAudioMediaPlayer player;

class MyCall : public Call
{
public:
  MyCall(Account &acc, int call_id = PJSUA_INVALID_ID)
      : Call(acc, call_id)
  {
  }

  ~MyCall()
  {
  }

  virtual void onCallState(OnCallStateParam &params) {
    CallInfo callInfo = getInfo();
    if(callInfo.stateText == "CONFIRMED") { // call answered
      pj_thread_sleep(2000); // 2 seconds
      for (unsigned i = 0; i < callInfo.media.size(); i++)
      {
        if (callInfo.media[i].type == PJMEDIA_TYPE_AUDIO && getMedia(i))
        {
          audioMedia = (AudioMedia *)getMedia(i);
          player.createPlayer("demos/survey/audios/greetings.wav", PJMEDIA_FILE_NO_LOOP);
          player.startTransmit(*audioMedia);
          break;
        }
      }
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

  pj_thread_sleep(2000); // 2 seconds
  // Make outbound call
  Call *call = new MyCall(*myAccount);
  CallOpParam prm(true); // Use default call settings
  call->makeCall("sip:" + calleeNumber, prm);

  pj_thread_sleep(36000000); // 10 hours
  delete myAccount;
  return 0;
}
