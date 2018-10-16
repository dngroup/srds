#!/bin/bash

set -x

cd ~/srds/sgx-srdsserver
source ~/srds/sgx-srdsserver/stop-client.sh

sleep 1

cd ~/content
docker run -d -p 8080:8080 -v ~/content:/src/main/ressources msstream/server_srds

sleep 1

cd ~/player
git checkout dev_srds
http-server --cors -p 8082 &

sleep 1

cd ~/srds/sgx-srdsserver
source /opt/intel/sgxsdk/environment
make clean
make
./SGXSRDSServerApp 8081 0 &

sleep 10

cd ~/wrk2

./wrk -t"$1" -c"$2" -d"$3"s -R"$4" -s report.lua --timeout 60s -H "Origin: http://localhost:8082" -H "Accept-Encoding: gzip, deflate, br" -H "X-Forwarded-Host: 5GGKUZXWKE2LVA5VMXRA5JAWJKHQE" -H "Accept-Language: en-US,en;q=0.9" -H "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Ubuntu Chromium/69.0.3497.81 Chrome/69.0.3497.81 Safari/537.36" -H "Accept: */*" -H "Referer: http://localhost:8082/samples/dash-if-reference-player/index.html" -H "Connection: keep-alive" "http://localhost:8081/api/description/srds/7?order=111111111111&q0=0&q1=4129849"

# chromium-browser --headless --remote-debugging-port=7777 --ignore-autoplay-restrictions --no-user-gesture-required --disable-default-apps --no-first-run --disk-cache-dir=/dev/null --disk-cache-size=1 http://localhost:8082/samples/dash-if-reference-player/index.html

# chromium-browser --headless --remote-debugging-port=7777 --ignore-autoplay-restrictions --autoplay-policy=no-user-gesture-required --no-user-gesture-required --disable-default-apps --no-first-run --disk-cache-dir=/dev/null --disk-cache-size=1 "http://localhost:8082/samples/privastream/index.html?addr_mpd=5GGKUZXWKE2LVA5VMXRA5JAWJKHQE&addr_tracker=5GGKUZXWKE2LVA5VMXQAPJAWIKHQW&videoID=srds"

# http://localhost:8082/samples/privastream/index.html?addr_mpd=ADDR:PORT&addr_tracker=ADDR:PORT&videoID=srds&seek=0&addr_proxy=ADDR:PORT&addr_server=ADDR:PORT
# http://localhost:8082/samples/privastream/index.html?addr_mpd=5GGKUZXWKE2LVA5VMXRA5JAWJKHQE&addr_tracker=5GGKUZXWKE2LVA5VMXQAPJAWIKHQW&videoID=srds

