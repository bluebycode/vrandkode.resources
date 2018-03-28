#!/usr/bin/python
# -*- coding: utf-8 -*-
import MySQLdb as mdb
import psycopg2
import sys
import MySQLdb
con = MySQLdb.connect("127.0.0.1","root","toor","example" )
with con:
  print('connecting to database')
  cur = con.cursor()
  cur.execute("COPY incidents(id, category, description, dayoftheweek, time, district, resolution, address,x,y,location) FROM \'%s\' 
WITH DELIMITER=\'\\t\' and HEADER=false and DATETIMEFORMAT=\'%%Y-%%m-%%d %%H:%%M:%%S\';", (filename))
  cur.close()
  con.commit()
con.close()
