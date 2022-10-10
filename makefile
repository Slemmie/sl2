# install directory
# may be modified
INSTALL_DIR = /usr/local/include

# create directory /usr/local/include/sl2/
# and copy necessary files
.PHONY: export
export:
	rm -rf $(INSTALL_DIR)/sl2/
	mkdir -p $(INSTALL_DIR)/sl2/
	cp -r ./src/* $(INSTALL_DIR)/sl2/

# ready scripts and export into a ./bin/ directory for easy manual system wide export
.PHONY: scripts
scripts:
	rm -rf ./bin/
	mkdir -p ./bin/
	g++ -std=c++2a ./scripts/pre.cpp -o ./bin/pre -O2 -DINSTALL_DIR=$(INSTALL_DIR)
