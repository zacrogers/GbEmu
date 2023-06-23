#!/bin/bash

echo "Creating virtual environment and installing west"
python3 -m venv ~/zephyrproject/.venv
source ~/zephyrproject/.venv/bin/activate
pip install west

echo "Initialise zephyr and fetch all of the modules"
west init
west update

echo "Export the zephyr cmake package"
west zephyr-export

echo "Install the zephyr python dependencies"
pip install -r ~/zephyrproject/zephyr/scripts/requirements.txt

# echo "Install embedded template library"
# git clone https://github.com/ETLCPP/etl.git
# cd etl
# git checkout master
# cmake -B build .
# cmake --install build/