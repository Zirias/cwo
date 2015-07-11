include src$(PSEP)cwo$(PSEP)internal$(PSEP)internal.mk

include src$(PSEP)cwo$(PSEP)versions.mk

P:= src$(PSEP)cwo$(PSEP)

cwo_OBJS := $(cwo_OBJS)
cwo_SOURCES := $(cwo_OBJS:.o=.c)
cwo_SOBJS := $(cwo_OBJS:.o=_s.o)

SOURCES += $(cwo_SOURCES)
CLEAN += $(cwo_SOBJS)

LIBARCHIVES += $(LIBDIR)$(PSEP)$(LIBPRE)cwo.a
LIBRARIES += $(LIBDIR)$(PSEP)$(LIBPRE)cwo$(cwo_VPRE)$(LIBSUFF)$(cwo_VSUFF)

$(LIBDIR)$(PSEP)$(LIBPRE)cwo.a: $(cwo_OBJS) | libdir
	$(VAR)
	$(VR)$(AR) rcs $@ $^

$(LIBDIR)$(PSEP)$(LIBPRE)cwo$(cwo_VPRE)$(LIBSUFF)$(cwo_VSUFF): \
    $(cwo_SOBJS) | libdir
	$(VLD)
	$(VR)$(CC) -shared \
	    -Wl,-soname,$(LIBPRE)cwo$(cwo_VPRE)$(LIBSUFF)$(cwo_VSS) \
	    -o$@ $(LDFLAGS) $^

T_OBJS := $(cwo_OBJS)
include mk$(PSEP)libobj.mk

