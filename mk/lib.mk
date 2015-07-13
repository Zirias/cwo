LIBARCHIVES += $(LIBDIR)$(PSEP)lib$(T).a
LIBRARIES += $(LIBDIR)$(PSEP)$(LIBPRE)$(T)$($(T)_VPRE)$(LIBSUFF)$($(T)_VSUFF)

$(LIBDIR)$(PSEP)lib$(T).a: $($(T)_OBJS) | libdir
	$(VAR)
	$(VR)$(AR) rcs $@ $^

$(LIBDIR)$(PSEP)$(LIBPRE)$(T)$($(T)_VPRE)$(LIBSUFF)$($(T)_VSUFF): \
    $($(T)_SOBJS) | libdir
	$(VLD)
	$(VR)$(CC) -shared \
	    -Wl,-soname,$(LIBPRE)$(T)$($(T)_VPRE)$(LIBSUFF)$($(T)_VSS) \
	    -o$@ $(LDFLAGS) $^

$(P)%.d: $(P)%.c Makefile conf.mk
	$(VDEP)
	$(VR)$(CCDEP) -MT"$@ $(@:.d=.o) $(@:.d=_s.o)" -MF$@ \
	    $(CFLAGS) $(INCLUDES) $<

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),distclean)
-include $($(T)_OBJS:.o=.d)
endif
endif

$(P)%.o: $(P)%.c Makefile conf.mk
	$(VCC)
	$(VR)$(CC) -o$@ -c $(CFLAGS) $($(T)_DEFINES) $(INCLUDES) $<

$(P)%_s.o: $(P)%.c Makefile conf.mk
	$(VCC)
	$(VR)$(CC) -o$@ -c $(lib_CFLAGS) $($(T)_DEFINES) $(CFLAGS) \
		$(INCLUDES) $<

