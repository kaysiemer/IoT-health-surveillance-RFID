# python3.6

import random

from paho.mqtt import client as mqtt_client
class mqttDhbw:
    skip = 0
    broker = '81.169.194.117'
    port = 1883
    topic = [("dhbw/team5/value1",2),("dhbw/team5/value2",2),("dhbw/team5/value3",2),("dhbw/team5/value4",2),("dhbw/team5/value5",2),("dhbw/team5/value6",2),("dhbw/team5/value7",2)]
    client_id = 'raspi-mqtt'
    user = "student"
    password = "iotproject2§&d"
    
    def __init__(self):
        self.mqttClient = self.connect_mqtt()

    def connect_mqtt(self) -> mqtt_client:
        def on_connect(client, userdata, flags, rc):
            if rc == 0:
                print("Connected to DHBW MQTT Broker!")
            else:
                print("Failed to connect to DHBW MQTT B, return code %d\n", rc)
        client = mqtt_client.Client(self.client_id)
        client.username_pw_set(self.user, password=self.password)
        client.on_connect = on_connect
        client.connect(self.broker, self.port)
        print("Hello")
        return client

    def publish(self, co2, Personen, Gefaehrdungspotential, Fenster, Lufttemp, Luftfeuchtigkeit, Luftdruck):
        def on_publish(client, userdata, result):
            #print("data is published")
            return

        self.mqttClient.on_publish = on_publish
        self.mqttClient.publish("dhbw/team5/value1", co2)
        self.mqttClient.publish("dhbw/team5/value2", Personen)
        self.mqttClient.publish("dhbw/team5/value3", Gefaehrdungspotential)
        self.mqttClient.publish("dhbw/team5/value4", Fenster)
        self.mqttClient.publish("dhbw/team5/value5", Lufttemp)
        self.mqttClient.publish("dhbw/team5/value6", Luftfeuchtigkeit)
        self.mqttClient.publish("dhbw/team5/value7", Luftdruck)
