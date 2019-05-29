from ubidots import ApiClient
import serial
import urlparse
import smtplib
import time
import csv
import RPi.GPIO as GPIO
import sys
import json
import MySQLdb as mdb
from datetime import date
#GPIO.setmode(GPIO.BOARD)
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(18,GPIO.OUT)
GPIO.setup(24,GPIO.OUT)
GPIO.setup(27,GPIO.IN)
#buzzer=25 
GPIO.setup(25,GPIO.OUT)
ser = serial.Serial("/dev/ttyS0",115200)
ser.baudrate=115200
current_state = 0
api = ApiClient(token='A1E-8iPxlRvzzA0dhZPUPg33iikQWjRNPb')
variable_h = api.get_variable('5cbe70e3c03f975ddc6371cc')
variable_t = api.get_variable('5cbe70d9c03f975ddc6371cb')
variable_s=api.get_variable('5cbe70eac03f975cfe918f27')
variable_motion=api.get_variable('5cbe7c29c03f97667daad378')
variable_led=api.get_variable('5cbe81b0c03f976b8d676ace')

con= mdb.connect('127.0.0.1','root','pewdipie','pfa');
try:
    while True:
            i1=GPIO.setup(18,GPIO.OUT)
            i=GPIO.input(27)
            time.sleep(1)
            current_state = GPIO.input(27)
            data=ser.readline()
            #time.sleep(1)
            data= ser.readline()
            #time.sleep(1)
            data = ser.readline()
            #time.sleep(1)
            pi = data.split("\t")
            temp=pi[0]
            hum=pi[1]
            sen=pi[2]
            set={}
            set[0]=pi[0]
            set[1]=pi[1]
            set[2]=pi[2]
            set[3]=current_state
            response_h = variable_h.save_value({"value": set[1]})
            response_t = variable_t.save_value({"value": set[0] })
            response_s = variable_s.save_value({"value": set[2]})
            last_value_t = variable_t.get_values(1)
            if (GPIO.output(18,GPIO.HIGH)):
                i1=1
                response_led=variable_led.get_values({"value":i1})
            else:
                response_led=variable_led.get_values({"value":i1})
            if (i==1) : 
                response_motion = variable_motion.save_value({"value": i})
                time.sleep(1)
            if(i):
                response_motion = variable_motion.save_value({"value": i})
                time.sleep(1)
            if last_value_t > 30 :
                GPIO.output(24,GPIO.HIGH)
                time.sleep(1)
                GPIO.output(25, GPIO.HIGH)
                time.sleep(1)
                GPIO.output(25, GPIO.LOW)
                GPIO.output(24,GPIO.LOW)
            with con:
                    for ob in set:
                            cursor=con.cursor()
                            cursor.execute("""INSERT INTO data VALUES(%s,%s,%s,%s)""" ,(set[0],set[1],set[2],set[3]))
                            time.sleep(1)
                    cursor.execute("""SELECT DISTINCT sensibility from data WHERE sensibility < 150 and detection=1""")
                    rows=cursor.fetchall()
                    for row in rows:        
                            if ((current_state == 1) & (row[0] < 150)):
                                    print ("motion detected and sensibility below 150 :open led")
                                    GPIO.output(18,GPIO.HIGH)
                                    time.sleep(1)
                                    GPIO.output(18,GPIO.LOW)     
                            elif ((current_state==1) & (row[0] > 200)):
                                    print ("motion detected and sensibility over 200 :led off")
                                    GPIO.output(18,GPIO.LOW)
                                    time.sleep(1)
                            elif ((current_state==0) & (row[0] > 200)):
                                    print ("no motion detected and sensibility over 200 :led off")
                                    GPIO.output(18,GPIO.LOW)
                                    time.sleep(1)
                            elif ((current_state==0) & (row[0] < 200)):
                                    print ("no motion detected and sensibility below 200 :led off")
                                    GPIO.output(18,GPIO.LOW)
                                    time.sleep(1)                            
                    con.commit()
                    cursor.close()

except KeyboardInterrupt:
    pass
finally:
    GPIO.cleanup()                           
