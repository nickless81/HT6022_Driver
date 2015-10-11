
#Compilador
CC     = gcc

#Diretórios de busca
SHDIR   = -I Driver/Inc

#Flags
LDFLAGS   = -L /libusb-1.0
LDFLAGS  += -lusb-1.0 
LDFLAGS  += -lm 
#CFLAGS    = -ansi
CFLAGS   = -pedantic
CFLAGS   += -Wall

#Executável
EXEC = LoadFirmwareHT6022BL

# Arquivos fonte
SRC    = $(wildcard Usr/*.c)
SRC   += $(wildcard Driver/Src/*.c)

# Arquivos objeto
COBJ   = $(SRC:.c=.o) 

all: $(EXEC)

$(EXEC):$(COBJ)
	@echo linking
	@$(CC) $(COBJ) $(LDFLAGS) -o $(EXEC)

$(COBJ): %.o : %.c
	@echo Compiling $<
	@$(CC) $(CFLAGS) $(SHDIR) -c $< -o $@

clean:
	@echo removing files .o e .out
	@rm -f $(COBJ)
	@rm -f $(EXEC)
