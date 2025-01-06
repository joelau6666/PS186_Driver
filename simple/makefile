export CC := $(CROSS_COMPILE)gcc

C_SRC := $(wildcard ./*.c)
C_SRC += $(wildcard ../*.c)

C_OBJECT := $(patsubst %.c, %.o, $(C_SRC))

%.o: %.c
	$(CC) -c $< -o $@ -I ../

test: $(C_OBJECT) 
	$(CC) $^ -o $@

echo:
	@echo $(CC)
	@echo $(C_SRC)
	@echo $(C_OBJECT)

.PHONY: echo
