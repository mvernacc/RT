# Combined examples taken from https://github.com/alexanderhiam/PyBBIO
# I already installed the python module. Simply run sudo python bbio_test.py

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

  # Run analog reading once every 500 ms
  if frame%50 == 0:
    val = analogRead(POT)
    voltage = inVolts(val)
    print " ADC value: %i - voltage: %0.2f" % (val, voltage)

  # Run serial writing once every 30 ms
  if frame%3 == 0:
    Serial2.write(random.randint(0,255))
  
  delay(10)

# Start the loop:
#run(setup, loop) <-- this is the library's normal way of doing startup

# This is the code that the library runs. If we want to have our own control
# loop, then we can probably play around with this.
try:
  bbio_init()
  setup()
  while (True):
    loop()
except KeyboardInterrupt:
  # Manual exit signal, clean up and exit happy
  bbio_cleanup()
except Exception, e:
  # Something may have gone wrong, clean up and re-raise exception
  bbio_cleanup()
  raise e
