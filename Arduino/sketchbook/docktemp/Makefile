# My makefile

SKETCH = docktemp.ino
UPLOAD_PORT = /dev/tty.usbmodem1461
BOARD = d1_mini
CHIP = esp8266
CUSTOM_LIBS=$(ESPLIBS)/ESP2866Wifi \
	$(ARLIBS)/OneWire \
	$(ARLIBS)/DallasTemperature
include $(HOME)/Projects/sej/Arduino/makeEspArduino/makeEspArduino.mk
