# Project:  health surveillance system
# Authors:  Nils Dedner / Timm Buchholz / Kay Siemer
# Date:     Last updated 30.04.2021
# Version:  V1.0
# Python 3.8.2 64-bit

# Module importieren
from sql.db import db
from mqtt.mqtt import mqtt
from mqtt.mqttDhbw import mqttDhbw
import RPi.GPIO as GPIO
import time
import datetime
import telepot
from emoji import emojize

# Telegram Bot verknüpfen
bot = telepot.Bot('1648829762:AAGYNOGRjbenji3j3Hm2gqyTh2w155xsahs')
bot.getMe()

# Timerfunktion initialisieren
timeNowTele = time.time()
timeNowDh = time.time()
timeNowRP = time.time()
timeNowFE = time.time()
timeNowLED = time.time()
timeNowTeleNote = time.time()
timeNowTeleError = time.time()
timeNowLEDError = time.time()

# Fehler-Flag initialisieren
ErrorFlag = 0

# Verbindung zur Datenbank herstellen
piSql1 = db()
piSql2 = db()

# MQTT Server und Subscriptions starten
piMqtt = mqtt(piSql1) 
dhbwMqtt = mqttDhbw()

# GPIO Pins einrichten
GPIO.setmode(GPIO.BCM) # GPIO Nummern statt Board Nummern
# GPIO Pins Relais
RELAIS_1_GPIO = 15	# GPIO 15 festgelegt
RELAIS_2_GPIO = 14	# GPIO 14 festgelegt
GPIO.setup(RELAIS_1_GPIO, GPIO.OUT) # GPIO-Modus zuweisen
GPIO.setup(RELAIS_2_GPIO, GPIO.OUT) # GPIO-Modus zuweisen
# GPIO Pins RGB-LED
redPin = 17     # GPIO 17 festgelegt
greenPin = 27   # GPIO 27 festgelegt
bluePin = 22    # GPIO 22 festgelegt
GPIO.setup(redPin, GPIO.OUT)    # GPIO-Modus zuweisen
GPIO.setup(greenPin, GPIO.OUT)  # GPIO-Modus zuweisen
GPIO.setup(bluePin, GPIO.OUT)   # GPIO-Modus zuweisen

# Initialisierung Gefaehrdungspotenzial & Raumgröße
Gefaehrdungspotenzial = 0
Raumgroesse = 35
FanEnabled = 0

