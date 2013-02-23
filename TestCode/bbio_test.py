# Import PyBBIO library:
from bbio import *
import threading
import random

LED = PWM2A
POT = AIN5

brightness = 0  # Global variable to store brightness level
inc = 1         # How much to increment the brightness by

serial_output = 127 # What the serial output should be

# Create a setup function:
def setup():
  global pwmThread, ainThread, stxThread
  # Start the serial port at 9600 baud:
  Serial2.begin(9600)

  # Add threads
  pwmThread = threading.Timer(0.01, pwm_loop)
  ainThread = threading.Timer(0.5, analog_loop)
  stxThread = threading.Timer(0.025, serial_loop)

  # Start them
  pwmThread.start();
  ainThread.start();
  stxThread.start();

def pwm_loop():
  global brightness, inc
  
  # Set the PWM duty cycle:
  analogWrite(LED, brightness)
  # Increment value:
  brightness += inc
  if ((brightness == 255) or (brightness == 0)):
    # Change increment direction:
    inc *= -1
  
def analog_loop():
  val = analogRead(POT)
  voltage = inVolts(val)
  print " ADC value: %i - voltage: %0.2f" % (val, voltage)

def serial_loop():
  global serial_output

  serial_output = max(min(serial_output+random.randint(-1,1),255),0)
  Serial2.write(serial_output)

def kill_timer(timer):
  while True:
    try:
      timer.cancel()
      break
    except:
      pass

def loop():
  global pwmThread, ainThread, stxThread

  try:
    delay(30)
  except Exception, e:
    kill_timer(stxThread)
    kill_timer(ainThread)
    kill_timer(pwmThread)
    raise e

# Start the loop:
run(setup, loop)

