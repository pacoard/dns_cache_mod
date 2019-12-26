#!/bin/sh

[ "$USER" = root ] || { echo "You must run $0 as root."; exit 1; }

[ -d root ] || mkdir root
[ -d root/ip ] || mkdir root/ip

# INI paco
[ -f root/ip/3.2.4.1 ] || touch root/ip/3.2.4.1
[ -f root/ip/12.234.5.1 ] || touch root/ip/12.234.5.1
[ -f root/ip/127.0.0.1 ] || touch root/ip/127.0.0.1
[ -f root/ip/1.34.0.3 ] || touch root/ip/1.34.0.3
# END paco

[ -d root/servers ] || mkdir root/servers
[ -f root/servers/@ ] || cp dnsroots.global root/servers/@

export ROOT=./root
export IP=127.0.0.1
export IPSEND=0.0.0.0
export CACHESIZE=1024576
export GID=0

# INI paco
# Save public IP as an env variable
#export MY_PUBLIC_IP=$(dig +short @208.67.222.222 myip.opendns.com  | grep '^[.0-9]*$')
export MY_PUBLIC_IP=$(curl ifconfig.co | grep '^[.0-9]*$')
# END paco

# UID is read-only in bash, so we set it with env.
#
# Dnscache requires 128 bytes from standard input to start up; we use
# the dnscache binary itself for that.
exec env UID=0 ./dnscache <dnscache

