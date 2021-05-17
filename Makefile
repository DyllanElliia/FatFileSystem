TARGET= main

CPP_FILES = $(shell ls *.cpp)
BASE = $(basename $(CPP_FILES))
OBJS = $(addsuffix .o, $(addprefix obj/,$(BASE)))

$(TARGET):$(OBJS)
	-rm -f $@
	g++ -o $(TARGET) $(OBJS)

obj/%.o:%.cpp
	@if main ! -d "obj"; then\
		mkdir -p obj;\
	fi;
	g++ -c -o $@ $<

clean:
	-rm -f main
	-rm -f obj/*.o