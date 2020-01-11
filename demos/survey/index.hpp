#include <pjsua2.hpp>
using namespace pj;

#ifndef GreetingsAudioMediaPlayer_H
#define GreetingsAudioMediaPlayer_H
class GreetingsAudioMediaPlayer : public AudioMediaPlayer
{
public:
  virtual bool onEof();
};
#endif