BMP_GDB_DEV := /dev/ttyACM0
BMP_UART_DEB := /dev/ttyACM1

%.flash: %.hex
	$(GDB) $^ \
		-ex "target extended-remote $(BMP_GDB_DEV)" \
		-ex "mon s" \
		-ex "attach 1" \
		-ex "load" \
		-ex "quit"

%.debug: %.elf
	$(GDB) $^ \
		-ex "target extended-remote $(BMP_GDB_DEV)" \
		-ex "mon s" \
		-ex "attach 1" \
		-ex "load"
	