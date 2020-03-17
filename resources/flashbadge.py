#!/usr/bin/env python
import sys, serial,  time, subprocess

print "Programming...",
p=0
#p = subprocess.call(['st-flash', '--reset', 'write', '/Users/plum/STM32CubeIDE/workspace_1.0.1/BsidesSLC2020/Debug/BsidesSLC2020.bin', '0x8000000'])

if len(sys.argv) < 2:
	p= subprocess.call(['/Applications/STMicroelectronics/STM32Cube/STM32CubeProgrammer/STM32CubeProgrammer.app/Contents/MacOs/bin/STM32_Programmer_CLI', '-c', 'port=SWD', '-w', '/Users/plum/STM32CubeIDE/workspace_1.0.1/BsidesSLC2020/Debug/BsidesSLC2020.bin', '0x8000000', '-s'])

if p!=0:
	print("Failed! %d"%p)
	sys.exit(p)

print("done")

ser = serial.Serial("/dev/cu.usbserial-AC01A6RT", 115200, timeout=2)

for i in range(6):
	print ser.readline()

subprocess.call(['diskutil', 'umountDisk', '/dev/disk2'])
print "Erasing...",
ser.write('E')
print ser.readline()
ser.write('Y')
while not 'Erase complete' in ser.readline():
	time.sleep(1)
print "complete"
ser.write('W')
print "Flashing...",

p = subprocess.call(['dd', 'if=CaseFile.fat', 'of=/dev/disk2'])

if p!=0:
	print "Failed! %d"%p
	sys.exit(p)

ser.write('L')
time.sleep(1)
ser.write('R')
print "done! Rebooting"




