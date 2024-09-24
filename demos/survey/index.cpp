#include "./index.hpp"
#include "../../credentials.hpp"
#include <iostream>
#include <pjsua2.hpp>

using namespace pj;

AudioMedia *audioMedia;
GreetingsAudioMediaPlayer greetingsPlayer;
QuestionAudioMediaPlayer questionPlayer;
RedAudioMediaPlayer redPlayer;
GreenAudioMediaPlayer greenPlayer;
BlueAudioMediaPlayer bluePlayer;
InvalidAudioMediaPlayer invalidPlayer;
ByeAudioMediaPlayer byePlayer;
bool questionPlaying = false;
Call *call;

bool GreetingsAudioMediaPlayer::onEof()
{
    this->stopTransmit(*audioMedia);
    pj_thread_sleep(2000); // 2 seconds
    questionPlayer.startTransmit(*audioMedia);
    questionPlaying = true;
    return false;
}

bool QuestionAudioMediaPlayer::onEof()
{
    return true;
}

bool RedAudioMediaPlayer::onEof()
{
    this->stopTransmit(*audioMedia);
    pj_thread_sleep(2000); // 2 seconds
    byePlayer.startTransmit(*audioMedia);
    return false;
}

bool GreenAudioMediaPlayer::onEof()
{
    this->stopTransmit(*audioMedia);
    pj_thread_sleep(2000); // 2 seconds
    byePlayer.startTransmit(*audioMedia);
    return false;
}

bool BlueAudioMediaPlayer::onEof()
{
    this->stopTransmit(*audioMedia);
    pj_thread_sleep(2000); // 2 seconds
    byePlayer.startTransmit(*audioMedia);
    return false;
}

bool InvalidAudioMediaPlayer::onEof()
{
    this->stopTransmit(*audioMedia);
    questionPlayer.setPos(0);
    questionPlayer.startTransmit(*audioMedia);
    questionPlaying = true;
    return true;
}

bool ByeAudioMediaPlayer::onEof()
{
    this->stopTransmit(*audioMedia);
    pj_thread_sleep(2000); // 2 seconds
    CallOpParam callOpParam;
    call->hangup(callOpParam);
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
          greetingsPlayer.startTransmit(*audioMedia);
          break;
        }
      }
    }
  }

  virtual void onDtmfDigit(OnDtmfDigitParam &params)
  {
    std::cout << std::endl
              << params.digit << std::endl;
    if(!questionPlaying) 
    {
      return;
    }
    questionPlayer.stopTransmit(*audioMedia);
    questionPlaying = false;
    pj_thread_sleep(2000); // 2 seconds
    if(params.digit == "1")
    {
      redPlayer.startTransmit(*audioMedia);
    }
    else if(params.digit == "2")
    {
      greenPlayer.startTransmit(*audioMedia);
    }
    else if(params.digit == "3")
    {
      bluePlayer.startTransmit(*audioMedia);
    } else {
      invalidPlayer.startTransmit(*audioMedia);
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
  endpoint.transportCreate(PJSIP_TRANSPORT_TCP, transportConfig);
  endpoint.libStart();

  Endpoint::instance().audDevManager().setNullDev();

  AccountConfig accountConfig;
  accountConfig.idUri = "sip:" + userName + "@sip.ringcentral.com";
  accountConfig.regConfig.registrarUri = "sip:sip.ringcentral.com";
  accountConfig.sipConfig.proxies = {"sip:" + outboundProxy + ";transport=tcp"};
  accountConfig.sipConfig.authCreds.push_back(
      AuthCredInfo("digest", "*", authorizationId, 0, password));
  MyAccount *myAccount = new MyAccount;
  myAccount->create(accountConfig);

  pj_thread_sleep(5000); // 5 seconds
  // Make outbound call
  call = new MyCall(*myAccount);
  CallOpParam prm(true); // Use default call settings
  call->makeCall("sip:" + calleeNumber, prm);

  invalidPlayer.createPlayer("demos/survey/audios/invalid.wav");
  questionPlayer.createPlayer("demos/survey/audios/question.wav");
  byePlayer.createPlayer("demos/survey/audios/bye.wav");
  greetingsPlayer.createPlayer("demos/survey/audios/greetings.wav");
  redPlayer.createPlayer("demos/survey/audios/red.wav");
  greenPlayer.createPlayer("demos/survey/audios/green.wav");
  bluePlayer.createPlayer("demos/survey/audios/blue.wav");

  pj_thread_sleep(36000000); // 10 hours
  delete myAccount;
  return 0;
}
