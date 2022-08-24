CC = aarch64-linux-gnu-g++

# CC = arm-linux-gnueabihf-gcc

INCY   := -I
SOURCE :=


CFLAGS :=
LDLAGS := 

INCY   += ./inc


SOURCE += ./src

CFLAGS += -O2
CFLAGS += -Wall
# CFLAGS += -D DEBUG_X3F_60FPS   #use x3f 60fps config
# CFLAGS += -D DEBUG_X3F_30FPS  #use x3f 30fps config

OBJS = $(wildcard $(SOURCE)/*.cpp)

TARGET = i2ctool


all:$(TARGET)
$(TARGET) : $(OBJS)
	$(CC) $^  $(INCY) -o $@ $(CFLAGS)  

.PHONY:clean
clean:
	@rm -rf $(TARGET)
