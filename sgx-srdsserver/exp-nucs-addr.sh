#!/bin/bash

export raspi=$(curl 'http://msstream.net:3000' | jq --raw-output '."pi"."ip"')

#http://192.168.1.100:3000
#ssh pi@"$raspi"
#ssh -o ProxyCommand="ssh -W %h:%p pi@$raspi" nuc@$nuc1

export nuc1=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc1"."ip"')
export nuc2=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc2"."ip"')
export nuc3=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc3"."ip"')
export nuc4=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc4"."ip"')
export nuc5=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc5"."ip"')
export nuc6=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc6"."ip"')

export nuc11=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc11"."ip"')
export nuc12=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc12"."ip"')
export nuc13=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc13"."ip"')
export nuc14=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc14"."ip"')
export nuc15=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc15"."ip"')
export nuc16=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc16"."ip"')
export nuc17=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc17"."ip"')
export nuc18=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc18"."ip"')
#export nuc19=$(ssh pi@"$raspi" curl 'http://192.168.1.100:3000' | jq --raw-output '."nuc19"."ip"')

