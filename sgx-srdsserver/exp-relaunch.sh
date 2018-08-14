#!/bin/bash

set -x

source exp-stop.sh

echo "Hope you comitted."

git push

source exp-fast-update.sh

sleep 5

source exp-setup.sh

