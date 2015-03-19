TARGET=AddInFF.so

SOURCES=config.cpp plugin.cpp

XUL_SDK=/usr/lib/xulrunner-1.9.2.3

LIB_DIRS=/usr/lib/xulrunner-1.9.2.3 ../../lib
LIBRARIES=nspr4 plds4 plc4 xpcom mozjs 1CEAdnWebFFLin32-1.9.2 

LIBS=$(addprefix -L, $(LIB_DIRS)) $(addprefix -l, $(LIBRARIES))
STATIC_LIB=/usr/lib/xulrunner-devel-1.9.2.3/sdk/lib/libxpcomglue_s.a

INCLUDES_DIRS=$(XUL_SDK)/include \
	$(XUL_SDK)/include/xpcom \
	$(XUL_SDK)/include/xpconnect \
	$(XUL_SDK)/include/js \
	$(XUL_SDK)/sdk/include \
	/usr/include/xulrunner-1.9.2.3 \
	/usr/include/nspr

INCLUDES=$(addprefix -I, $(INCLUDES_DIRS))

DEF_NAMES=XPCOM_GLUE MOZILLA_STRICT_API XPCOM_GLUE_USE_NSPR XP_UNIX
DEFINES=$(addprefix -D, $(DEF_NAMES))

OBJECTS=$(SOURCES:.cpp=.o)
CXXLAGS=$(CXXFLAGS) $(INCLUDES) -m32 -fshort-wchar -finput-charset=WINDOWS-1251 -fPIC -g -O2 $(DEFINES) 

%.o: %.cpp
	g++ -c  $(CXXLAGS) $*.cpp -o $*.o
	g++ -MM $(CXXLAGS) $*.cpp >  $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

all: $(TARGET)

$(TARGET): $(OBJECTS) Makefile
	g++ $(CXXFLAGS) -shared $(OBJECTS) -o $(TARGET) $(LIBS) $(STATIC_LIB)

clean:
	-rm $(TARGET) *.o *.d
