$(P)%.d: $(P)%.c Makefile conf.mk
	$(VDEP)
	$(VR)$(CCDEP) -MT"$@ $(@:.d=.o) $(@:.d=_s.o)" -MF$@ \
	    $(lib_CFLAGS) $(CFLAGS) $(INCLUDES) $<

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),distclean)
-include $(T_OBJS:.o=.d)
endif
endif

$(P)%.o: $(P)%.c Makefile conf.mk
	$(VCC)
	$(VR)$(CC) -o$@ -c $(CFLAGS) $(INCLUDES) $<

$(P)%_s.o: $(P)%.c Makefile conf.mk
	$(VCC)
	$(VR)$(CC) -o$@ -c $(lib_CFLAGS) $(CFLAGS) $(INCLUDES) $<

