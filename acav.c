#include<stdio.h>
/**
 * main - prints all arguments without using ac
 * @ac: number of arguments in av
 * @av: array of strings (arguments)
*/

int main(int ac, char ** av)
{
	int i = 0;

	printf("the argument count is %d\n", ac);
	printf("the second one is %s\n", av[0]);

	for (i = 0; i < ac; i++)
		printf("%d : %s\n", i+1, av[i]);
	
	return (0);
}
