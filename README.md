# ringcentral-pjsip

Use [PJSIP](https://www.pjsip.org/) to create RingCentral device simulators.

Technical Reference: [slides](https://docs.google.com/presentation/d/15KvnbZVB_adSN6xjwlRHh-dsc3HWeVrAkxvBRSLVGPc/edit?usp=sharing).


## Setup

```
docker build -t ringcentral-pjsip:latest .
cp credentials.sample.hpp credentials.hpp
```

Edit `credentials.hpp` and specify credentials.


## Run

```
docker run -it --rm -v $(pwd):/ringcentral-pjsip ringcentral-pjsip ./run.sh <file-name>.cpp
```

For example:

```
docker run -it --rm -v $(pwd):/ringcentral-pjsip ringcentral-pjsip ./run.sh demos/call-and-play-wav-file.cpp
```


## Test

Call the phone number associated with your RingCentral device.


## Todo

- How to deploy to AWS?
- Add demo about microphone and speaker
- high concurrence
    - one application to handle handreds of calls


## Miscellaneos

### How to gererate wav file for testing

```
say -o temp.wav --data-format=LEI16@48000 "Hello world"
```

Command above convert text "Hello world" to `temp.wav` file.

**BUT** this file doesn't have proper headers.

In order to fix it, we need ffmpeg:

```
ffmpeg -i temp.wav hello.wav
```


## Dev notes

How to run a docker bash:

```
docker run --rm -it ringcentral-pjsip:latest /bin/bash
```