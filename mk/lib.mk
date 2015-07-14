define LIBRULES
LIBARCHIVES += $$(LIBDIR)$$(PSEP)$$(LIBPRE)$(T).a
LIBRARIES += $$(SODIR)$$(PSEP)$$(SOPRE)$(T)$$($(T)_VPRE)$$(SO)$$($(T)_VSUFF)

ifneq ($$(PLATFORM),win32)
$$(LIBDIR)$$(PSEP)$$(LIBPRE)$(T).a: $$($(T)_OBJS) | libdir
	$$(VAR)
	$$(VR)$$(AR) rcs $$@ $$^
endif

ifeq ($$(PLATFORM),win32)
$$(SODIR)$$(PSEP)$$(SOPRE)$(T)$$($(T)_VPRE)$$(SO)$$($(T)_VSUFF): \
    $$($(T)_SOBJS) | bindir libdir
	$$(VLD)
	$$(VR)$$(CC) -shared \
	    -Wl,--out-implib,$$(LIBDIR)$$(PSEP)$$(LIBPRE)$(T).a \
	    -Wl,--output-def,$$(LIBDIR)$$(PSEP)$$(SOPRE)$(T).def \
	    -o$$@ $$(LDFLAGS) $$^
else
$$(SODIR)$$(PSEP)$$(SOPRE)$(T)$$($(T)_VPRE)$$(SO)$$($(T)_VSUFF): \
    $$($(T)_SOBJS) | libdir
	$$(VLD)
	$$(VR)$$(CC) -shared \
	    -Wl,-soname,$$(SOPRE)$(T)$$($(T)_VPRE)$$(SO)$$($(T)_VSS) \
	    -o$$@ $$(LDFLAGS) $$^
endif

$(P)%.d: $(P)%.c Makefile conf.mk
	$$(VDEP)
	$$(VR)$$(CCDEP) -MT"$@ $(@:.d=.o) $(@:.d=_s.o)" -MF$$@ \
	    $$(CFLAGS) $$(INCLUDES) $$<

ifneq ($$(MAKECMDGOALS),clean)
ifneq ($$(MAKECMDGOALS),distclean)
-include $$($(T)_OBJS:.o=.d)
endif
endif

$(P)%.o: $(P)%.c Makefile conf.mk
	$$(VCC)
	$$(VR)$$(CC) -o$$@ -c $$(CFLAGS) $$($(T)_DEFINES) $$(INCLUDES) $$<

$(P)%_s.o: $(P)%.c Makefile conf.mk
	$$(VCC)
	$$(VR)$$(CC) -o$$@ -c $$(lib_CFLAGS) $$($(T)_DEFINES) $$(CFLAGS) \
		$$(INCLUDES) $$<
endef

