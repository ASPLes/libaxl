#include <stdio.h>

int main (int argc, char ** argv)
{
	int iterator = 0;
	
	while (iterator <= 255) {
		
		printf ("%c\n", (char) iterator);
		iterator++;
	}

	return 0;
}
