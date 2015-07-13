P:= src$(PSEP)test$(PSEP)

testcwo_OBJS := $(P)testcwo.o
testcwo_SOURCES := $(testcwo_OBJS:.o=.c)
testcwo_SOBJS := $(testcwo_OBJS:.o=_s.o)

testcwo_LIBS = $(LIBDIR)$(PSEP)$(LIBPRE)cwo.a

SOURCES += $(testcwo_SOURCES)
CLEAN += $(testcwo_SOBJS)

T := testcwo
$(eval $(TESTRULES))
