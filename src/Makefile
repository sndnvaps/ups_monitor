CXX ?= g++
DESTDIR?=/usr
PREFIX?=/local

all: ups_monitor

ups_monitor:
	$(CXX) ina219.cpp -o ups_monitor -lwiringPi

.PHONY:	install
install: ups_monitor
	$Q echo "[Install]"
	$Q mkdir -p		$(DESTDIR)$(PREFIX)/bin
	$Q cp ups_monitor		$(DESTDIR)$(PREFIX)/bin

.PHONY: clean
clean:
	rm *.o ups_monitor
