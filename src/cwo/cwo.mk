include src$(PSEP)cwo$(PSEP)internal$(PSEP)internal.mk
include src$(PSEP)cwo$(PSEP)versions.mk

P:= src$(PSEP)cwo$(PSEP)

cwo_OBJS := $(cwo_OBJS) $(P)object.o $(P)type.o
cwo_SOURCES := $(cwo_OBJS:.o=.c)
cwo_SOBJS := $(cwo_OBJS:.o=_s.o)

SOURCES += $(cwo_SOURCES)
CLEAN += $(cwo_SOBJS)

T := cwo
include mk$(PSEP)lib.mk

