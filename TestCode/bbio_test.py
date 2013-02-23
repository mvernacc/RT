# Import PyBBIO library:
from bbio import *
import threading
import random

LED = PWM2A
POT = AIN5

brightness = 0  # Global variable to store brightness level
inc = 1         # How much to increment the brightness by

frame = 0

# Create a setup function:
def setup():
  # Start the serial port at 9600 baud:
  Serial2.begin(9600)
  
def loop():
  global frame, brightness, inc

  frame += 1

  # Set the PWM duty cycle:
  analogWrite(LED, brightness)
  # Increment value:
  brightness += inc
  if ((brightness == 255) or (brightness == 0)):
    # Change increment direction:
    inc *= -1

  if frame%50 == 0:
    val = analogRead(POT)
    voltage = inVolts(val)
    print " ADC value: %i - voltage: %0.2f" % (val, voltage)

  if frame%3 == 0:
    Serial2.write(random.randint(0,255))
  
  delay(10)

# Start the loop:
run(setup, loop)
