#!/bin/bash

# Create virtual env and install west
python3 -m venv ~/zephyrproject/.venv
source ~/zephyrproject/.venv/bin/activate
pip install west

# Init zephyr and fetch all of the modules
west init
west update

# Export the zephyr cmake package
west zephyr-export

# Install the zephyr python deps
pip install -r ~/zephyrproject/zephyr/scripts/requirements.txt