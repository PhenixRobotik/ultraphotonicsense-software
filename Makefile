build: | _build
	ninja -C _build

_build:
	meson _build --cross-file stm32f303.meson

clean:
	rm -rf _build

flash: build
	ninja -C _build ups.flash

flash_tests: build
	ninja -C _build ups_tests.flash
