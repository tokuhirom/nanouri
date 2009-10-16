RM=rm
NAME=nanouri
FIRST_MAKEFILE=Makefile
NOECHO=@
TRUE = true
NOOP = $(TRUE)
PERL = perl
VERSION = 
DISTVNAME = $(NAME)-$(VERSION)
PREOP = $(PERL) -I. "-MModule::Install::Admin" -e "dist_preop(q($(DISTVNAME)))"
TO_UNIX = $(NOECHO) $(NOOP)
TAR = tar
TARFLAGS = cvf
RM_RF = rm -rf
COMPRESS = gzip --best
POSTOP = $(NOECHO) $(NOOP)
DIST_DEFAULT = tardist
DIST_CP = best
PERLRUN = $(PERL)
TEST_VERBOSE=0
TEST_FILES=t/*.t

all: t/01_parse t/02_escape t/03_class

test: t/01_parse.t t/02_escape.t t/03_class.t
	PERL_DL_NONLAZY=1 $(PERLRUN) "-MExtUtils::Command::MM" "-e" "test_harness($(TEST_VERBOSE), 'inc')" $(TEST_FILES)

dist: $(DIST_DEFAULT) $(FIRST_MAKEFILE)

tardist: $(NAME).tar.gz
	$(NOECHO) $(NOOP)

$(NAME).tar.gz: distdir Makefile
	$(PREOP)
	$(TO_UNIX)
	$(TAR) $(TARFLAGS) $(DISTVNAME).tar $(DISTVNAME)
	$(RM_RF) $(DISTVNAME)
	$(COMPRESS) $(DISTVNAME).tar
	$(POSTOP)

distdir:
	$(RM_RF) $(DISTVNAME)
	$(PERLRUN) "-MExtUtils::Manifest=manicopy,maniread" \
	    -e "manicopy(maniread(),'$(DISTVNAME)', '$(DIST_CP)');"

clean:
	$(RM) t/01_parse t/02_escape t/03_class t/03_class.o t/01_parse.o t/02_escape.o
	$(RM) Makefile
	/bin/rm -f try try a.out .out try.[cho] try..o core core.try* try.core*

install: all
	

manifest:
	perl -MExtUtils::Manifest -e 'ExtUtils::Manifest::mkmanifest()'

t/01_parse: t/01_parse.o
	env MACOSX_DEPLOYMENT_TARGET=10.3 cc  -o t/01_parse t/01_parse.o  

t/01_parse.o: t/01_parse.c Makefile
	/usr/bin/gcc-4.2  -Wall -Wextra -I extlib -c -o t/01_parse.o t/01_parse.c

t/01_parse.o: t/01_parse.c t/../nanouri.h

t/01_parse.t: t/01_parse
	$(PERL) -e 'print "exec q{t/01_parse} or die $!"' > t/01_parse.t

t/02_escape: t/02_escape.o
	g++  -o t/02_escape t/02_escape.o  

t/02_escape.o: t/02_escape.cc Makefile
	g++  -Wall -Wextra -I extlib -c -o t/02_escape.o t/02_escape.cc

t/02_escape.o: t/02_escape.cc t/../nanouri.h

t/02_escape.t: t/02_escape
	$(PERL) -e 'print "exec q{t/02_escape} or die $!"' > t/02_escape.t

t/03_class: t/03_class.o
	g++  -o t/03_class t/03_class.o  

t/03_class.o: t/03_class.cc Makefile
	g++  -Wall -Wextra -I extlib -c -o t/03_class.o t/03_class.cc

t/03_class.o: t/03_class.cc t/../nanouri.h

t/03_class.t: t/03_class
	$(PERL) -e 'print "exec q{t/03_class} or die $!"' > t/03_class.t


