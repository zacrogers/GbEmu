#!/bin/bash

# Color escape sequences
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color


if [[ $1 == "DOCTOR" ]]; then

cd desktop

cmake .

echo -e "${BLUE}*********************************${NC}"
echo -e "${RED}**********   ${GREEN}Building   ${RED}*********${NC}"
echo -e "${BLUE}*********************************${NC}"

make

echo -e "${BLUE}*********************************${NC}"
echo -e "${RED}**********    ${GREEN}Running   ${RED}*********${NC}"
echo -e "${BLUE}*********************************${NC}"

./GameboyEmulatorDesktop '../roms/01-special.gb' > ../tools/gameboy-doctor/output.txt
cd ../tools/gameboy-doctor
./gameboy-doctor output.txt cpu_instrs 1


elif [[ $1 == "DESKTOP" ]]; then


cd desktop

cmake .

echo -e "${BLUE}*********************************${NC}"
echo -e "${RED}**********   ${GREEN}Building   ${RED}*********${NC}"
echo -e "${BLUE}*********************************${NC}"

make

echo -e "${BLUE}*********************************${NC}"
echo -e "${RED}**********    ${GREEN}Running   ${RED}*********${NC}"
echo -e "${BLUE}*********************************${NC}"

./GameboyEmulatorDesktop '../roms/01-special.gb'

elif [[ $1 == "EMBEDDED" ]]; then

cd embedded
west build -p always -b esp32 && west flash && west espressif monitor

else

echo "You have enter something, ya dingus. Select either 'DESKTOP' or 'EMBEDDED'"

fi
