# Sleep Timer
A device that records how long you sleep for and how long it takes you to fall asleep, then uploads this data to a Google Spreadsheet.

# Working Principle
![57118](https://github.com/aleighwood/sleepTimer/assets/86426050/92cfe88e-999b-4309-99ab-769da68fd9dd)
There are 3 modes of the device, selected my a potentiometer connected to an analog read pin:

**In-Bed**: Selected when the user goes to bed, the user can trigger the sleep timer. Starts the 'In-Bed' timer.

**Reset**: The timer will reset if 'Reset' is selected before 'In-Bed' is selected.

**Send-Data**: Selected when the user wakes up, sleep data is uploaded to a Google Spreadsheet using a webhook and [IFTTT](https://ifttt.com/). 3 values are calculated: 
+ Time in Bed (duration of **In-Bed** selected to **Send-Data** )
+ Time to fall asleep (duration of **In-Bed** selected to latest sleep timer trigger)
+ Time asleep  (latest sleep timer trigger to **Send-Data** selected)

The sleep timer is triggered when the user moves their into the range of the PIR sensors, these may be placed on the floor by the side of your bed for example. When the sleep timer is trigged, the device assummes the user will fall asleep within 10 minuets. The user can trigger the sleep timer at any point up to 40 minuets after the most recent trigger - after this the trigger is locked so it is not activated when the user wakes up and triggers the PIR sensors.

# Circuit Diagram 
![Screenshot 2023-08-20 114150](https://github.com/aleighwood/sleepTimer/assets/86426050/cda6f0f9-e93d-4a6b-82bd-040165173171)

# Requirements

[OLED display](https://randomnerdtutorials.com/guide-for-oled-display-with-arduino/) 

[Ability to programme ESP-8266](https://www.instructables.com/Setting-Up-the-Arduino-IDE-to-Program-ESP8266/)

[IFTTT Google Sheets Integration](https://ifttt.com/google_sheets)

# Hardware Note

I used a Node-MCU with an ESP8266 chip, other dev boards are available. 

Two PIR sensors have been used, their values are AND-ed. This setup reduces erroneous PIR sensor triggering. 

# Spreadsheet Setup

IFTTT updates a Google Spreadsheet on every 'Send Data' event. I recommend creating a separate sheet in the same file to play with the data, otherwise IFTTT gets confused as where to update the spreadsheet.

Here is a screenshot of my sleep data from a few years ago (I made this device a while ago!).

![Screenshot 2023-09-10 140743](https://github.com/aleighwood/sleepTimer/assets/86426050/cc1c724a-4a71-4c0c-b651-c2d1666b1f8b)
