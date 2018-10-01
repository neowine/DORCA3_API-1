SRCS =   ./function/dorca.c ./function/test_main.c ./function/core_funtion.c ./function/api_fuction.c \
	./function/aes_core.c ./function/aes_ecb.c ./function/aes_misc.c ./function/cipher_memory.c ./function/register.c\
	./function/rsa_ecc_rand.c ./function/sha.c ./function/rootSerial.c ./function/permission.c Entry.c 
OBJS = $(SRCS:.c=.o)
CFLAGS = -g #-w
all: test
test: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ -lbcm2835  
	
.c.o: 
	@echo [Compile C] $<
	@$(CC) -c -o $@ $< $(CFLAGS)
clean:
	rm -rf *.o test
	cd function && rm -rf *.o 
	
