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

#ifndef RedAudioMediaPlayer_H
#define RedAudioMediaPlayer_H
class RedAudioMediaPlayer : public AudioMediaPlayer
{
public:
  virtual bool onEof();
};
#endif

#ifndef GreenAudioMediaPlayer_H
#define GreenAudioMediaPlayer_H
class GreenAudioMediaPlayer : public AudioMediaPlayer
{
public:
  virtual bool onEof();
};
#endif

#ifndef BlueAudioMediaPlayer_H
#define BlueAudioMediaPlayer_H
class BlueAudioMediaPlayer : public AudioMediaPlayer
{
public:
  virtual bool onEof();
};
#endif

#ifndef InvalidAudioMediaPlayer_H
#define InvalidAudioMediaPlayer_H
class InvalidAudioMediaPlayer : public AudioMediaPlayer
{
public:
  virtual bool onEof();
};
#endif

#ifndef ByeAudioMediaPlayer_H
#define ByeAudioMediaPlayer_H
class ByeAudioMediaPlayer : public AudioMediaPlayer
{
public:
  virtual bool onEof();
};
#endif
