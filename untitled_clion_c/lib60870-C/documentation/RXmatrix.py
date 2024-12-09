
# Copyright (c) 2017 Adafruit Industries
# Author: Tony DiCola & James DeVito
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
import time

import Adafruit_GPIO.SPI as SPI
import Adafruit_SSD1306

from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont

import subprocess

# Raspberry Pi pin configuration:
RST = None     # on the PiOLED this pin isnt used
# Note the following are only used with SPI:
DC = 23
SPI_PORT = 0
SPI_DEVICE = 0

# Beaglebone Black pin configuration:
# RST = 'P9_12'
# Note the following are only used with SPI:
# DC = 'P9_15'
# SPI_PORT = 1
# SPI_DEVICE = 0

# 128x32 display with hardware I2C:
#disp = Adafruit_SSD1306.SSD1306_128_32(rst=RST)

# 128x64 display with hardware I2C:
disp = Adafruit_SSD1306.SSD1306_128_64(rst=RST)

# Note you can change the I2C address by passing an i2c_address parameter like:
# disp = Adafruit_SSD1306.SSD1306_128_64(rst=RST, i2c_address=0x3C)

# Alternatively you can specify an explicit I2C bus number, for example
# with the 128x32 display you would use:
# disp = Adafruit_SSD1306.SSD1306_128_32(rst=RST, i2c_bus=2)

# 128x32 display with hardware SPI:
# disp = Adafruit_SSD1306.SSD1306_128_32(rst=RST, dc=DC, spi=SPI.SpiDev(SPI_PORT, SPI_DEVICE, max_speed_hz=8000000))

# 128x64 display with hardware SPI:
# disp = Adafruit_SSD1306.SSD1306_128_64(rst=RST, dc=DC, spi=SPI.SpiDev(SPI_PORT, SPI_DEVICE, max_speed_hz=8000000))

# Alternatively you can specify a software SPI implementation by providing
# digital GPIO pin numbers for all the required display pins.  For example
# on a Raspberry Pi with the 128x32 display you might use:
# disp = Adafruit_SSD1306.SSD1306_128_32(rst=RST, dc=DC, sclk=18, din=25, cs=22)

# Initialize library.
disp.begin()

# Clear display.
disp.clear()
disp.display()

# Create blank image for drawing.
# Make sure to create image with mode '1' for 1-bit color.
width = disp.width
height = disp.height
image = Image.new('1', (width, height))

# Get drawing object to draw on image.
draw = ImageDraw.Draw(image)

# Draw a black filled box to clear the image.
draw.rectangle((0,0,width,height), outline=0, fill=0)

# Draw some shapes.
# First define some constants to allow easy resizing of shapes.
padding = -2
top = padding
bottom = height-padding
# Move left to right keeping track of the current x position for drawing shapes.
x = 0


# Load default font.
font = ImageFont.load_default()

# Alternatively load a TTF font.  Make sure the .ttf font file is in the same directory as the python script!
# Some other nice fonts to try: http://www.dafont.com/bitmap.php
#font = ImageFont.truetype('Minecraftia.ttf', 8)

#text="---"

