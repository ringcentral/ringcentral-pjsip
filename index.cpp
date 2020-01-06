#include "credentials.hpp"
#include <iostream>
#include <pjsua2.hpp>

using namespace pj;

class MyCall : public Call
{
public:
  AudioMediaRecorder recorder;

  MyCall(Account &acc, int call_id = PJSUA_INVALID_ID)
      : Call(acc, call_id)
  {
  }

  ~MyCall()
  {
  }

  virtual void onCallState(OnCallStateParam &params)
  {
    std::cout << std::endl
              << "onCallState" << std::endl;
    CallInfo callInfo = getInfo();
    std::cout << std::endl
              << callInfo.stateText << std::endl;
    if (callInfo.stateText == "DISCONNCTD")
    {
      // delete &recorder;
    }
  }

  virtual void onCallMediaState(OnCallMediaStateParam &params)
  {
    std::cout << std::endl
              << "000" << std::endl;
    CallInfo callInfo = getInfo();
    for (unsigned i = 0; i < callInfo.media.size(); i++)
    {
      std::cout << std::endl
                << "111" << std::endl;
      if (callInfo.media[i].type == PJMEDIA_TYPE_AUDIO && getMedia(i))
      {
        std::cout << std::endl
                  << "222" << std::endl;
        AudioMedia *audioMedia = (AudioMedia *)getMedia(i);

        recorder.createRecorder("/ringcentral-pjsip/temp.wav");
        audioMedia->startTransmit(recorder);
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
    std::cout << (acountInfo.regIsActive ? "*** Register:" : "*** Unregister:")
              << " code=" << params.code << " reason=" << params.reason
              << std::endl;

    // Try to make outbound call
    // Call *call = new MyCall(*this);
    // CallOpParam prm(true); // Use default call settings
    // try
    // {
    //   call->makeCall("16508888888", prm);
    // }
    // catch (Error &err)
    // {
    //   std::cout << err.info() << std::endl;
    // }
  }

  virtual void onIncomingCall(OnIncomingCallParam &params)
  {
    Call *call = new MyCall(*this, params.callId);
    CallOpParam callOpParam;
    callOpParam.statusCode = PJSIP_SC_OK;
    call->answer(callOpParam);
    // callOpParam.statusCode = PJSIP_SC_DECLINE;
    // call->hangup(callOpParam);
    // delete call;
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

  AccountConfig accountConfig;
  accountConfig.idUri = "sip:" + userName + "@sip.ringcentral.com";
  accountConfig.regConfig.registrarUri = "sip:sip.ringcentral.com";
  accountConfig.sipConfig.proxies = {"sip:" + outboundProxy};
  accountConfig.sipConfig.authCreds.push_back(
      AuthCredInfo("digest", "*", authorizationId, 0, password));
  MyAccount *myAccount = new MyAccount;
  myAccount->create(accountConfig);

  Endpoint::instance().audDevManager().setNullDev();

  pj_thread_sleep(36000000); // 10 hours
  delete myAccount;
  return 0;
}
