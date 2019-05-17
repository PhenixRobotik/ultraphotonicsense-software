build:
	meson _build --cross-file stm32f303.meson
	ninja -C _build
