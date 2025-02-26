export DIR = ${PWD}
ARCH := $(shell dpkg --print-architecture)
CODE_NAME := $(shell lsb_release -cs)
ifeq (${CODE_NAME},buster)
	SUB = "~bpo10+1"
else
	SUB = "~bpo11+1"
endif

all: tcltls piaware dump1090

tcltls:
	cd ${DIR}; \
	sudo \rm -rf tcltls-rebuild; \
	sudo apt-get install -y libdistro-info-perl; \
	git clone https://github.com/flightaware/tcltls-rebuild.git
	cd ${DIR}/tcltls-rebuild; \
	sudo ${DIR}/tcltls-rebuild/prepare-build.sh ${CODE_NAME}
	cd ${DIR}/tcltls-rebuild/package-${CODE_NAME}; \
	sudo dpkg-buildpackage -b --no-sign
	cd ${DIR}/tcltls-rebuild; \
	sudo dpkg -i tcl-tls_1.7.22-2+fa1_${ARCH}.deb

piaware:
	cd ${DIR}; \
	sudo \rm -rf piaware_builder; \
	git clone https://github.com/flightaware/piaware_builder.git
	cd ${DIR}/piaware_builder; \
	sudo ${DIR}/piaware_builder/sensible-build.sh ${CODE_NAME}
	cd ${DIR}/piaware_builder/package-${CODE_NAME}; \
	sudo dpkg-buildpackage -b --no-sign
	cd ${DIR}/piaware_builder; \
	sudo dpkg -i piaware_9.0.1${SUB}_${ARCH}.deb

dump1090:
	cd ${DIR}; \
	sudo \rm -rf dump1090-fa; \
	git clone https://github.com/flightaware/dump1090 dump1090-fa
	cd ${DIR}/dump1090-fa; \
	sudo ${DIR}/dump1090-fa/prepare-build.sh ${CODE_NAME}
	cd ${DIR}/dump1090-fa/package-${CODE_NAME}; \
	sudo dpkg-buildpackage -b --no-sign
	cd ${DIR}/dump1090-fa; \
	sudo dpkg -i dump1090-fa_9.0${SUB}_${ARCH}.deb
