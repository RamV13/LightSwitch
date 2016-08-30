# Arduino WiFi Light Switch

## Usage

### Materials

1. Arduino UNO
2. Adafruit WiFi Shield
3. Arduino Relay
4. Tools - Screwdriver, Wire cutters, Soldering Kit
5. Cables - USB

### Hardware

1. Connect an Arduino UNO to a relay switch and an Adafruit WiFi Shield (adhere to the PIN connections in the code or adjust the constants in the code itself)
2. TURN OFF THE FUSE IN THE HOUSEHOLD/BUILDING
3. Carefully install the Arduino in series or in parallel (depending on intended behavior) with the existing light circuit
4. Connect the Arduino to a permanent power source (batteries will run out quickly)
5. Turn the fuse in the household/building back on

### Software

1. Replace the lines with `<SSID>` and `<PASSWORD>` with your LAN information.
2. Load the code onto the Arduino UNO using the USB cable and the Arduino IDE
3. Send an HTTP POST request with a payload containing the keyword `switch` (see [lightswitch.sh](https://github.com/RamV13/LightSwitch/blob/master/lightswitch.sh) for reference)

[See this image for reference](http://ramvellanki.com/img/projects/lightswitch.jpg)
