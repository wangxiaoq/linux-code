#!/bin/bash

#
# author: Herbert Wang
# Date: 2019.02.15
#

check_err()
{
    if [ $? -ne 0 ]; then
        echo "*************$1*****************"
        exit 1
    fi
}

net_interfaces=($(ifconfig -a | sed '/^        /d; /^$/d' | awk -F: '{print $1}'))
echo -e "AVAILABLE NETWORK INTERFACES:\n"
for ((i=0;i<${#net_interfaces[@]};i++))
do
    echo "$i. ${net_interfaces[$i]}"
done
echo 
read -p "YOUR CHOICE: " choice
if [ $choice -lt 0 -o $choice -ge ${#net_interfaces[@]} ]; then
    echo "*************INPUT ERROR: INVALID CHOICE*****************"
    exit 1
fi

read -p "IP ADDRESS: " ip_addr
echo $ip_addr | grep -E -q "^([0-9]{1,3}\.){3}[0-9]{1,3}$"
check_err "INPUT ERROR: INVALID IP ADDRESS"

read -p "NETWORK MASK: " mask
echo $mask | grep -E -q "^([0-9]{1,3}\.){3}[0-9]{1,3}$"
check_err "INPUT ERROR: INVALID NETWORK MASK"

read -p "GATEWAY: " gateway
echo $gateway | grep -E -q "^([0-9]{1,3}\.){3}[0-9]{1,3}$"
check_err "INPUT ERROR: INVALID GATEWAY"

ifconfig ${net_interfaces[$choice]} $ip_addr
check_err "CONFIG IP ADDRESS ERROR"

ifconfig ${net_interfaces[$choice]} netmask $mask
check_err "CONFIG NETMASK ERROR"

route add default gw $gateway 2>/dev/null

echo -n "nameserver $gateway" >> /etc/resolve.conf

echo "NETWORK CONFIG SUCCESSFULLY"
