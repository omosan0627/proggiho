#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static void usage(void);
static int caesar(FILE *in, FILE *out);
extern int main(int argc, char *argv[]);

static void usage() {
	puts("usage: caesar [input.txt] [output.txt]\n"
		"	specify `-' for stdin\n"
		"	omit output for stdout");
	exit(EXIT_FAILURE);
}

static int caesar(FILE *in, FILE *out) {
	char b;
	while(fread(&b,1,1,in)) {
		if(isalnum(b)) {
			b++;
		}
		if(!fwrite(&b,1,1,out)) {
			return EXIT_FAILURE;
		}
	}
	return feof(in)?EXIT_SUCCESS:EXIT_FAILURE;
}

extern int main(int argc, char *argv[]) {
	FILE *in=stdin, *out=stdout;
	int r;

	if(argc>1) {
		if(!strcmp(argv[1],"-")) {
			in=stdin;
		} else if(!(in=fopen(argv[1],"r"))) {
			usage();
		}
	}
	if(argc>2) {
		if(!(out=fopen(argv[2],"w"))) {
			fclose(in);
			usage();
		}
	}

	r=caesar(in,out);

	fclose(out);
	fclose(in);
	return r;
}
