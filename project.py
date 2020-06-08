# -*- coding: utf-8 -*-
from flask import Flask # Flask 서버 동작을 위한 Library 추가
import paho.mqtt.client as mqtt # MQTT Client Lib 추가
#import paho.mqtt.publish as publish # MQTT Publish Lib 추가
import time # Time delay 사용을 위한 Lib

app=Flask(__name__)

ip = "192.168.0.27" # 자신이 사용하는 Rapsberry Pi3의 IP 주소 입력

topic=''
message=''

#Raspberry Pi3의 MQTT Client/Publish 동시 동작을 위한 설정(접속)

################### routing ##################
@app.route('/iot/<num>/DHT22')
def temperature(num):
        client.subscribe("iot/"+str(num)+"/DHT22")
        #temp 명령어에 따른 temeprature subscribe
        time.sleep(1)
        client.unsubscribe("iot/"+str(num)+"/DHT22")
        return topic +' '+message #Web 상 temperature 표시

@app.route('/iot/<num>/CDS')
def light(num):
        client.subscribe("iot/"+str(num)+"/CDS")
        #light 명령어에 따른 조도센서값 subscribe
        time.sleep(1)
        client.unsubscribe("iot/"+str(num)+"/CDS")
        return topic+' '+message #Web 상 light Value 표시

@app.route('/iot/<num>/DOOR')
def door(num):
        client.subscribe("iot/"+str(num)+"/DOOR")
        time.sleep(1)
        client.unsubscribe("iot/"+str(num)+"/DOOR")
        return topic+' '+message

ledCommand = {'led', 'ledon', 'ledoff'}

#LED Control part
@app.route('/iot/<num>/<state>')
def indiv(num, state):
        if state in ledCommand:
                client.publish("iot/1", state)
                #ledControl(state, int(num))
                str = state2str(state)
                return num + str
        else:
                return "Wrong URL"
################### Function defition #################
def on_message(client, userdata, msg): #nodeMCU로 부터 송신된 message를 받아 변$
        global topic
        global message
        print(msg.topic+" "+str(msg.payload))
        topic=msg.topic
        message=str(msg.payload)

def state2str(state):
        if (state == 'led'):
                return ' LED Toggle'
        elif (state == 'ledon'):
                return ' LED ON'
        elif (state == 'ledoff'):
                return ' LED OFF'

def ledControl(*args): #LED 제어를 위한 함수 배열을 파라미터로 받는다
        if len(args) == 1: #All LED toggle
                client.publish("iot/all/", args[0])
                return

        elif len(args) == 2: #개별 LED toggle
                client.publish("iot/"+str(args[1]), args[0])
                return

        else:
                return

if __name__=="__main__":
        #mqtt 클라이언트 설정
        client = mqtt.Client()
        client.connect(ip, 1883, 60)
        client.loop_start()
        client.on_message=on_message
        app.run(host="0.0.0.0", port=8000, debug=True)







