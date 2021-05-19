import mysql.connector

class db:
    user = "root"
    password = "iot"
    host = "192.168.65.20"
    port = "3306"
    database = "iot"
    def __init__(self):
        self.dbServer = mysql.connector.connect(user= self.user, password= self.password, host= self.host, port= self.port, database= self.database)
    def insertData(self, table, values):
        cursor = self.dbServer.cursor()
        query = "INSERT INTO {table} VALUES({values})".format(table = table, values = values)
        #print(query)
        cursor.execute(query)
        self.dbServer.commit()
    def getData(self, query):
        cursor = self.dbServer.cursor()
        cursor.execute(query)
        temp = cursor.fetchall()
        self.dbServer.commit()
        return temp

