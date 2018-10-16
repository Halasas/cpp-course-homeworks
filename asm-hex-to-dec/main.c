#include <stdio.h>
#include <stdlib.h>

void __attribute__((__cdecl__)) hex_to_dec(char* out, char* hex);

char main() {
	char* hex = "-FFFFffffFFFFffEF";
	char* out = (char*)malloc(20 * sizeof(char));
	printf("hex: %s\n", hex);
	hex_to_dec(out, hex);
	printf("dec: ");
	for(char i = 0; i < 20; ++i)
		printf("%c", out[i]);
	printf("\n");
}
