import csv
import sqlite3


class Database:
    def __init__(self):
        self.conn = sqlite3.connect("Data.db", check_same_thread=False)
        self.cur = self.conn.cursor()

    def create_table(self, table):
        cmd = "CREATE TABLE IF NOT EXISTS " + table + "\n" \
                                                      '''(ID           INTEGER  PRIMARY KEY AUTOINCREMENT,
                DATE         TEXT     NOT NULL,
                TIME         TEXT     NOT NULL,
                CO2          REAL     NOT NULL,
                TEMPERATURE  REAL     NOT NULL,
                HUMIDITY     REAL     NOT NULL,
                PIR_TRIGGER  INT      NOT NULL);'''
        self.cur.execute(cmd)
        print('Create Table ' + table + ' Successfully')

    def update_data(self, table, content):
        cmd = "INSERT INTO " + table + " (Date, Time, CO2, Temperature, Humidity, PIR_trigger) VALUES('" \
              + content[0] + "', '" + content[1] + "', " + content[2] + ", " \
              + content[3] + ", " + content[4] + ", " + content[5] + ");"
        self.cur.execute(cmd)
        self.conn.commit()
        print('Successfully upload to database')

    def save_csv(self, filename, table):
        cmd = "SELECT * FROM " + table
        self.cur.execute(cmd)
        with open(filename, mode="w", encoding="utf-8-sig", newline="") as file:
            writer = csv.writer(file)
            column_name = [tuple[0] for tuple in self.cur.description]
            writer.writerow(column_name)
            for row in self.cur.fetchall():
                writer.writerow(row)
            file.close()
