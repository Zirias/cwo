include src$(PSEP)cwo$(PSEP)internal$(PSEP)internal.mk

P := src$(PSEP)cwo$(PSEP)
T := cwo

cwo_OBJS := $(cwo_OBJS) $(P)cwo.o $(P)object.o $(P)type.o $(P)string.o
cwo_SOURCES := $(cwo_OBJS:.o=.c)
cwo_SOBJS := $(cwo_OBJS:.o=_s.o)
cwo_DEFINES := -DBUILDING_CWO

SOURCES += $(cwo_SOURCES)
CLEAN += $(cwo_SOBJS)

$(eval $(LIBRULES))

