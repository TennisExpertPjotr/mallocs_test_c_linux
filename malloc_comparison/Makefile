.PHONY: all clean

APP=tmall
CFLAGS=-Wall -Wextra -Werror -g -ldl

all: $(APP)

$(APP): $(APP).c
	gcc -o $(APP) $(CFLAGS) $(APP).c

clean:
	rm $(APP)
