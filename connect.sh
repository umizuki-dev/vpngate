CONFIG_DIR="./configs"
TIMEOUT=10

make
wget https://www.vpngate.net/api/iphone/ -O list.csv
./trim
./generate

for config in "$CONFIG_DIR"/*.ovpn; do
    echo "Trying $config"

    sudo openvpn --config "$config" --data-ciphers AES-128-CBC --daemon --writepid /tmp/openvpn.pid --log /tmp/openvpn.log

    success=false
    for ((i=0; i<TIMEOUT; i++)); do
        if grep -q "Initialization Sequence Completed" /tmp/openvpn.log; then
            echo "Connected using $config"
            success=true
            break
        fi
        sleep 1
    done

    if $success; then
        exit 0
    else
        echo "Failed to connect with $config"
        if [ -f /tmp/openvpn.pid ]; then
            sudo kill "$(cat /tmp/openvpn.pid)" 2>/dev/null
        fi
        sleep 2
    fi
done

echo "All configs tried, none connected."
exit 1
