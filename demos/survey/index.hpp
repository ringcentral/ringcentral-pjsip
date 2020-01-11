#ifndef MyAudioMediaPlayer_H
#define MyAudioMediaPlayer_H
#include <pjsua2.hpp>
using namespace pj;
class MyAudioMediaPlayer : public AudioMediaPlayer
{
public:
  virtual bool onEof();
};
#endif