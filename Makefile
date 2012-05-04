ARDUINO_DIR = /usr/share/arduino
TARGET = arduino-test3
BOARD_TAG = uno
ARDUINO_PORT = /dev/ttyACM0
PARSE_BOARD = $(ARDUINO_DIR)/ard-parse-boards --boards_txt=$(BOARDS_TXT)
ARDUINO_LIBS = LiquidCrystal Wire Wire/utility
include /usr/share/arduino/Arduino.mk