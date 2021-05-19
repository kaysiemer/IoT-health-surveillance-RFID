# python3.6

import random

from paho.mqtt import client as mqtt_client
class mqtt:
    skip = 0
    broker = '192.168.65.20'
    port = 8883
    topic = [("dhbw/team5/value1",2),("dhbw/team5/value2",2),("dhbw/team5/value3",2),("dhbw/team5/value4",2),("dhbw/team5/value5",2),("dhbw/team5/value6",2),("dhbw/team5/value7",2)]
    client_id = 'raspi-mqtt'
    tableRfid = "iot.table_sensor_rfid ( iot.table_sensor_rfid.sensor_id, iot.table_sensor_rfid.`timestamp`, iot.table_sensor_rfid.tag_id)"
    tableCo2 = "iot.table_sensor_co2 ( iot.table_sensor_co2.sensor_id,iot.table_sensor_co2.`timestamp`, iot.table_sensor_co2.`co2`)"
    tableHumidity = "iot.table_sensor_humidity ( iot.table_sensor_humidity.sensor_id,iot.table_sensor_humidity.`timestamp`, iot.table_sensor_humidity.humidity)"
    tableTemperature = "table_sensor_temperature(iot.table_sensor_temperature.sensor_id, iot.table_sensor_temperature.`timestamp`, iot.table_sensor_temperature.temperature)"
    tableAirPressure = "table_sensor_airpressure(iot.table_sensor_airpressure.sensor_id, iot.table_sensor_airpressure.`timestamp`, iot.table_sensor_airpressure.airpressur)"
    tableWindow = "table_sensor_window(iot.table_sensor_window.window_id, iot.table_sensor_window.`timestamp`, iot.table_sensor_window.window_status)"
    def __init__(self, sql):
        self.mqttClient = self.connect_mqtt()
        self.subscribe()
        self.mqttClient.loop_start()
        self.sqlServer = sql

    def connect_mqtt(self) -> mqtt_client:
        def on_connect(client, userdata, flags, rc):
            if rc == 0:
                print("Connected to RaspberryPi MQTT Broker!")
            else:
                print("Failed to connect to RaspberryPi MQTT Broker, return code %d\n", rc)

        client = mqtt_client.Client(self.client_id)
        client.on_connect = on_connect
        client.connect(self.broker, self.port)
        return client

    def subscribe(self):
        def on_message(client, userdata, msg):
            print(f"Received `{msg.payload.decode()}` on RaspberryPi from `{msg.topic}` topic and device {client}")
            if (msg.payload.decode() == "hello world" or self.skip == 0):
                self.skip = 1
                return
            else:
                if (msg.topic == "dhbw/team5/value1"):
                    values = "1, NOW(), {val}".format(val=msg.payload.decode())
                    self.sqlServer.insertData(self.tableCo2, values)
                    self.co2 = int(msg.payload.decode())
                elif (msg.topic == "dhbw/team5/value2"):
                    values = "1, NOW(), '{val}'".format(val=msg.payload.decode())
                    self.sqlServer.insertData(self.tableRfid, values)

                elif (msg.topic == "dhbw/team5/value4"):
                    values = "1, NOW(), {val}".format(val=msg.payload.decode())
                    self.sqlServer.insertData(self.tableWindow, values)

                elif (msg.topic == "dhbw/team5/value5"):
                    values = "1, NOW(), {val}".format(val=msg.payload.decode())
                    self.sqlServer.insertData(self.tableTemperature, values)

                elif (msg.topic == "dhbw/team5/value6"):
                    values = "1, NOW(), {val}".format(val=msg.payload.decode())
                    self.sqlServer.insertData(self.tableHumidity, values)

                elif (msg.topic == "dhbw/team5/value7"):
                    values = "1, NOW(), {val}".format(val=msg.payload.decode())
                    self.sqlServer.insertData(self.tableAirPressure, values)

                else:
                    print("Invalid Topic")

        self.mqttClient.subscribe(self.topic)
        self.mqttClient.on_message = on_message