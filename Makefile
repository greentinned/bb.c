CC=gcc
CFLAGS=-Wall -g
TARGET=main
OBJECTS=$(TARGET).o
BUILD_PATH=build

main: $(OBJECTS)
	mkdir -p $(BUILD_PATH)
	$(CC) $(FLAGS) -o "$(BUILD_PATH)/$@" $(OBJECTS)
run: 
	make main
	./build/$(TARGET)

clean:
	rm $(OBJECTS)
	rm $(TARGET)
