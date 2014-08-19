CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES_SERVER=http_parser.cpp http_request_parse.cpp net.cpp http_request.cpp request_parser.cpp util.cpp request.cpp http_response.cpp callback.cpp pugixml.cpp Plist.cpp PlistDate.cpp  routing.cpp server_airplay.cpp
SOURCES_SERVER_SCREEN_MIRROR=http_parser.cpp http_request_parse.cpp net.cpp http_request.cpp request_parser.cpp util.cpp request.cpp http_response.cpp callback.cpp pugixml.cpp Plist.cpp PlistDate.cpp routing.cpp server_screen_mirroring.cpp 
INCLUDES=-I../include 
OBJECTS_SERVER=$(SOURCES_SERVER:.cpp=.o)
OBJECTS_SERVER_SCREEN_MIRROR=$(SOURCES_SERVER_SCREEN_MIRROR:.cpp=.o)
EXECUTABLE_SERVER=server
EXECUTABLE_SERVER_SCREEN_MIRROR=server_screen_mirroring

all: server server_screen_mirroring

server: $(SOURCES_SERVER) $(EXECUTABLE_SERVER)
server_screen_mirroring: $(SOURCES_SERVER_SCREEN_MIRROR) $(EXECUTABLE_SERVER_SCREEN_MIRROR)

$(EXECUTABLE_SERVER): $(OBJECTS_SERVER) 
	$(CC) $(LDFLAGS) $(OBJECTS_SERVER) -o $@

$(EXECUTABLE_SERVER_SCREEN_MIRROR): $(OBJECTS_SERVER_SCREEN_MIRROR) 
	$(CC) $(LDFLAGS) $(OBJECTS_SERVER_SCREEN_MIRROR) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< $(INCLUDES) -o $@

clean:
	rm -rf *o server server_screen_mirroring