# running '$ make' will compile scripts and prepare everything to be exported/installed
all: scripts

# directory where scripts will be placed
# may be modified
INSTALL_DIR = /usr/local/bin/

# directory where headers will be placed
# may be modified
EXPORT_DIR = /usr/local/include/

# default install option will place scripts in /usr/local/bin/
# and export headers according to the rule below
.PHONY: install
install: export
	if [ -d ./bin/ ]; then cp ./bin/* $(INSTALL_DIR); fi

# will uninstall (remove) all traces of sl2 as installed by the 'install' rule above
# will likely not remove everything correctly if e.g. scripts have been renamed manually,
# or in case of mismatch between INSTALL_DIR or EXPORT_DIR in case these have been manually altered
# (make sure values of INSTALL_DIR and EXPORT_DIR match for install and uninstall)
.PHONY: uninstall
uninstall:
	rm -f $(INSTALL_DIR)prep $(INSTALL_DIR)tm
	rm -rf $(EXPORT_DIR)sl2

# create directory /usr/local/include/sl2/
# and copy necessary files (headers)
.PHONY: export
export:
	rm -rf $(EXPORT_DIR)sl2/
	mkdir -p $(EXPORT_DIR)sl2/
	cp -r ./src/* $(EXPORT_DIR)sl2/
	cp -r ./template $(EXPORT_DIR)sl2/

# prepare scripts and export into a ./bin/ directory
.PHONY: scripts
scripts:
	rm -rf ./bin/
	mkdir -p ./bin/
	g++ -std=c++2a ./scripts/prep.cpp -o ./bin/prep -O2 -DEXPORT_DIR=$(EXPORT_DIR)
	g++ -std=c++2a ./scripts/tm.cpp -o ./bin/tm -O2 -DEXPORT_DIR=$(EXPORT_DIR)
