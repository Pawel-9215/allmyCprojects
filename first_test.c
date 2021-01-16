#include <stdio.h>

int main()
{
	char tekst1[20] = "Hello World!";
	int x = 12;
	int *test_pointer = &x;
	char *char_pointer = tekst1;
	
	struct point
	{
		int x, y;
	};
	
	struct point p1;
	
	p1.x = 12;
	p1.y = 16;
	
	printf("%s\n", tekst1);
	printf("\a\n");
	printf("adress: %p\n", test_pointer);
	printf("content: %d\n", *test_pointer);
	printf("char pointer adress: %p\n", char_pointer);
	printf("Char pointer content: %s\n", char_pointer);
	printf("I wonder what It'll print:\n%d\n%d\n", p1.x, p1.y);
	
	return 0;
}
