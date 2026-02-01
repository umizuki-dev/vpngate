#!/bin/bash

if [ -f /tmp/openvpn.pid ]; then
    pid=$(cat /tmp/openvpn.pid)
    if ps -p $pid > /dev/null; then
        sudo kill $pid
        echo "Disconnected OpenVPN process with PID $pid"
    else
        echo "OpenVPN process with PID $pid not found, already disconnected?"
    fi

    rm /tmp/openvpn.pid
else
    echo "No OpenVPN PID file found. Is OpenVPN running?"
fi
