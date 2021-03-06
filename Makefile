##
##  Makefile -- Build procedure for sample hostaccess Apache module
##  Autogenerated via ``apxs -n hostaccess -g''.
##

builddir=.
top_srcdir=/usr/local/httpd-2.2.21
top_builddir=/usr/local/httpd-2.2.21
include /usr/local/httpd-2.2.21/build/special.mk

#   the used tools
APXS=apxs
APACHECTL=apachectl

#   additional defines, includes and libraries
#DEFS=-Dmy_define=my_value
#INCLUDES=-Imy/include/dir
#LIBS=-Lmy/lib/dir -lmylib

#   the default target
all: local-shared-build

#   install the shared object file into Apache 
install: install-modules-yes

#   cleanup
clean:
	-rm -f mod_hostaccess.o mod_hostaccess.lo mod_hostaccess.slo mod_hostaccess.la 

#   simple test
test: reload
	lynx -mime_header http://localhost/hostaccess

#   install and activate shared object by reloading Apache to
#   force a reload of the shared object file
reload: install restart

#   the general Apache start/restart/stop
#   procedures
start:
	$(APACHECTL) start
restart:
	$(APACHECTL) restart
stop:
	$(APACHECTL) stop

