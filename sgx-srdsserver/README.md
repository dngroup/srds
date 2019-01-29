# Raspberry Pi setup instructions

## Timezone and locales

en-US
UTC timezone
QWERTY US International with dead keys

Remember to enable SSH!

## Cron job to publish public IP address

``
sudo bash -c "echo \"*/10 * * * * pi curl -X POST http://msstream.net:3000/raspi\" >> /etc/crontab"
``

## DHCPCD for static IP

``
sudo nano /etc/dhcpcd.conf
``

``
interface eth1
static ip_address=192.168.1.100/24
static routers=192.168.1.100
static domain_name_servers=192.168.1.100
``

``
sudo systemctl daemon-reload
sudo service networking restart
``

## DNSMASQ for DHCP server

``
sudo nano /etc/dnsmasq.conf
``

``
interface=eth1
dhcp-range=192.168.1.101,192.168.1.120,255.255.255.0,12h
``

``
sudo systemctl daemon-reload
sudo service dnsmasq restart
sudo service networking restart
``

