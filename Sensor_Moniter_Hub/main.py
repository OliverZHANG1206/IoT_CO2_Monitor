import time
import schedule
from MQTT_System import MQTT_service

# Global Variable
day_count = 0


def save(mqtt_service):
    global day_count
    for topic in mqtt_service.topic:
        table = topic.split("/")[2]
        filename = "SensorData_" + table + "_Day" + str(day_count) + ".csv"
        mqtt_service.storage.save_csv(filename=filename, table=table)
    day_count = day_count + 1


# main program
if __name__ == '__main__':
    mqtt_channel = MQTT_service()
    mqtt_channel.init()
    time.sleep(1)

    mqtt_channel.subscribe_sensor(topic="/Sensor_data/Sensor0")
    mqtt_channel.subscribe_command(topic="/Command")
    time.sleep(1)

    schedule.every().day.do(save, mqtt_service=mqtt_channel)

    while True:
        schedule.run_pending()
