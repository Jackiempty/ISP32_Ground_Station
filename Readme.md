# Ground Station
## Introduction
Ground station is a system that receive data using LoRa protocol from the rocket and upload those data to cloud so everyone can access the data from the rocket in real time.  

## Structure
### MCU
The most important part of the system is its control board, MCU, and the program running on it.  

The program handle the tasks of receiving data and processing it, by using LoRa library, it receives raw data and parses it into each needed data and sends it up to the cloud using influxdb library.  

In addition, there are two mode of sending data up to cloud:  
- MCU as a Wifi client:  
If the environment MCU is at is equipped with Wifi and internet, the MCU can just connect to it and upload the data to the cloud located in the same LAN. Also, if the LAN is connected to internet, the data on the cloud will be able to be accessed by everyone on internet.  

- MCU as a Wifi AP:  
If the environment is not equipped with Wifi, the MCU itself can be the Wifi AP and upload the data to the cloud connected to the MCU's LAN. However, since MCU itself is not connected to internet, the cloud in the LAN would not be able to be accessed by anyone outside of the same LAN as the DB server.  

### Data Base
The data base is based on influxdb and the data is upload to the influxdb server under the same LAN through library functions.  

### Antenna
The antenna is an individual part of the system and is equipped with rotatable servo motor so that the antenna can rotate and trace the direction of the rocket to ensure the LoRa communication robustness during the flight of the rocket. The tracing control command will be provided by MCU in the future, while for now it is running on arduino as an independent sub-system, we feed it the coordinate that we aquire from the LoRa data and it automatically follows the rocket.  

### Rocket
Also equipped with a MCU that sends LoRa data containing all those important data of the current situation of the rocket.  