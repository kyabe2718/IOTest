#! /bin/bash

devname=$1

type smartctl > /dev/null 2>&1

if [[ -n $? ]] ; then
    echo "smartctl is not found"
    exit
fi

echo "smartctl is found"

sudo smartctl -i ${devname} | tee smartctl.log

rpm=$(cat smartctl.log | grep "Rotation Rate" | sed -e "s/[^0-9]*//g")

max_rot_lat_ms=$(( 60 * 1000 / $rpm))
ave_rot_lat_ms=$(( $max_rot_lat_ms / 2 ))

echo "average rotational latency: $ave_rot_lat_ms ms"

echo "expected IOPS: $((1000 / $ave_rot_lat_ms))"

rm smartctl.log

