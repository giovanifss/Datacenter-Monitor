#!/bin/bash

if [ "$EUID" -ne 0 ]
  then echo "::Error: Please run as root"
  exit
fi

echo "$(tput setaf 1)==> Downloading library$(tput sgr 0)"
git clone git@github.com:adafruit/DHT-sensor-library.git

echo "$(tput setaf 1)==> Copying code files$(tput sgr 0)"
mkdir DHT
cp DHT-sensor-library/DHT.cpp DHT-sensor-library/DHT.h DHT/

echo "$(tput setaf 1)==> Removing files$(tput sgr 0)"
sudo rm -r DHT-sensor-library

echo "$(tput setaf 1)==> Instaling library$(tput sgr 0)"
sudo mv DHT/ /usr/share/arduino/libraries/

echo "$(tput setaf 1)==> Successfully installed library$(tput sgr 0)"
