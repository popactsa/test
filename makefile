TARGET = main

.PHONY: all install clean print

CPP = g++

S_DIR = ./source

H_DIR = ./headers

O_DIR = ./object_files

D_DIR = ./data

.SILENT: print install $(O_DIR) $(D_DIR)

SRCS = $(shell ls $(S_DIR) | grep ".cpp") 

OBJS = $(SRCS:%.cpp=$(O_DIR)/%.o)

INC_FLAGS = $(addprefix -I,$(H_DIR))

CFLAGS = $(INC_FLAGS) -O0 -Wall -std=c++20 -Wno-narrowing -Wno-return-type

all: $(H_DIR) $(O_DIR) $(D_DIR) $(TARGET) 

$(TARGET): $(OBJS)
	$(CPP) $(OBJS) -o $@

$(O_DIR)/%.o: $(S_DIR)/%.cpp
	$(CPP) $(CFLAGS) -c $< -o $@

$(H_DIR):
	if [ ! -d "$(H_DIR)" ]; then \
		mkdir $(H_DIR); \
		echo "$(H_DIR) created"; \
	fi

$(O_DIR):
	if [ ! -d "$(O_DIR)" ]; then \
		mkdir $(O_DIR); \
		echo "$(O_DIR) created"; \
	fi

$(D_DIR):
	if [ ! -d "$(D_DIR)" ]; then \
		mkdir $(D_DIR); \
		echo "$(D_DIR) created"; \
	fi

clean:
	rm -rf $(O_DIR)/* $(TARGET)

print:
	echo $(SRCS)
