#!/bin/bash

# Color escape sequences
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

cmake .

echo -e "${BLUE}*********************************${NC}"
echo -e "${RED}**********   ${GREEN}Building   ${RED}*********${NC}"
echo -e "${BLUE}*********************************${NC}"
make

echo -e "${BLUE}*********************************${NC}"
echo -e "${RED}**********    ${GREEN}Running   ${RED}*********${NC}"
echo -e "${BLUE}*********************************${NC}"
./GbEmu 'roms/cpu_instrs.gb'
