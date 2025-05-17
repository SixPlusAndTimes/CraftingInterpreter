BUILD_DIR := build


clox:
	$(MAKE) -f CPPMake.mk NAME=clox MODE=debug SOURCE_DIR=cpp SNIPPET=true
	@ cp build/clox clox
clean:
	 rm -rf $(BUILD_DIR)

.PHONY: clox