# GPS Simulation

This project was made to simulate a travel with a boat. It will calculate the position during the travel based on the parameters you will set.

## To begin

### Prerequisites

- A web browser
- A device with an access to Wi-Fi connections
- Something to power up the ESP32

### Installation

Open your Wi-Fi settings on your device, select "ESP32-GPS" and type the password. It's the access point made by the ESP. When you are connected to the access point, you can access to the web page with your browser.

Open your browser and type "192.168.4.1" that will lead you to the web page where there is what you need to get started.

## Start-up

### Access Point (AP)

When you are connected to the AP and you are on the web page, you can choose to start a simulation or to connect the ESP32 to a Wi-Fi network.

If you choose to start a simulation, there is two possibilities "Destination" or "COG".

- The "Destination" mode allows you to set a starting point, a speed and an arrival point. When the settings are set and submit the simulation will start. You will see your settings with your current position and the distance travelled. Once you have reached your destination, the simulation will stop and you will be able to start an another one or you can stop it manually.

- The "COG" mode allows you to set a starting point, a speed and a course over ground. When the settings are set and submit the simulation will start. You will see your settings with your current position and the distance travelled following the direction set. You can stop the simulation and start another whenever you want to.

If you choose to connect the ESP to Wi-Fi you just need to set the name, the password, the IP address for the ESP, the gateway address and the subnet address according to your Wi-Fi. Then the ESP will try to connect to your Wi-Fi. Once connected, the AP will be disabled and if the ESP loose the Wi-Fi connection, it will try to reconnect and if it failed it will restart the AP and try again later.

### Wi-Fi

To access the Wi-Fi mode's web page, open your browser and type the IP address you set for the ESP.

When you are connected to the Wi-Fi and you are on the web page, you can start a simulation just like the AP but with a map to show the simulation in real time with markers on each position calculated to show where you are.

## Made with

* Arduino  
* ESP32 
* HTML/CSS/JavaScript


