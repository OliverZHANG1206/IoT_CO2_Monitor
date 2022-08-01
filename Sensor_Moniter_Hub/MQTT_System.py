import re
import random
import paho.mqtt.client as mqtt
from SQLite_Database import Database


class MQTT_service:
    def __init__(self):
        self.broker = 'broker.emqx.io'
        self.port = 1883
        self.topic = []
        self.username = 'Yunfan'
        self.password = '123456'
        self.storage = Database()
        self.clint_id = f'python-mqtt-{random.randint(0, 10000)}'
        self.clint = mqtt.Client(self.clint_id)

    def init(self):
        self.clint.username_pw_set(self.username, self.password)
        self.clint.on_connect = self.on_connect
        self.clint.on_message = self.on_message
        self.clint.connect(self.broker, self.port)
        self.clint.loop_start()

    def subscribe_sensor(self, topic):
        self.storage.create_table(table=topic.split("/")[2])
        self.topic.append(topic)
        self.clint.subscribe(topic)

    def subscribe_command(self, topic):
        self.clint.subscribe(topic)

    def on_connect(self, client, userdata, flags, rc):
        if rc == 0:
            print('Connected to MQTT Broker!')
        else:
            print('Failed to connect, return code {:d}'.format(rc))

    def on_message(self, client, userdata, msg):
        print('Received {} from {} topic'.format(msg.payload.decode(), msg.topic))
        self.storage.update_data(table=msg.topic.split("/")[2], content=self.data_conv(message=msg.payload.decode()))

    @staticmethod
    def data_conv(message):
        expression = re.compile(r'\[(.*?)\]', re.S)
        raw_data = re.findall(expression, message)
        data = [raw_data[0].split()[1] + '-' + raw_data[0].split()[2] + '-' + raw_data[0].split()[4],
                raw_data[0].split()[3]]
        data.extend(raw_data[1].split("; "))
        data.append(raw_data[2])
        return data
