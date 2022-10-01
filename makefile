# create directory /usr/local/include/sl2/
# and copy necessary files
.PHONY: export
export:
	rm -rf /usr/local/include/sl2/
	mkdir -p /usr/local/include/sl2/
	cp -r ./src/* /usr/local/include/sl2/
