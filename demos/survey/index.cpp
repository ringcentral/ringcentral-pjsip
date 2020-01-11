#include "./index.hpp"
#include "../../credentials.hpp"
#include <iostream>
#include <pjsua2.hpp>

using namespace pj;

AudioMedia *audioMedia;
GreetingsAudioMediaPlayer greetingsPlayer;
QuestionAudioMediaPlayer questionPlayer;
ByeAudioMediaPlayer byePlayer;

bool GreetingsAudioMediaPlayer::onEof()
{
    this->stopTransmit(*audioMedia);
    pj_thread_sleep(2000); // 2 seconds
    questionPlayer.createPlayer("demos/survey/audios/question.wav", PJMEDIA_FILE_NO_LOOP);
    questionPlayer.startTransmit(*audioMedia);
    return false;
}

bool QuestionAudioMediaPlayer::onEof()
{
    this->stopTransmit(*audioMedia);
    return false;
}

bool RedAudioMediaPlayer::onEof()
{
    this->stopTransmit(*audioMedia);
    return false;
}

bool GreenAudioMediaPlayer::onEof()
{
    this->stopTransmit(*audioMedia);
    return false;
}

bool BlueAudioMediaPlayer::onEof()
{
    this->stopTransmit(*audioMedia);
    return false;
}

bool InvalidAudioMediaPlayer::onEof()
{
    this->stopTransmit(*audioMedia);
    return false;
}

bool ByeAudioMediaPlayer::onEof()
{
    this->stopTransmit(*audioMedia);
    return false;
}

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
          greetingsPlayer.createPlayer("demos/survey/audios/greetings.wav", PJMEDIA_FILE_NO_LOOP);
          greetingsPlayer.startTransmit(*audioMedia);
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

  pj_thread_sleep(3000); // 3 seconds
  // Make outbound call
  Call *call = new MyCall(*myAccount);
  CallOpParam prm(true); // Use default call settings
  call->makeCall("sip:" + calleeNumber, prm);

  pj_thread_sleep(36000000); // 10 hours
  delete myAccount;
  return 0;
}
