#!/usr/bin/python3

import time, os
from datetime import datetime
from flask import Flask, jsonify, request, Response
from elasticsearch import Elasticsearch

listen_port = 5001
es_host = "192.168.43.100"
es_port = 9200
es_index = "rawdata"
es_doc_type = "ts"

# Elasticsearch instance
es = Elasticsearch([{'host': es_host, 'port': es_port}])

# Flask instance
app = Flask(__name__)

# Receives packets data from sniffer
@app.route('/save', methods=['POST'])
def index_post():
    global listen_port, es_host, es_port
    if request.json: # if request body is json
        for body in request.json:
            print (body)
            # feeds
            result = es.index(index=es_index, doc_type=es_doc_type, body=body)

        return Response({"message": "OK"}, status=201, mimetype='application/json')
    return Response({"message": "Malformed request - not JSON data received"}, status=400, mimetype='application/json')

# Start listening
if __name__ == "__main__":
    app.run()