while True:
    # Werte aus Datenbank auslesen & auf Variablen schreiben
    queryHazardPotential = "SELECT * FROM view_hazard_potential"
    answer = piSql2.getData(queryHazardPotential)
    #print(answer)
    CO2Gehalt, fAnzahlPersonen, Fensterstellung, Temperatur, Luftfeuchtigkeit, Luftdruck, LuefterManuell = answer[0][0], answer[0][1], answer[0][2], answer[0][3], answer[0][4], answer[0][5], answer[0][6]
    queryCo2Timestamp = "SELECT `timestamp` FROM table_sensor_co2 ORDER BY id DESC LIMIT 1;"
    answer = piSql2.getData(queryCo2Timestamp)
    CO2Timestamp = answer[0][0]
    print(CO2Timestamp)
    # print(fAnzahlPersonen)
    print(datetime.datetime.now()-datetime.datetime.timdelta(minutes= 3)
    # MQTT-Kommunikation überprüfen
    if (CO2Timestamp >(datetime.datetime.now() - 300)): # Letzte MQTT-Kommunikation länger als XY (?) Minuten her
        ErrorFlag = 1
    else:
        ErrorFlag = 0
    
    # Steuerung Lüfter: 
    # Luefter anschalten, wenn Fenster zu: Stufe 1 bei CO2-Gehalt zwischen 1.200 ppm und 1.600 ppm / Stufe 2 bei CO2-Gehalt größer 1.600 ppm / Lüfter deaktivieren bei CO2-Gehalt kleiner 800 ppm
    # Prüfung, ob Fehler in der MQTT-Kommunikation
    if (ErrorFlag == 1):
        GPIO.output(RELAIS_1_GPIO, GPIO.LOW) # Relais 1 an
        GPIO.output(RELAIS_2_GPIO, GPIO.LOW) # Relais 2 an
    # Ansonsten normaler Betrieb
    else:
        if ((CO2Gehalt >= 1600 and Fensterstellung == 0) or LuefterManuell == 1): 
            GPIO.output(RELAIS_1_GPIO, GPIO.LOW) # Relais 1 an
            GPIO.output(RELAIS_2_GPIO, GPIO.LOW) # Relais 2 an
            FanEnabled = 2
            print("Relais Stufe 2")
        elif (CO2Gehalt < 1600 and CO2Gehalt >= 1200 and Fensterstellung == 0):
            GPIO.output(RELAIS_1_GPIO, GPIO.LOW)  # Relais 1 an
            GPIO.output(RELAIS_2_GPIO, GPIO.HIGH) # Relais 2 aus
            print("Relais Stufe 1")
            FanEnabled = 1
        elif (CO2Gehalt < 900 or Fensterstellung == 1):
            GPIO.output(RELAIS_1_GPIO, GPIO.HIGH) # beide Relais aus
            FanEnabled = 0
            print("Relais aus")

    if (timeNowFE <= time.time()):
        fanStatusTable = "table_fan_status(`timestamp`, fan_status)"
        fanStatuslValues = "NOW(), '{val}'".format(val=Gefaehrdungspotenzial)
        piSql2.insertData(fanStatusTable, fanStatuslValues)
        timeNowFE = time.time() + 20
    # Berechnung des Gefaehrdungspotenzials:
    # Gefährdungspotenzial abhängig von CO2-Gehalt & Anzahl Personen im Raum
    # Prüfung, ob Fehler in der MQTT-Kommunikation
    if (ErrorFlag == 1):
    # LedFarbe = Magenta blinken
        if (timeNowLEDError <= time.time()):
            if (GPIO.input(redPin) == 0 and GPIO.input(bluePin) == 0):
                GPIO.output(redPin, GPIO.HIGH)
                GPIO.output(greenPin, GPIO.LOW)
                GPIO.output(bluePin, GPIO.HIGH)
            else:
                GPIO.output(redPin, GPIO.LOW)
                GPIO.output(greenPin, GPIO.LOW)
                GPIO.output(bluePin, GPIO.LOW)
            timeNowLEDError = time.time() + 2
    # Ansonsten normaler Betrieb
    else:
        if ((CO2Gehalt < 800) and (fAnzahlPersonen < (0.05 * Raumgroesse))):
            Gefaehrdungspotenzial = 1
            # LedFarbe = Gruen
            GPIO.output(redPin, GPIO.LOW)
            GPIO.output(greenPin, GPIO.HIGH)
            GPIO.output(bluePin, GPIO.LOW)

        if (((CO2Gehalt < 1200) and (CO2Gehalt >= 800)) or (((fAnzahlPersonen < (0.08 * Raumgroesse)) and (fAnzahlPersonen >= (0.05 * Raumgroesse))))):
            Gefaehrdungspotenzial = 2
            # LedFarbe = Gelb
            GPIO.output(redPin, GPIO.HIGH)
            GPIO.output(greenPin, GPIO.HIGH)
            GPIO.output(bluePin, GPIO.LOW)

        if (((CO2Gehalt < 1600) and (CO2Gehalt >= 1200)) or (((fAnzahlPersonen < (0.1 * Raumgroesse)) and (fAnzahlPersonen >= (0.08 * Raumgroesse))))):
            Gefaehrdungspotenzial = 3
            # LedFarbe = Orange / Magenta
            GPIO.output(redPin, GPIO.HIGH)
            GPIO.output(greenPin, GPIO.LOW)
            GPIO.output(bluePin, GPIO.HIGH)

        if (((CO2Gehalt < 2000) and (CO2Gehalt >= 1600)) or (((fAnzahlPersonen < (0.2 * Raumgroesse)) and (fAnzahlPersonen >= (0.1 * Raumgroesse))))):
            Gefaehrdungspotenzial = 4
            # LedFarbe = Rot
            GPIO.output(redPin, GPIO.HIGH)
            GPIO.output(greenPin, GPIO.LOW)
            GPIO.output(bluePin, GPIO.LOW)

        if ((CO2Gehalt >= 2000) or (fAnzahlPersonen >= (0.2 * Raumgroesse))):
            Gefaehrdungspotenzial = 5
            # LedFarbe = Rot blinken
            if (timeNowLED <= time.time()):
                if (GPIO.input(redPin) == 0):
                    GPIO.output(redPin, GPIO.HIGH)
                    GPIO.output(greenPin, GPIO.LOW)
                    GPIO.output(bluePin, GPIO.LOW)
                else:
                    GPIO.output(redPin, GPIO.LOW)
                    GPIO.output(greenPin, GPIO.LOW)
                    GPIO.output(bluePin, GPIO.LOW)
                timeNowLED = time.time() + 2

    print("Das Gefährdungspotential beträgt: {}".format(Gefaehrdungspotenzial))

    # Gefährdungspotential in Datenbank schreiben
    if (timeNowRP <= time.time()):
        riskPotentialTable = "iot.table_risk_potential ( iot.table_risk_potential.`timestamp`, iot.table_risk_potential.risk_potential)"
        riskPotentialValues = "NOW(), '{val}'".format(val=Gefaehrdungspotenzial)
        piSql2.insertData(riskPotentialTable, riskPotentialValues)
        timeNowRP = time.time() + 20
    
    # Sensorwerte & Gefährdungspotential per MQTT an DHBW-Server weiterleiten
    if (timeNowDh <= time.time()):
        dhbwMqtt.publish(CO2Gehalt, fAnzahlPersonen, Gefaehrdungspotenzial, Fensterstellung, Temperatur, Luftfeuchtigkeit, Luftdruck)
        timeNowDh = time.time() + 60

    # Ausgabe des Gefährdungspotenzials über Telegram:
    # Prüfung, ob Fehler in der MQTT-Kommunikation
    if (ErrorFlag == 1):
        if (timeNowTeleError <= time.time()):
            bot.sendMessage(-599483576, 'WARNUNG: Störung in der MQTT-Kommunikation des health surveillance systems! Es können keine zuverlässigen Aussagen über die Gefährungslage im überwachten Raum getroffen werden. Zu Ihrer Sicherheit wurde der Lüfter aktiviert.')
            timeNowTeleError = time.time() + 60
    # Ansonsten normaler Betrieb
    else:
        # Voraussetzung: Gefährdungspotenzial >= 3
        if ((Gefaehrdungspotenzial >= 3) and (timeNowTele <= time.time())):
            bot.sendMessage(-599483576, emojize(':police_car_light: Achtung: Das aktuelle Gefährungspotenzial beträgt: {}'.format(Gefaehrdungspotenzial), use_aliases=True))
            timeNowTele = time.time() + 60

    # Regelmäßiger Lüftungshinweise über Telegram
    if (timeNowTeleNote <= time.time()):
        bot.sendMessage(-599483576, 'Schützen Sie sich und Ihre Mitmenschen: Regelmäßig lüften, Abstand halten, Hygiene beachten, im Alltag Maske tragen und Corona-Warn-App nutzen!')
        timeNowTeleNote = time.time() + 900
    
    # Next steps:
    #   - Prüfung MQTT Kommunikation (Z. 68)
    #   - Steuerung Lüfter über Web-API
