# Import PyBBIO library:
from bbio import *
import threading

LED = PWM1A
POT = AIN5
brightness = 0  # Global variable to store brightness level
inc = 1         # How much to increment the brightness by
pause = 10      # Delay in ms between each step

# Create a setup function:
def setup():
  # Start the serial port at 9600 baud:
  Serial2.begin(9600)

  # Add threads
  t = threading.Thread(target=pwm_loop)
  t.daemon = True
  t.start()
  t = threading.Thread(target=analog_loop)
  t.daemon = True
  t.start()
  t = threading.Thread(target=serial_loop)
  t.daemon = True
  t.start()

def pwm_loop():
  global brightness, inc

  # Set the PWM duty cycle:
  analogWrite(LED, brightness)
  # Increment value:
  brightness += inc
  if ((brightness == 255) or (brightness == 0)):
    # Change increment direction:
    inc *= -1
  # Sleep a bit:
  delay(pause)
  
def analog_loop():
  val = analogRead(POT)
  voltage = inVolts(val)
  print " ADC value: %i - voltage: %0.2f" % (val, voltage)
  delay(500)

def serial_loop():
  Serial2.write(randint(0,255))
  delay(25)

def loop():
  delay(30)

# Start the loop:
run(setup, loop)

