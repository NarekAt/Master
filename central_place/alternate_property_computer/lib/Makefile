TARGET=alternate_property_computer.exe
include ../common.mk

SUBDIRS=../exceptions ../graph_types ../common ../package ../main ../argument_parser ../mediator ../io ../property_counters ../randomizers ../task_managers
OBJS=$(wildcard $(join $(SUBDIRS), $(DIR)/*.o))

$(TARGET) : $(OBJS)
	@for dir in $(SUBDIRS); do \
		if [ -d "$$dir/$(DIR)" ]; then cp $$dir/$(DIR)/*.o ./; fi \
	done
	$(GCC) *.o $(CXXFLAGS) $(LFLAGS) -o $@

