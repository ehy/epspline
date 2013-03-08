#include <stdio.h>

int
main(int argc, char* argv[])
{
	int c;
	unsigned long n = 0;

	printf("\nchar %s[] = {\n", argc > 1 ? argv[1] : "chararray");
	while ( (c = getchar()) != EOF ) {
		printf("%c 0x%.02X", n++ ? ',' : ' ', c);
		if ( (n % 9) == 0 )
			putchar('\n');
	}
	puts("\n, 0x00\n};\n");

	return 0;
}

