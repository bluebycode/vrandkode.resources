#!/usr/bin/python3
from kamene.all import *
import socket
import json
import time
import requests
import _thread
import argparse
import os

filtered_macs = []
data_list = []
time_old = 0
parameters = ''

def enumeration(packet):
	"""
	Enumeration of the packet's layers
	"""
	yield packet.name
	while packet.payload:
		packet =packet.payload
		yield packet.name

# Function from https://stackoverflow.com/questions/5998245/get-current-time-in-milliseconds-in-python
# Epoch milliseconds calculator
epoch_millis = lambda: int(round(time.time() * 1000))


def send(data):
	print(data)
	global parameters
	try:
		headers = {'Content-type': 'application/json', 'Accept': 'text/plain'}
		resp = requests.post("http://127.0.0.1:5000/save", data=data, headers=headers)
		return resp.status_code
	except Exception as err:
		print(str(err))

def parser(packet):
	global data_list, time_old, parameters
	dictionary = {}

	# Timestamp identification for Grafana and ElasticSearch
	dictionary['@timestamp'] = epoch_millis()
	layers = ""

	# Agregation of all the packet's layers
	for i in enumeration(packet):
		if i == 'Raw':
			break
		if (layers == ""):
			layers = i
		layers += '@' + i
	dictionary['layers'] = layers

	# Agregation of specific packet's data
	try:

		dictionary['mac'] = getattr(packet.getlayer('Ethernet'),'src').replace(':','')
		if(packet.getlayer('IP')):
			dictionary['src'] = getattr(packet.getlayer('IP'),'src')
			dictionary['dst'] = getattr(packet.getlayer('IP'),'dst')
		if(packet.getlayer('TCP')):
			dictionary['seq'] = getattr(packet.getlayer('TCP'),'seq')
			dictionary['psrc'] = getattr(packet.getlayer('TCP'),'sport')
			dictionary['pdst'] = getattr(packet.getlayer('TCP'),'dport')
			if dictionary['psrc'] == 22 or dictionary['pdst'] == 22 or dictionary['psrc'] == 9200 or dictionary['pdst'] == 9200 or dictionary['psrc'] == 3000 or dictionary['pdst'] == 3000:
				return

	except Exception as e:
		print(str(e))

	# Json creation and data sending
	json_data = json.dumps(dictionary)
	time = epoch_millis()


        # Data agregation
	data_list.append(json_data)

	epsilon = 500
	#print(time - time_old)
	if (time_old == 0):
		time_old = time
	elif ((time - time_old) > int(2000+epsilon)):
		try:
			print(time - time_old)
			# Sending the data to the server API
			_thread.start_new_thread(send,(json.dumps(data_list),))
		except Exception as e:
			print(str(e))
		data_list = []
		time_old = 0


def main():
	filtered_macs=""#"00:50:56:c0:00:08,00:0c:29:a0:c6:f4"
	sniff(filter='ip',prn=parser, iface="wlan0" ,lfilter = lambda d: filtered_macs == "" or d.src.lower() in filtered_macs.lower().split(',') )


if __name__ == "__main__":
	main()
