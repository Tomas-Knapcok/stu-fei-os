#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int numbers() {
	int out[2];
	
	// create pipe
	pipe(out);

	//send numbers 2-35 to child through pipe
	if (!fork()) {
		for (int i = 2; i < 36; i++) {
			write(out[1], &i, sizeof(int));
		}
		close(out[1]);

		exit(0);
	}

	close(out[1]);

	return out[0];
}

int prime_numbers(int in, int prime) 
{
	int num;
	int out[2];

	pipe(out);

	//read numbers sent from parent through pipe
	//find if number is prime
	if (!fork()) {
		while (read(in, &num, sizeof(int))) {
			if (num % prime != 0) {
				write(out[1], &num, sizeof(int));
			}
		}
		close(in);
		close(out[1]);
		
		exit(0);
	}

	close(in);
	close(out[1]);

	return out[0];
}

int
main(int argc, char *argv[])
{
	int prime; 
	int num = numbers();

	//print number which is prime
	//call recursive function to find all primes 
	while (read(num, &prime, sizeof(int))) {
		printf("prime %d\n", prime); 
		num= prime_numbers(num, prime);
	}

	exit(0);
}