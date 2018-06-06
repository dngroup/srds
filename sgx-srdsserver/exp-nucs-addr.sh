#!/bin/bash

set -x

nuc1=$(curl 'http://msstream.net:3000' | jq --raw-output '."nuc1"."ip"')
nuc2=$(curl 'http://msstream.net:3000' | jq --raw-output '."nuc2"."ip"')
nuc3=$(curl 'http://msstream.net:3000' | jq --raw-output '."nuc3"."ip"')
nuc4=$(curl 'http://msstream.net:3000' | jq --raw-output '."nuc4"."ip"')
nuc5=$(curl 'http://msstream.net:3000' | jq --raw-output '."nuc5"."ip"')
nuc6=$(curl 'http://msstream.net:3000' | jq --raw-output '."nuc6"."ip"')

nuc11=$(curl 'http://msstream.net:3000' | jq --raw-output '."nuc11"."ip"')
nuc12=$(curl 'http://msstream.net:3000' | jq --raw-output '."nuc12"."ip"')
nuc13=$(curl 'http://msstream.net:3000' | jq --raw-output '."nuc13"."ip"')
nuc14=$(curl 'http://msstream.net:3000' | jq --raw-output '."nuc14"."ip"')
nuc15=$(curl 'http://msstream.net:3000' | jq --raw-output '."nuc15"."ip"')
nuc16=$(curl 'http://msstream.net:3000' | jq --raw-output '."nuc16"."ip"')
nuc17=$(curl 'http://msstream.net:3000' | jq --raw-output '."nuc17"."ip"')
nuc18=$(curl 'http://msstream.net:3000' | jq --raw-output '."nuc18"."ip"')
nuc19=$(curl 'http://msstream.net:3000' | jq --raw-output '."nuc19"."ip"')
