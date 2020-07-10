COMPILER=gcc
OUTPUT_FILE_BUE=arbolesADTBUE
OUTPUT_FILE_VAN=arbolesADTVAN
FILES=main.c csv.c queryDataADT.c
CFLAGS=-pedantic -std=c99 -Wall -fsanitize=address
DEBUG_COMPILER=-g -DDEBUG=1

all: arbolesADTBUE arbolesADTVAN

arbolesADTBUE:
	$(COMPILER) $(CFLAGS) -DBUE -o $(OUTPUT_FILE_BUE) $(FILES)

arbolesADTVAN:
	$(COMPILER) $(CFLAGS) -DVAN -o $(OUTPUT_FILE_VAN) $(FILES)

debug: CFLAGS+= $(DEBUG_COMPILER)
debug: all

clean:
	rm -rf $(OUTPUT_FILE_BUE) $(OUTPUT_FILE_VAN) *.o query*.csv
