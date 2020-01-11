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

#ifndef QuestionAudioMediaPlayer_H
#define QuestionAudioMediaPlayer_H
class QuestionAudioMediaPlayer : public AudioMediaPlayer
{
public:
  virtual bool onEof();
};
#endif

#ifndef ByeAudioMediaPlayer_H
#define ByeAudioMediaPlayer_H
class ByeAudioMediaPlayer : public AudioMediaPlayer
{
// public:
//   virtual bool onEof();
};
#endif
