#!/bin/bash

set -x

raspi=$(curl 'http://msstream.net:3000' | jq --raw-output '."pi"."ip"')

#http://192.168.1.100:3000
#ssh pi@"$raspi"
#ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" nuc@$nuc1

nuc1=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc1"."ip"')
nuc2=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc2"."ip"')
nuc3=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc3"."ip"')
nuc4=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc4"."ip"')
nuc5=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc5"."ip"')
nuc6=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc6"."ip"')

nuc11=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc11"."ip"')
nuc12=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc12"."ip"')
nuc13=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc13"."ip"')
nuc14=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc14"."ip"')
nuc15=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc15"."ip"')
nuc16=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc16"."ip"')
nuc17=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc17"."ip"')
nuc18=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc18"."ip"')
nuc19=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc19"."ip"')

