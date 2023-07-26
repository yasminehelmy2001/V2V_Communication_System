import sys
import csv
from threading import Thread
from PyQt5.QtWidgets import QApplication
import telnetlib as tel
import json
import serial
import time
import tensorflow.compat.v1 as tf
import numpy as np
import pandas as pd
import sys
import csv
from threading import Thread
from PyQt5.QtWidgets import QApplication
import telnetlib as tel
import json
import serial
import time
import math

# Initialize an empty buffer to store the data
buffer = []
ser = serial.Serial('COM9', 4800)  # Change the port name and baud rate to match your setup
model = tf.keras.models.load_model(r'C:\Users\yasmi\OneDrive\Desktop\_best_model.h5')

attention = 0
meditation = 0
delta_power = 0
theta_power = 0
alpha_low_power=0
alpha_high_power=0
beta_low_power=0
beta_high_power=0
gamma_low_power=0
gamma_high_power=0
notworn=0
signal_quality=0

connection = tel.Telnet('127.0.0.1', 13854)  # connect to TGC local host
connection.write(b'{"eSense": true, "format": "Json"}')  # activate eSense value

freq_bands = {'delta': (0.5, 4),
              'theta': (4, 8),
              'low_alpha': (8, 10),
              'high_alpha': (10, 12),
              'low_beta': (12, 16),
              'high_beta': (16, 20),
              'low_gamma': (20, 30),
              'high_gamma': (30, 40)}

# Define a function to get the data
def get_data(connection):
    global attention, meditation, delta_power, theta_power, alpha_low_power,alpha_high_power
    global beta_low_power, beta_high_power, gamma_low_power, gamma_high_power, notworn, signal_quality, buffer
    line = connection.read_until(b'\r')
    while True:
        line = connection.read_until(b'\r')
        raw_str = (str(line).rstrip("\\r'").lstrip("b'"))
        data = json.loads(str(raw_str))

        if "eSense" in data.keys():  #if eSense found
            attention=int(data['eSense']["attention"])  #get attention
            meditation=int(data['eSense']['meditation'])  #get meditation
        if "eegPower" in data.keys():
            eeg_power = data['eegPower']
            delta_power = eeg_power['delta']
            theta_power = eeg_power['theta']
            alpha_low_power = eeg_power['lowAlpha']
            alpha_high_power = eeg_power['highAlpha']
            beta_low_power = eeg_power['lowBeta']
            beta_high_power = eeg_power['highBeta']
            gamma_low_power = eeg_power['lowGamma']
            gamma_high_power = eeg_power['highGamma']
        if "poorSignalLevel" in data.keys():
            signal_quality = int(data['poorSignalLevel'])
            if signal_quality == 200:
                notworn=1
                #print("Signal quality: Electrodes not contacting the skin")
            else:
                notworn=0
                #print("Signal quality: Excessive noise or interference or poor signal")
                            

        input_data = np.array([[attention, meditation, delta_power, theta_power, alpha_low_power,
                                alpha_high_power, beta_low_power, beta_high_power, gamma_low_power,
                                gamma_high_power]])

        
        input_data = pd.DataFrame(input_data, columns=['attention', 'meditation', 'delta_power', 'theta_power',
                                                       'alpha_low_power', 'alpha_high_power', 'beta_low_power',
                                                       'beta_high_power', 'gamma_low_power', 'gamma_high_power'])

        input_data.iloc[0, 0:10] = np.log2(input_data.iloc[0, 0:10] + 0.0001)  # log normalization for the first row

        md = np.median(input_data.iloc[0, 0:10])
        p75 = np.percentile(input_data.iloc[0, 0:10], 75)
        p25 = np.percentile(input_data.iloc[0, 0:10], 25)

        input_data.iloc[0, 0:10] = np.tanh((input_data.iloc[0, 0:10] - md) / (p75 - p25))
     
        input_data = input_data.values  # convert back to NumPy array
        
        buffer.append(input_data)
        
        if len(buffer) == 2:
            input_data = np.array(buffer)  # Convert the list to a numpy array
            buffer=[]
            input_data = input_data.reshape(1, 2, 10)
            #print(input_data)
            prediction = model.predict(input_data)
            #print(prediction)
       
            threshold = 0.9

            if notworn==1:
                print("Headset Not Worn")
                data = '1'  # string and add a newline character
                ser.write(data.encode('utf-8'))  # Encode the string using UTF-8 and send it

                
            elif prediction <= threshold:
                print("Drowsy")
                data = '1'  # string and add a newline character
                ser.write(data.encode('utf-8'))  # Encode the string using UTF-8 and send it

                
            elif prediction > threshold:
                print("Not Drowsy")
                data = '2'  # string and add a newline character
                ser.write(data.encode('utf-8'))  # Encode the string using UTF-8 and send it
         


get_data(connection)
