#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cs542.h"
#define NDIGITS 16

static const char *menu = "1-Load File\n2-Build Routing Table for Each Router\n3-Out Optimal Path and Minimum Cost\n0-Exit";
static const char *prompt0 = "Invalid input. Please try again. Only character '1', '2', or '3' is allowed.";
static const char *prompt1 = "Please load original routing table data file:";
static const char *prompt2 = "Please select a router:";
static const char *prompt3 = "Please input the source and destination router number:";

static int readNat(int *p) {
	static char buf[NDIGITS];
	static int r = 0;

	if (fgets(buf, NDIGITS, stdin) == NULL)
		return -1;
	if ((r = atoi(buf)) < 0)
		return -1;
	if (r == 0 && *buf != '0')
		return -1;
	
	return (*p = r);
}

static char *readFilename() {
	static char buf[BUFSIZ];

	int i;
	char c;
	for (i = 0; i < BUFSIZ && ((c = (char)fgetc(stdin))!='\n' && c!='\0' && c!=EOF); i++) {
		*(buf+i) = c;
	}
	if (i < BUFSIZ-1) {
		*(buf+i) = '\0';
	} else {
		printf("Input buffer overflows by the file_name.\n");
		return NULL;
	}

	return buf;
}

static int read2Nats(int *p) {
	static char buf[2*NDIGITS];
	static int a = 0;
	static int b = 0;

	if (fgets(buf, 2*NDIGITS, stdin) == NULL)
		return -1;
	char *sp = buf;
	char *endp = buf;
	if ((a = (int)strtol(sp, &endp, 10)) == 0)
		return -1;
	sp = endp;
	if ((b = (int)strtol(sp, &endp, 10)) == 0)
		return -1;
	// both a and b are non-zero
	if (a < 0 || b < 0)
		return -1;
	// both a and b are positive
	*p++ = a;
	*p = b;
	
	return 0;
}

static int getaline(char *line, int max, FILE *in) {
	if (fgets(line, max, in) == NULL)
		return 0;
	else
		return strlen(line);
}

static int linetoints(char *sp, int *ip) {
	char *endp;
	int i;
	for (i = 0; i < BUFSIZ; i++, sp=endp, ip++) {
		if ((*ip = (int)strtol(sp, &endp, 10))==0 && endp==sp) {//nothing is read
			//printf("i: %d sp: %s endp: %s\n", i, sp, endp);
			goto END;
		}
		//printf("%d ", *ip);
	}
END:
	return i;
}

static int intcpy(int *dest, const int *src, int n) {
	int i;
	for (i = 0; i < n; i++) {
		*dest++ = *src++;
	}
	return i;
}

int main() {
	char 	line[BUFSIZ];
	int 	row[BUFSIZ];

	int 	opt = 0;
	FILE*	file= NULL;
	//char*	fn 	= NULL;
	int 	ncol= 0;
	int 	tmp = 0;
	graph*	g 	= NULL;
	int*	ap 	= NULL;	// pointer within g->adjmtx
	router*	r 	= NULL;
	int 	fromto[2] = { 0, 0 };
	for (;;) {
		printf("\n%s\n", menu);
		switch (readNat(&opt)) {
		case 0:
			freegraph(&g);
			goto END;
		case 1:
			printf("%s\n", prompt1);
			//fn = readFilename();
			if ((file = fopen(readFilename(), "r")) == NULL) {
				printf("File not found.\n");
				break;
			}
			// file is opened
			while (getaline(line, BUFSIZ, file)) {//printf("while ...\n");
				tmp = 0;
				if ((tmp = linetoints(line, row)) > 0) {
					if (ncol == 0) {// 1st row
						ncol = tmp;
						if (g != NULL)
							freegraph(&g);	// g == NULL
						g = makegraph(ncol);
						ap = g->adjmtx + intcpy(g->adjmtx, row, ncol);
					} else if (ncol == tmp) {
						ap += intcpy(ap, row, ncol);
					} else {
						printf("File content invalid: different number of elements on different rows.\n");
						break;
					}
				}
			}
			ncol = 0;
			fclose(file);
			//if (isvalidgraph(g)) {// graph is created
				printf("Original routing table is as follows:\n");
				printgraph(g);
			/*} else {
				freegraph(&g);
				printf("File content invalid: elements on upper-left to bottom-right diagonal are not all-zero, ");
				printf("or, the upper-right half of the matrix and the bottom-left half are not symmetric.\n");
			}*/
			
			break;
		case 2:
			if (g == NULL) {
				printf("There is no data currently.\nPlease use option 1 to read a routing table data file first.\n");
				break;
			}
			printf("%s\n", prompt2);
			if (readNat(&tmp) < 0 || tmp > g->size) {
				printf("Not a valid router id.\n");
				break;
			}
			r = makerouter(tmp, g->size);
			buildtable(r, g);
			printf("The routing table for router %d is:\n", r->id);
			printrouter(r);
			freerouter(r);
			break;
		case 3:
			if (g == NULL) {
				printf("There is no data currently.\nPlease use option 1 to read a routing table data file first.\n");
				break;
			}
			printf("%s\n", prompt3);
			if (read2Nats(fromto) < 0 || fromto[0] > g->size || fromto[1] > g->size) {
				printf("Invalid input. Please follow the format:\n\tvalid_src_router_id valid_dest_router_id.\n");
				break;
			}
			// fromto[2] is valid
			r = makerouter(fromto[0], g->size);
			dijkstra(g, r->id, r->varray);
			printpath(r, fromto[1]);
			freerouter(r);
			break;
		default:
			printf("%s\n", prompt0);
			break;
		}
	}
END:
	return 0;
}