all:
	$(MAKE) -C src

install: all
	install -d build
	install -d build/temp
	install -d build/humi
	cp html/front.html build
	cp src/homecon build

clean:
	rm -rf build
	$(MAKE) -C src clean
