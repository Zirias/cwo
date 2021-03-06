define TESTRULES
TESTS += $$(TESTDIR)$$(PSEP)$(T)$$(EXE)

$$(TESTDIR)$$(PSEP)$(T)$$(EXE): $$($(T)_OBJS) $$($(T)_LIBS) | testdir
	$$(VLD)
	$$(VR)$$(CC) -o$$@ $$(LDFLAGS) $$^

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
	$$(VR)$$(CC) -o$$@ -c $$(CFLAGS) $$($(T)_DEFINES) \
	    $$(INCLUDES) $$($(T)_INCLUDES) $$<
endef
