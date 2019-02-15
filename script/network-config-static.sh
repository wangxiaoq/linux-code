#!/bin/bash

#
# Author: Herbert Wang
# Date: 2019.02.15
#

# list  network interfaces and let user select.
net_interfaces=($(ifconfig -a | sed '/^        /d;/^$/d' | awk -F: '{print $1}'))

echo "Available network interfaces:"
echo
for((i=0;i<${#net_interfaces[@]};i++))
do
    echo $i. ${net_interfaces[$i]}
done
echo
read -p "Your choice[0-$((${#net_interfaces[@]}-1))]: " net_choice

if [ $net_choice -lt 0 -o $net_choice -ge ${#net_interfaces[@]} ]; then
    echo "ERROR: Invalid input, your choice should in range: [0-$((${#net_interfaces[@]}-1))]"
    exit 1
fi
echo

# read network configuration information: IP address, network mask and gateway.

check_ip()
{
    echo $1 | egrep -q "^([0-9]{1,3}\.){3}[0-9]{1,3}$"
    return $?
}

#
# read ip address, network mask and gateway.
# $1 - "ip address"
#      "network mask"
#      "gateway"
#
read_net_config()
{
    read -p "Input $1: " config
    try_time=0
    check_ip $config
    while [ $? -ne 0 ]
    do
        let try_time++
        if [ $try_time -ge 3 ]; then
            exit 1
        fi
        echo "$1 format error" 1>&2
        read -p "Input $1 again: " config
        check_ip $config
    done
    echo $config
}

#
# change ip format of network mask to number format.
# e.g: "255.255.255.0" -> 24
#      "255.255.254.0" -> 23
#
# $1 - mask
#
network_mask_ip_format_to_number_format()
{
    check_ip $1
    if [ $? -ne 0 ]; then
        echo "network_mask_ip_format_to_number_format error" 1>&2
        exit 1
    fi

    nums=($(echo $1 | tr '.' ' '))
    count=0
    for ((i=0;i<${#nums[@]};i++))
    do
        num=${nums[$i]}
        for ((j=0;j<8;j++))
        do
            if [ $[$num&1] -ne 0 ]; then
                let count++
            fi
            num=$[$num>>1]
        done
    done
    echo $count
}

ip_addr=$(read_net_config "ip address")
if [ $? -ne 0 ]; then
    exit 1
fi
net_mask=$(read_net_config "network mask")
if [ $? -ne 0 ]; then
    exit 1
fi
gateway=$(read_net_config "gateway")
if [ $? -ne 0 ]; then
    exit 1
fi

mask_num=$(network_mask_ip_format_to_number_format $net_mask)

# get the OS version to determine which configuration file to use
os=$(cat /proc/version | egrep "(Red Hat|ubuntu)" -o)
config_file=""
if [[ $os == "Red Hat" ]]; then
    config_file="/etc/sysconfig/network-scripts/ifcfg-${net_interfaces[$net_choice]}"
elif [[ $os == "ubuntu" ]]; then
    config_file="/etc/network/interfaces"
fi

if [[ $os == "Red Hat" ]]; then
cat <<EOF> $config_file
TYPE=Ethernet
PROXY_METHOD=none
BROWSER_ONLY=no
BOOTPROTO=none
DEFROUTE=yes
IPV4_FAILURE_FATAL=no
IPV6INIT=yes
IPV6_AUTOCONF=yes
IPV6_DEFROUTE=yes
IPV6_FAILURE_FATAL=no
IPV6_ADDR_GEN_MODE=stable-privacy
NAME=${net_interface[$net_choice]}
DEVICE=${net_interface[$net_choice]}
ONBOOT=yes
IPADDR=$ip_addr
PREFIX=$mask_num
GATEWAY=$gateway
DNS1=$gateway
EOF

service network restart

elif [[ $os == "ubuntu" ]]; then
cat <<EOF> $config_file
auto lo
iface lo inet loopback

# The primary network interface
auto ${net_interfaces[$net_choice]}

iface ${net_interfaces[$net_choice]} inet static
address $ip_addr
netmask $net_mask
gateway $gateway

# This is an autoconfigured IPv6 interface
iface ${net_interfaces[$net_choice]} inet6 auto
EOF

/etc/init.d/networking restart
fi
