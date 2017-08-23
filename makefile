PROJECT=tm
CFLAGS+= -O2 -Wall -Wextra
CFLAGS+= `pkg-config --cflags libnotifymm-1.0`
CFLAGS+= -I/usr/include/tclap/
LIBS+= `pkg-config --libs libnotifymm-1.0`

all : expiretimer.cpp
	g++  $(CFLAGS) $(LIBS) -o $(PROJECT) expiretimer.cpp

clean: 
	rm -f $(PROJECT)

install: $(PROJECT)
	echo "bla" 
.PHONY: install 
