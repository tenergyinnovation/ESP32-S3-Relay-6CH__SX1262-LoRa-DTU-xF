#!/bin/bash

# ================================================================
# ESP32-S3-POE-ETH-8DI-8DO Project - Library Installation Script
# ================================================================
# Purpose: Clone and install required libraries for the project
# Author: Tenergy Innovation Co., Ltd.
# Date: 28 June 2026
# ================================================================

set -e  # Exit on any error

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Get the script's directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo -e "${BLUE}================================================================${NC}"
echo -e "${BLUE}  ESP32-S3-POE-ETH-8DI-8DO - Library Installation Script${NC}"
echo -e "${BLUE}================================================================${NC}"
echo ""

# ================================================================
# Step 1: Check Prerequisites
# ================================================================
echo -e "${YELLOW}[1/4] Checking prerequisites...${NC}"

if ! command -v git &> /dev/null; then
    echo -e "${RED}❌ ERROR: git is not installed${NC}"
    echo "Please install git first: brew install git (macOS) or apt-get install git (Linux)"
    exit 1
fi
echo -e "${GREEN}✓ git is installed${NC}"

if ! command -v curl &> /dev/null; then
    echo -e "${RED}❌ ERROR: curl is not installed${NC}"
    exit 1
fi
echo -e "${GREEN}✓ curl is installed${NC}"
echo ""

# ================================================================
# Step 2: Define Libraries to Clone
# ================================================================
echo -e "${YELLOW}[2/4] Setting up library list...${NC}"

# ================================================================
# HOW TO ADD LIBRARIES:
# ================================================================
# To add a new library, append entries to both arrays using += syntax:
#
# Example - Adding a new library:
#   LIBRARY_NAMES+=("LibraryName")
#   LIBRARY_URLS+=("https://github.com/owner/repo.git")
#
# The arrays use indexed format for macOS/Linux compatibility.
# Each library name must have a corresponding URL.
# ================================================================

# Core Libraries - Required for ESP32-S3-POE-ETH-8DI-8DO
LIBRARY_NAMES=("Adafruit_NeoPixel" "ArduinoJson" "PubSubClient" "NTPClient" "ESP32-S3-Relay-6CH")
LIBRARY_URLS=(
    "https://github.com/adafruit/Adafruit_NeoPixel.git"
    "https://github.com/bblanchon/ArduinoJson.git"
    "https://github.com/knolleary/pubsubclient.git"
    "https://github.com/taranais/NTPClient.git"
    "https://github.com/tenergyinnovation/ESP32-S3-Relay-6CH__SX1262-LoRa-DTU-xF.git"
)

# Optional Libraries (uncomment to enable)
# LIBRARY_NAMES+=("WiFiManager" "ModbusMaster" "Universal-Arduino-Telegram-Bot")
# LIBRARY_URLS+=("https://github.com/tzapu/WiFiManager.git" "https://github.com/4-20ma/ModbusMaster.git" "https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot.git")

# LoRa Support Libraries (if using LoRa communication)
# LIBRARY_NAMES+=("RadioLib")
# LIBRARY_URLS+=("https://github.com/jgromes/RadioLib.git")

echo -e "${GREEN}Libraries to install:${NC}"
for lib_name in "${LIBRARY_NAMES[@]}"; do
    echo "  • $lib_name"
done
echo ""

for i in "${!LIBRARY_NAMES[@]}"; do
    lib_name="${LIBRARY_NAMES[$i]}"
    lib_url="${LIBRARY_URLS[$i]}"
    lib_path="${SCRIPT_DIR}/${lib_name}"
    
    if [ -d "$lib_path" ]; then
        echo -e "${BLUE}ℹ $lib_name${NC} already exists at ${lib_path}"
        
        # Update existing repo
        echo "  Updating $lib_name..."
        cd "$lib_path"
        if git pull --quiet; then
            echo -e "  ${GREEN}✓ Updated${NC}"
        else
            echo -e "  ${YELLOW}⚠ Could not update (may be offline)${NC}"
        fi
        cd "$SCRIPT_DIR"
    else
        echo -e "${BLUE}Cloning $lib_name...${NC}"
        if git clone --quiet --depth 1 "$lib_url" "$lib_path"; then
            echo -e "  ${GREEN}✓ Successfully cloned${NC}"
        else
            echo -e "  ${RED}✗ Failed to clone${NC}"
            exit 1
        fi
    fi
done
echo ""

# ================================================================
# Step 4: Verify Installation
# ================================================================
echo -e "${YELLOW}[4/4] Verifying installation...${NC}"

ALL_SUCCESS=true
for lib_name in "${LIBRARY_NAMES[@]}"; do
    lib_path="${SCRIPT_DIR}/${lib_name}"
    
    if [ -d "$lib_path" ]; then
        # Count files in the library
        file_count=$(find "$lib_path" -type f | wc -l)
        echo -e "${GREEN}✓ $lib_name${NC} ($file_count files)"
    else
        echo -e "${RED}✗ $lib_name NOT FOUND${NC}"
        ALL_SUCCESS=false
    fi
done
echo ""

# ================================================================
# Final Summary
# ================================================================
echo -e "${BLUE}================================================================${NC}"
if [ "$ALL_SUCCESS" = true ]; then
    echo -e "${GREEN}✅ All libraries installed successfully!${NC}"
    echo ""
    echo -e "${YELLOW}Next steps:${NC}"
    echo "  1. Open the project in Arduino IDE or PlatformIO"
    echo "  2. The libraries should now be available in lib/ folder"
    echo "  3. Include headers in your sketch: #include <LibraryName.h>"
    echo ""
    echo -e "${YELLOW}Library locations:${NC}"
    for lib_name in "${LIBRARY_NAMES[@]}"; do
        echo "  • ${SCRIPT_DIR}/${lib_name}"
    done
else
    echo -e "${RED}❌ Some libraries failed to install${NC}"
    echo "Please check the errors above and try again"
    exit 1
fi
echo -e "${BLUE}================================================================${NC}"

