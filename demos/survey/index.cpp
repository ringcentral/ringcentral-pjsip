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

bool GreetingsAudioMediaPlayer::onEof()
{
    this->stopTransmit(*audioMedia);
    pj_thread_sleep(2000); // 2 seconds
    questionPlayer.createPlayer("demos/survey/audios/question.wav");
    questionPlayer.startTransmit(*audioMedia);
    questionPlaying = true;
    return false;
}

bool QuestionAudioMediaPlayer::onEof()
{
    // this->stopTransmit(*audioMedia);
    // return false;
    return true;
}

bool RedAudioMediaPlayer::onEof()
{
    this->stopTransmit(*audioMedia);
    pj_thread_sleep(2000); // 2 seconds
    byePlayer.createPlayer("demos/survey/audios/bye.wav", PJMEDIA_FILE_NO_LOOP);
    byePlayer.startTransmit(*audioMedia);
    return false;
}

bool GreenAudioMediaPlayer::onEof()
{
    this->stopTransmit(*audioMedia);
    pj_thread_sleep(2000); // 2 seconds
    byePlayer.createPlayer("demos/survey/audios/bye.wav", PJMEDIA_FILE_NO_LOOP);
    byePlayer.startTransmit(*audioMedia);
    return false;
}

bool BlueAudioMediaPlayer::onEof()
{
    this->stopTransmit(*audioMedia);
    pj_thread_sleep(2000); // 2 seconds
    byePlayer.createPlayer("demos/survey/audios/bye.wav", PJMEDIA_FILE_NO_LOOP);
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
      redPlayer.createPlayer("demos/survey/audios/red.wav", PJMEDIA_FILE_NO_LOOP);
      redPlayer.startTransmit(*audioMedia);
    }
    else if(params.digit == "2")
    {
      greenPlayer.createPlayer("demos/survey/audios/green.wav", PJMEDIA_FILE_NO_LOOP);
      greenPlayer.startTransmit(*audioMedia);
    }
    else if(params.digit == "3")
    {
      bluePlayer.createPlayer("demos/survey/audios/blue.wav", PJMEDIA_FILE_NO_LOOP);
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

  invalidPlayer.createPlayer("demos/survey/audios/invalid.wav");

  pj_thread_sleep(36000000); // 10 hours
  delete myAccount;
  return 0;
}
