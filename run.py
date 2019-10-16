#!/usr/bin/python
# coding: utf-8
from flask import request,Flask,send_from_directory
import os
from werkzeug.utils import secure_filename
import pyzbar.pyzbar as pyzbar
from PIL import Image,ImageEnhance
import pymysql
import json
from ctypes import *
app = Flask(__name__)

UPLOAD_DIR = '/var/www/html'
flag=True

def insert(ID,result,sale,buy):
	global flag
	conn=pymysql.Connect(host='127.0.0.1',user='root',passwd= '123456',db='ginsheng',port=3306)
	cursor= conn.cursor()
	sql="insert into Shen(ID,result,sale,buy) values(%s,%s,%s,%s)"
	try:
		if cursor.execute(sql,(ID,result,sale,buy))!=-1:
			flag=False
		conn.commit()
	except Exception as e:
		conn.rollback()
		print(e)
	cursor.close()
	conn.close()

def update(buy,ID):
	conn=pymysql.Connect(host='127.0.0.1',user='root',passwd= '123456',db='ginsheng',port=3306)
	cursor= conn.cursor()
	sql="update Shen set buy='%s' where ID='%s'"%(buy,ID)
	try:
		if cursor.execute(sql)!=-1:
			flag=False
		conn.commit()
	except Exception as e:
		conn.rollback()
		print(e)
	cursor.close()
	conn.close()

def select(ID):
	conn=pymysql.Connect(host='127.0.0.1',user='root',passwd= '123456',db='ginsheng',port=3306)
	cursor= conn.cursor()
	sql="select * from Shen where ID='%s'"%(ID)
	try:
		if cursor.execute(sql)!=-1:
			flag=False
		results=cursor.fetchall()
		for row in results:
			ID=row[0]
			result=row[1]
			sale=row[2]
			buy=row[3]
		return sale
	except Exception as e:
		print(e)
	cursor.close()
	conn.close()

def backfill(result,ID):
	conn=pymysql.Connect(host='127.0.0.1',user='root',passwd= '123456',db='ginsheng',port=3306)
	cursor= conn.cursor()
	sql="update Shen set result='%s' where ID='%s'"%(result,ID)
	try:
		if cursor.execute(sql)!=-1:
			flag=False
		conn.commit()
	except Exception as e:
		conn.rollback()
		print(e)
	cursor.close()
	conn.close()

@app.route('/')
def hello_world():
    return 'Hello World!'

@app.route("/upload2",methods=['POST'])
def upload2():
	if request.method=='POST':
		f=request.files['file']
		upload_path=os.path.join(UPLOAD_DIR, secure_filename(f.filename))
		f.save(upload_path)
		#print('save to ', upload_path)
		image = upload_path
		img = Image.open(image)
		barcodes = pyzbar.decode(img)
		for barcode in barcodes:
			barcodeData = barcode.data.decode("utf-8")
			#print(barcodeData)
			insert(barcodeData,'',f.filename,'')
			return barcodeData
		#return redirect(url_for('upload'))
		return "upload ok"

@app.route("/upload1",methods=['POST'])
def upload1():
	if request.method=='POST':
		f=request.files['file']
		upload_path=os.path.join(UPLOAD_DIR, secure_filename(f.filename))
		f.save(upload_path)
		#print('save to ', upload_path)
		image = upload_path
		img = Image.open(image)
		barcodes = pyzbar.decode(img)
		for barcode in barcodes:
			barcodeData = barcode.data.decode("utf-8")
			#print(barcodeData)
			update(f.filename,barcodeData)
			sale=select(barcodeData)
			sale_path=os.path.join(UPLOAD_DIR, sale)
			#print(sale_path)
			dll = cdll.LoadLibrary("./liblibshen.so")
			str1 = bytes(sale_path, 'utf-8')
			str2 = bytes(upload_path, 'utf-8')
			str3 = bytes("./3.jpg", 'utf-8')
			r=dll.pictureDifferentDection(str1,str2,str3,True,1,True)
			backfill(r,barcodeData)
			d=dict(result=r,address=sale)
			en_json=json.dumps(d)
			print(en_json)
			return en_json
			#return (barcodeData,sale)
		#return redirect(url_for('upload'))
		return "upload ok"
@app.route("/getfile/<filename>",methods=["GET"])
def getfile(filename):
	return send_from_directory(UPLOAD_DIR, filename)
if __name__=="__main__":
	app.run(host='127.0.0.1',port=8000,debug=True)