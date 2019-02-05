# Raspberry Pi setup instructions

## Timezone and locales

en-US
UTC timezone
QWERTY US International with dead keys

Remember to enable SSH!

## Cron job to publish public IP address

sudo bash -c "echo \"*/10 * * * * pi curl -X POST http://msstream.net:3000/raspi\" >> /etc/crontab"

## DHCPCD for static IP

sudo nano /etc/dhcpcd.conf

``
interface eth1
static ip_address=192.168.1.100/24
static routers=192.168.1.100
static domain_name_servers=192.168.1.100
``

sudo systemctl daemon-reload
sudo service networking restart

## IP forward

net.ipv4.ip_forward=1 in /etc/sysctl.conf
net.ipv6.ip_forward=1 in /etc/sysctl.conf

``
sudo iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
``

## DNSMASQ for DHCP server

sudo apt update
sudo apt install dnsmasq
sudo nano /etc/dnsmasq.conf

``
interface=eth1
dhcp-range=192.168.1.101,192.168.1.120,255.255.255.0,12h
dhcp-option=6,192.168.1.100
no-hosts
addn-hosts=/etc/hosts.dnsmasq
``

sudo nano /etc/hosts.dnsmasq

``
192.168.0.1	raspi
``

sudo systemctl daemon-reload
sudo service dnsmasq restart
sudo service networking restart

## REST server for NUCs IP addresses

Setup Docker (get docker script + usermod)

cd ~
git clone https://github.com/mlacaud/whatisip.git
cd whatisip
sudo apt-get install nodejs npm
npm install
docker build -t whatisip .
docker run -d -p 3000:3000 --name whatisip --restart=always whatisip
docker exec -it whatisip /bin/sh
mkdir /node/src/json/



