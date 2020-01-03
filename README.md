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
docker run -it --rm -v $(pwd):/ringcentral-pjsip ringcentral-pjsip
```


## Test

Call the phone number associated with your RingCentral device.
