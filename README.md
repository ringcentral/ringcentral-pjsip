# ringcentral-pjsip

Use [PJSIP](https://www.pjsip.org/) to create RingCentral device simulators.


## Setup

```
docker build -t ringcentral-pjsip:latest .
```

## Run

```
docker run -it --rm -v $(pwd):/src/cpp/ringcentral-pjsip ringcentral-pjsip
```
