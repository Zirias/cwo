cwo_VMAJ := 0
cwo_VMIN := 0
cwo_VREV := 1

ifeq ($(PLATFORM),posix)

cwo_VPRE :=
cwo_VSUFF := .$(cwo_VMAJ).$(cwo_VMIN).$(cwo_VREV)
cwo_VSS := .$(cwo_VMAJ)

endif

ifeq ($(PLATFORM),win32)

cwo_VPRE := -$(CWO_VMAJ)
cwo_VSUFF :=
cwo_VSS :=

endif

