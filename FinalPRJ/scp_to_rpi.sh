#!/bin/bash
# Script for secure copying files from host to target (Raspberry PI)
# One parameter:
# 1) The IP-address of the target Raspberry PI.
#
# Example of calling the script in the terminal:
# "./scp_to_pi.sh 10.9.8.2"
# This will copy prog.exe from the host machine to the '/home/pi/Downloads/' directory on the Raspberry PI.
if [ "$1" = "" ]; then
	echo "Missing IP-address for target."
	echo ">>> Example of correct parameter '10.9.8.2' <<<"
else
	echo "Copying file 'prog.exe' to destination 'home/pi/Downloads/' on Raspberry PI"
	scp bin/target/prog.exe pi@$1:Downloads/
fi
