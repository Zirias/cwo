include mk$(PSEP)platform$(PSEP)$(PLATFORM)$(PSEP)lib.mk

define LIBRULES +=


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

