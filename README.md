# ringcentral-pjsip

Use [PJSIP](https://www.pjsip.org/) to create RingCentral device simulators.


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
