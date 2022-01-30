ROOT_DIR := $(shell cd ../..; pwd)

VERSION := $(shell cat $(ROOT_DIR)/version)

WARNINGS := --warnings none
OPTIMIZE := --optimize-for-debug

PORT ?= /dev/ttyACM1

APPS := $(shell ls examples)

APP ?= multi_user

STM_PACKAGE := https://grumpyoldpizza.github.io/arduino-STM32L4/package_STM32L4_boards_index.json
CORE := grumpyoldpizza:stm32l4
BOARD := $(CORE):NUCLEO-L476RG

ifeq ($(findstring $(APP),$(APPS)),$(APP))
else
  $(error TARGET='$(APP)' is unknown)
endif

APP_DIR := "tmp/package/arduino_lapel/examples/$(APP)"
BUILD_DIR := "tmp/build/arduino_lapel/examples/$(APP)"
PACKAGE_DIR := "tmp/package"

compile: package
	arduino-cli compile -b $(BOARD) --output-dir $(BUILD_DIR) $(WARNINGS) $(OPTIMIZE) --libraries $(PACKAGE_DIR) $(APP_DIR)

flash: compile
	arduino-cli upload -b $(BOARD) --port $(PORT) --input-dir $(BUILD_DIR)

stlink_flash: compile
	STM32_Programmer_CLI --connect port=swd --write $(BUILD_DIR)/*.elf -hardRst

# flash arduino from factory boot
recover: compile
	arduino-cli upload -b $(BOARD) --port usb --input-dir $(BUILD_DIR)

package: $(PACKAGE_DIR)/arduino_lapel/library.properties $(PACKAGE_DIR)/arduino_lapel/NOTICE
	mkdir -p $(PACKAGE_DIR)/arduino_lapel/src/lapel
	cp -u $(ROOT_DIR)/LICENSE $(PACKAGE_DIR)/arduino_lapel
	cp -u $(ROOT_DIR)/src/* $(PACKAGE_DIR)/arduino_lapel/src/lapel
	cp -u $(ROOT_DIR)/include/* $(PACKAGE_DIR)/arduino_lapel/src/lapel
	cp -uR $(ROOT_DIR)/wrapper/arduino/src $(PACKAGE_DIR)/arduino_lapel
	cp -uR examples $(PACKAGE_DIR)/arduino_lapel
	cp -uR $(ROOT_DIR)/vendor/mbed_sha256 $(PACKAGE_DIR)/arduino_lapel/src
	echo "#define LAPEL_VERSION \"$(VERSION)\"" > $(PACKAGE_DIR)/arduino_lapel/src/version.h

$(PACKAGE_DIR)/arduino_lapel/library.properties:
	@ mkdir -p $(dir $@)
	@ echo "name=arduino_lapel" > $@
	@ echo "version=$(VERSION)" >> $@
	@ echo "author=Cameron Harper" >> $@
	@ echo "maintainer=Cameron Harper" >> $@
	@ echo "sentence=An interactive CLI shell for embedded applications." >> $@
	@ echo "paragraph=" >> $@
	@ echo "category=Communication" >> $@
	@ echo "url=https://github.com/cjhdev/lapel" >> $@
	@ echo "architectures=*" >> $@
	@ echo "includes=arduino_lapel.h" >> $@

$(PACKAGE_DIR)/arduino_lapel/NOTICE:
	@ mkdir -p $(dir $@)
	@ echo "This library combines original work with third party code." > $@
	@ echo "" >> $@
	@ echo "Third party code includes:" >> $@
	@ echo "" >> $@
	@ echo "- src/mbed_sha256: SHA256 from MBEDTLS, modified to suit this library" >> $@

release:
	rm -rf tmp/staging
	mkdir -p tmp/staging/arduino_lapel-$(VERSION)
	make clean
	make package
	cp -r $(PACKAGE_DIR)/arduino_lapel/* tmp/staging/arduino_lapel-$(VERSION)
	cd tmp/staging && zip -r arduino_lapel-$(VERSION).zip arduino_lapel-$(VERSION)

arduino-cli.yaml:
	@ echo "board_manager:" > $@
	@ echo "  additional_urls:"  >> $@
	@ echo "    - $(STM_PACKAGE)" >> $@

add_stm32: arduino-cli.yaml
	arduino-cli core update-index

install_stm32:
	arduino-cli core install $(CORE)

clean:
	rm -rf tmp/build
	rm -rf tmp/package
	rm -rf arduino-cli.yaml

.PHONY: package clean arduino-cli.yaml install_stm32
