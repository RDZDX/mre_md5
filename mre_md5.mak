#makefile - mre_md5

CC=armcc
CXX=armcpp
CXXFLAGS=$(CFLAGS)
CFLAGS+= -W  -o0 -g -Ono-inline   -I .\include;.\include\service;.\include\component;.\ResID;.\\.\src\app\widget;.\\.\src\app\launcher;.\\.\src\app\wallpaper;.\\.\src\app\screen_lock;.\\.\include\service;.\\.\include\component;.\\.\include\service;.\\.\src\framework;.\\.\src\framework\ui_core\base;.\\.\src\framework\ui_core\mvc;.\\.\src\framework\ui_core\pme;.\\.\src\framework\mmi_core;.\\.\src\ui_engine\vrt\interface;.\\.\src\component;.\\.\src\ui_engine\framework\xml;.\

LD=$(CXX) $(CXXFLAGS)

LDFLAGS=

LDFLAGS+= 

LIBS+=-lodbc32 -lodbccp32 -lmrewin32 -lmsimg32

TARGET=mre_md5

.PHONY: all
all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $<

%.o: %.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $<

%.res: %.rc
	$(RC) $(CPPFLAGS) -o $@ -i $<

SOURCE_FILES= \

	mre_md5.c \

	mre_md5.def

HEADER_FILES= \

	mre_md5.h

RESOURCE_FILES= \

	./res/mre_md5.res.xml

SRCS=$(SOURCE_FILES) $(HEADER_FILES) $(RESOURCE_FILES) 

OBJS=$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.c %.cc %.cpp %.cxx ,$(SRCS))))))

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.PHONY: clean
clean:
	-rm -f  $(OBJS) $(TARGET)

