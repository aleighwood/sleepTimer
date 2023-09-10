# sleepTimer
A device that records how long you sleep for and how long it takes you to fall asleep, then uplaods this data to Google Spreadsheets

# Working Principle
![57118](https://github.com/aleighwood/sleepTimer/assets/86426050/92cfe88e-999b-4309-99ab-769da68fd9dd)
There are 3 modes of the device:

In-Bed: Selected when the user goes to bed, the user can trigger the sleep timer at any point (up to an hour after the most recent trigger). Starts the 'In'Bed' timer
Reset: The timer will reset if 'Reset' is selected before 'In-Bed' is selected
Send-Data: Selected when the user wakes up, sleep data is uploaded to a Google Spreadsheet using a webhook and [IFTTT](https://ifttt.com/).

The 'Sleep-Timer' is triggered when the user moves there hand over the 

# Circuit Diagram 
![Screenshot 2023-08-20 114150](https://github.com/aleighwood/sleepTimer/assets/86426050/cda6f0f9-e93d-4a6b-82bd-040165173171)

# Requirements

[OLED display](https://randomnerdtutorials.com/guide-for-oled-display-with-arduino/) 

[Ability to programme ESP-8266](https://www.instructables.com/Setting-Up-the-Arduino-IDE-to-Program-ESP8266/)

[IFTTT Google Sheets Integration](https://ifttt.com/google_sheets)

# Spreadsheet Setup

IFTTT updates a Google Spreadsheet on every 'Send Data' event. I recommend creating a separate sheet in the same file to play with the data, otherwise IFTTT gets confused as where to update the spreadsheet.

Here is a screenshot of my sleep data from a few years ago (I made this device a while ago!).

![Screenshot 2023-09-10 140743](https://github.com/aleighwood/sleepTimer/assets/86426050/cc1c724a-4a71-4c0c-b651-c2d1666b1f8b)
