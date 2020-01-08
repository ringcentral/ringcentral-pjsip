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
docker run -it --rm -v $(pwd):/ringcentral-pjsip ringcentral-pjsip ./run.sh demos/answer-and-save-to-wav-file.cpp
```


## Test

Call the phone number associated with your RingCentral device.


## Similar projects

- [andrius/pjsua](https://github.com/andrius/pjsua)


## Todo

- How to deploy to AWS?
- Do a telephony survey demo, like the one I created using IVR scripting API


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
