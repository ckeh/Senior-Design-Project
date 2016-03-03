#!/usr/bin/python

# Test Python script to check Xbox controller events
# Requires lego-pi project from https://github.com/zephod/lego-pi
# See http://mattdyson.org/blog/2013/01/using-an-xbox-360-wireless-controller-with-raspberry-pi/
#
# Matt Dyson 05/01/2013
# http://mattdyson.org

from legopi.lib import xbox_read
import serial
import time
from struct import *

#from legopi lib
# object consists of
# key = button pushed
# value = current val 
# old_value

lights = 'A'
right_bumper = ""
left_bumper = ""
right_stick_x = "X1"
right_stick_y = "Y1"
DPAD_DOWN = "dd"
DPAD_UP = "du"
DPAD_LEFT = "dl"
DPAF_RIGHT = "dr"
MAX_STICK = 32000

############################################################
#serial declartations
############################################################

ser = serial.Serial()
ser.port = "/dev/ttyACM0"
ser.baudrate = 115200
ser.bytesize = serial.EIGHTBITS
ser.parity = serial.PARITY_EVEN
ser.stopbits = serial.STOPBITS_ONE
ser.xonxoff = False
ser.open()

#try:
#	ser.open

#except Exception, e:
#	print "error opening port" + ser.port
#	exit()



def joystick_range(x):
	num = x	
	if num <= 2100:
		return 1
	elif num > 2100 and num <= 4200:
		return 2
	elif num > 4200 and num <=6300:
		return 3
	elif num > 6300 and num <= 8400:
		return 4
	elif num >8400 and num <= 10500:
		return 5
	elif num >10500 and num <= 12600:
		return 6
	elif num >12600 and num <= 14700:
		return 7
	elif num >14700 and num <= 16800:
		return 8
	elif num >16800 and num <= 18900:
		return 9
	elif num >18900 and num <= 33000:
		return 10

#
if ser.isOpen():
	print "serial is open"
	ser.flushInput()
	ser.flushOutput()
	time.sleep(0.5) # give the serial port sometime to receive the data
	while 1:
		for event in xbox_read.event_stream(deadzone=12000):
			#only register event of pushing the btn down
			if event.key == lights and event.old_value != 1:
				ser.write('w')

				print( "lights on")
			#will need to change this when integrate, currently just testing on test harness			
			if event.key == DPAD_UP and event.old_value != 1:
				ser.write('w')
				#ser.write(pack('cc',0xff))--not working yet
				print("camera change")
			if event.key == DPAD_DOWN and event.old_value != 1:
				ser.write('s')
				print("camera change")
			
			#if event.key == 
			print event.key		
			print event

