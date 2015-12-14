CD=cd
MKDIR=mkdir -p
RM=rm -fr
CMAKEARGS=
REALMAKE=make
ARGS=
ECLIPSEARGS=-G"Eclipse CDT4 - Unix Makefiles" -DECLIPSE_CDT4_GENERATE_SOURCE_PROJECT=TRUE

all: cmake realmake

cmake: .dirs
	$(CD) build && cmake $(CMAKEARGS) ..

.dirs: build bin

build:
	$(MKDIR) build

bin:
	$(MKDIR) bin

realmake:
	$(CD) build && $(REALMAKE) $(ARGS)

eclipse-project: .dirs
	$(CD) build && cmake $(ECLIPSEARGS) .. && $(REALMAKE) $(ARGS)

.docdir: docs

docs: doc
	$(MKDIR) docs

doc: .docdir
	doxygen Doxyfile

cleaner: clean
	$(RM) .project

clean:
	$(RM) build
	$(RM) bin
	$(RM) docs
