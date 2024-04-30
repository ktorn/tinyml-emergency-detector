import serial
import pygame
import time

# Initialize the pygame mixer
pygame.mixer.init()

# Load sound files
low_tick_sound = pygame.mixer.Sound("tick.wav")
alarm_sound = pygame.mixer.Sound("alarm.wav")

# Open the serial port (replace '/dev/ttyUSB0' with the correct port for your setup)
arduino_port = '/dev/cu.usbmodem14201'  # Adjust to the correct serial port
baud_rate = 9600  # Ensure this matches the baud rate of your Arduino program
ser = serial.Serial(arduino_port, baud_rate)

try:
    while True:
        # Check if there's any data available
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()

            print(line)

            # Check the content of the serial line
            if "Event: TICK" in line:
                low_tick_sound.play()
            elif "Event: EMERGENCY" in line:
                alarm_sound.play()

        # Add a short delay to avoid CPU overuse
        time.sleep(0.1)

except KeyboardInterrupt:
    print("Exiting...")

finally:
    ser.close()
    pygame.mixer.quit()

