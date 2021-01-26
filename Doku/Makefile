
OUTPUT_DIR=build/

LATEX_COMPILER=/mnt/c/Program\ Files/MiKTeX\ 2.9/miktex/bin/x64/pdflatex.exe
COMPILE_OPTIONS = --interaction=nonstopmode --output-directory=$(OUTPUT_DIR) --quiet

SOURCE_FILE_EXT=.tex
OUTP_FILE_EXT=.pdf

all: main.pdf

#*$(OUTP_FILE_EXT) : *$(SOURCE_FILE_EXT)
#	$(LATEX_COMPILER) $(COMPILE_OPTIONS) $*$(SOURCE_FILE_EXT)

main.pdf : $(wildcard *.tex) $(wildcard */*.tex)
	$(LATEX_COMPILER) $(COMPILE_OPTIONS) main.tex
	mv $(OUTPUT_DIR)main.pdf main.pdf

.PHONY: clean
clean:
	rm -f main.pdf
	rm -f $(OUTPUT_DIR)main.aux
	rm -f $(OUTPUT_DIR)main.lof
	rm -f $(OUTPUT_DIR)main.lol
	rm -f $(OUTPUT_DIR)main.log
	rm -f $(OUTPUT_DIR)main.toc
