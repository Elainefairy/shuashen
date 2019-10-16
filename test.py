#!/usr/bin/python
# coding: utf-8
from flask import request,Flask,send_from_directory
import os
from werkzeug.utils import secure_filename
import pyzbar.pyzbar as pyzbar
from PIL import Image,ImageEnhance
import MySQLdb
app = Flask(__name__)

UPLOAD_DIR = '/root/upload'
cxn = MySQLdb.Connect(host = '127.0.0.1', user = 'root', passwd = '123456')
cur = cxn.cursor()
cur.execute("USE ginsheng")

@app.route('/')
def hello_world():
    return 'Hello World!'

@app.route("/upload",methods=['POST'])
def upload():
	if request.method=='POST':
		#qs=QssClient()
		#full_filr_lis=[]
		#print("files", request.files)
		#print("args:", request.args)
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
			cur.execute("INSERT INTO Shen VALUES(barcodeData,'','','','upload_path')")
			return barcodeData
		#return redirect(url_for('upload'))
		return "upload ok"
@app.route("/getfile/<filename>",methods=["GET"])
def getfile(filename):
	return send_from_directory(UPLOAD_DIR, filename)
if __name__=="__main__":
	app.run(host='127.0.0.1',port=8000,debug=True)