while True:

    for i in range(0,1):
        # Draw a black filled box to clear the image.
        draw.rectangle((0,0,width,height), outline=0, fill=0)

        # Shell scripts for system monitoring from here : https://unix.stackexchange.com/questions/119126/command-to-display-memory-usage-disk-usage-and-cpu-load
        cmd = "hostname -I | cut -d\' \' -f1"
        IP = subprocess.check_output(cmd, shell = True )
        cmd = "top -bn1 | grep load | awk '{printf \"CPU Load: %.2f\", $(NF-2)}'"
        CPU = subprocess.check_output(cmd, shell = True )
        cmd = "free -m | awk 'NR==2{printf \"Mem: %s/%sMB %.2f%%\", $3,$2,$3*100/$2 }'"
        MemUsage = subprocess.check_output(cmd, shell = True )
        cmd = "df -h | awk '$NF==\"/\"{printf \"Disk: %d/%dGB %s\", $3,$2,$5}'"
        Disk = subprocess.check_output(cmd, shell = True )
        cmd = "ifstat -i eth0 1s 1 | awk 'NR==3 {print $1}'"
        TX = subprocess.check_output(cmd, shell = True )
        cmd = "ifstat -i eth0 1s 1 | awk 'NR==3 {print $2}'"
        RX = subprocess.check_output(cmd, shell=True)
        trafo = 3
        ochrana = 3

        draw.text((x, top),"Tabor 400kV " , font=font, fill=255)
        draw.text((x, top+9), "Trafo: " + str(trafo) + " Ochrana: " + str(ochrana), font=font, fill=255)
        draw.text((x, top+18),       "IP: " + str(IP),  font=font, fill=255)
        draw.text((x, top+27),     str(CPU), font=font, fill=255)
        draw.text((x, top+35),    str(MemUsage),  font=font, fill=255)
        draw.text((x, top+44),    str(Disk),  font=font, fill=255)
        draw.text((x, top+52),"TX: " + str(TX) + " RX: " + str(RX), font=font, fill=255)
        disp.image(image)
        disp.display()
        time.sleep(0.1)

    for i in range(0,100):
        f = open('RXmatrix.txt', 'r')
        m= [[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0]]
        s=0
        for line in f:
            y=line.split()
            m[s][0]="%.f" % float(y[0])
            #m[s][0]="%.f0" % int(y[2])
            m[s][1]="%.f" % float(y[1])
            #if int(m[s][0])==14 or int(m[s][0])==36:
            if int(m[s][0])==14 or int(m[s][0])==36:
                if float(y[2])==0:
                    m[s][2]="FAIL"
                else:
                    m[s][2]="%.2f" % float(y[2])
            else:
                m[s][2]="%.f" % float(y[2])
            s+=1

        #tf = open('text.txt', 'r')
        #text=tf.readline()


        draw.rectangle((0, 0, width, height), outline=0, fill=0)
        draw.text((x, top), "Client "+str(IP), font=font, fill=255)
        #draw.text((x, top + 9), text, font=font, fill=255)
        draw.text((x, top + 9), "RX matrix", font=font, fill=255)

        draw.text((x, top + 18), "(" +str( m[0][0]) + ")", font=font, fill=255)
        draw.text((x+27, top + 18), "io:"+str(m[0][1])+" val:"+str(m[0][2]), font=font, fill=255)

        draw.text((x, top + 27), "(" +str( m[1][0]) + ")", font=font, fill=255)
        draw.text((x+27, top + 27), "io:"+str(m[1][1])+" val:"+str(m[1][2]), font=font, fill=255)

        draw.text((x, top + 35), "(" +str( m[2][0]) + ")", font=font, fill=255)
        draw.text((x+27, top + 35), "io:"+str(m[2][1])+" val:"+str(m[2][2]), font=font, fill=255)

        draw.text((x, top + 44), "(" +str( m[3][0] )+ ")", font=font, fill=255)
        draw.text((x+27, top + 44), "io:"+str(m[3][1])+" val:"+str(m[3][2]), font=font, fill=255)

        draw.text((x, top + 52), "(" +str( m[4][0]) + ")", font=font, fill=255)
        draw.text((x+27, top + 52), "io:"+str(m[4][1])+" val:"+str(m[4][2]), font=font, fill=255)


        #draw.text((x, top + 18), "(" + m[0][0] + ") io:"+m[0][1]+" val:"+m[0][2], font=font, fill=255)
        #draw.text((x, top + 27), "(" + m[1][0] + ") io:"+m[1][1]+" val:"+m[1][2], font=font, fill=255)
        #draw.text((x, top + 35), "(" + m[2][0] + ") io:"+m[2][1]+" val:"+m[2][2], font=font, fill=255)
        #draw.text((x, top + 44), "(" + m[3][0] + ") io:"+m[3][1]+" val:"+m[3][2], font=font, fill=255)
        #draw.text((x, top + 52), "(" + m[4][0] + ") io:"+m[4][1]+" val:"+m[4][2], font=font, fill=255)
        disp.image(image)
        disp.display()
        time.sleep(0.5)

'''
    for i in range(0,1):
        draw.rectangle((0, 0, width, height), outline=0, fill=0)
        draw.text((x, top), "prvni radek", font=font, fill=255)
        draw.text((x, top + 9), "druhej radek", font=font, fill=255)
        draw.text((x, top + 18), "EXTERMINATE"+" EXTERMINATE", font=font, fill=255)
        draw.text((x, top + 27), "EXTERMINATE"+" EXTERMINATE", font=font, fill=255)
        draw.text((x, top + 35), "EXTERMINATE"+" EXTERMINATE", font=font, fill=255)
        draw.text((x, top + 44), "EXTERMINATE"+" EXTERMINATE", font=font, fill=255)
        draw.text((x, top + 52), "EXTERMINATE"+" EXTERMINATE", font=font, fill=255)
        disp.image(image)
        disp.display()
        time.sleep(0.5)'''


