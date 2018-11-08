#maindir Makefile
@echo “main makelevel = $(MAKELEVEL)”
.PHONY : all
all:
	@$(shell cmake antsupporter/build)
	@$(MAKE) -C antsupporter/build

	@$(shell cmake app/build)
	@$(MAKE) -C app/build

.PHONY : clean
clean:
	@rm -r ./out/
