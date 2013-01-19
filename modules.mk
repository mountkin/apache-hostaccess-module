mod_hostaccess.la: mod_hostaccess.slo
	$(SH_LINK) -rpath $(libexecdir) -module -avoid-version  mod_hostaccess.lo
DISTCLEAN_TARGETS = modules.mk
shared =  mod_hostaccess.la
