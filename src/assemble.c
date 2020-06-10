#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define MAX_LINE_LENGTH (512) //Assumption made according to the spec

/*
	Just trying to see if reading the file and
	writing random stuff out to binary works
*/

int main(int argc, char **argv) {
	char buffer[MAX_LINE_LENGTH];
	FILE* assembly_file = fopen(argv[1], "r");
	while (!feof(assembly_file)) {
		fgets(&buffer[0], sizeof(buffer), assembly_file);
		printf("%s", buffer);
	}
	FILE* fp;
	fp = fopen(argv[2], "wb");
	uint8_t test[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	for (int i = 0; i < 2; i++) {
		fwrite(&test[4 * i], sizeof(uint8_t), 4, fp);
	}
	fclose(fp);
}
