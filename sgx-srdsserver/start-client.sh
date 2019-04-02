#!/bin/bash

set -x

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
(while sleep 1; do (./SGXSRDSServerApp 8081 0); done) &

sleep 1

rm -R ~/.cache/chromium/

sleep 1

chromium-browser --headless --remote-debugging-port=7777 --ignore-autoplay-restrictions --autoplay-policy=no-user-gesture-required --no-user-gesture-required --disable-default-apps --no-first-run --disk-cache-dir=/dev/null --disk-cache-size=1 --disable-web-security --user-data-dir="~" --disable-features=CrossSiteDocumentBlockingAlways,CrossSiteDocumentBlockingIfIsolating "http://localhost:8082/samples/privastream/index.html?addr_mpd=5GGKUZXWKE2LVA5VMXRA5JAWJKHQE&addr_tracker=5GGKUZXWKE2LVA5VMXQAPJAWIKHQW&videoID=srds&clientID=$1"

