# Airplay Service hỗ trợ photo và video thông qua giao thức http

## Install: 

`cd src`

`make`

## Yêu cầu:

* avahi

` sudo apt-get install avahi-daemon ` 

` sudo apt-get install avahi-utils ` 

## Chạy service

`./src/server`

### Server Airplay chạy ở cổng 7000

### Server Screen Mirror chạy ở cổng 7100

#### Các hàm tương ứng với mỗi request từ client được implement trong file callback.cpp  

 - void post_play(const Request &rq, http_response_t *res):

```
Nhận từ client:
params_map["Content-Location"], content_location: Địa chỉ video
params_map["Start-Position"], start_position: vị trí bắt đầu chạy (s)
```

- void post_scrub(const char *argument, http_response_t *res):

```
Nhận từ client:
params_map["position"]: vị trí client seek đến (s)
```

- void get_scrub(http_response_t *res):

```
Gửi đến client:
duration: Độ dài video
position: Thời điểm video đang phát
```

- void post_rate(const char *argument, http_response_t *res):

```
Nhận từ client:
params_map["value"]: Tốc độ chạy video
0: Dừng video
1: Chạy video ở tốc độ bình thường
```

- void post_stop(http_response_t *res):

```
Ngừng phát video
```

- void put_photo(const Request &rq, http_response_t *res):

```
Data photo: rq.get_data()
Size photo: rq.get_len_data()  
```


Protocol: http://nto.github.io/AirPlay.html

Http Parser: https://github.com/joyent/http-parser

PlistCpp: https://github.com/animetrics/PlistCpp