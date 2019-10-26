// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <utils.h>

/* ------------------------------- */
/* FUNCIONES HELPER */

int concat(char * to, const char * from){
	int i = 0;
	while(from[i] != 0){
		to[i] = from[i];
		i++;
	}
	return i;
}

// intercambiar dos numeros
void swap(char *x, char *y) {
	char t = *x; *x = *y; *y = t;
}

// invertir el contenido de buffer
char* reverse(char *buffer, int i, int j) {
	while (i < j)
		swap(&buffer[i++], &buffer[j--]);

	return buffer;
}

int atoi(const char* buffer, int len) {
	int i = 0;
	int result = 0;

	while(buffer[i] != 0){
		result += (pow(10, --len) * (buffer[i] - 48));
		i++;
	}
	return result;
}

int pow(int base, int exponent) {
	int result = 1;
	for (int i = 0; i < exponent; i++){
		result = result * base;
	}
	return result;
}

// Implementacion iterativa de itoa
char* itoa(int value, char* buffer, int base) {
	// numero invalido
	if (base < 2 || base > 32)
		return buffer;

	// se considera el valor absoluto del numero
	int n = value;

	int i = 0;
	while (n)
	{
		int r = n % base;

			buffer[i++] = 48 + r;

		n = n / base;
	}

	// si el numero es 0
	if (i == 0)
		buffer[i++] = '0';

    //Si la base es 10 y el numero es negativo, se
    //lo precede con un signo menos. Para cualquier
    //otra base, el numero se asume positivo
	if (value < 0 && base == 10)
		buffer[i++] = '-';

	buffer[i] = '\0'; // terminar el string

    // invertir el string y devolverlo
	return reverse(buffer, 0, i - 1);
}

uint64_t strlen(const char * string) {
    uint64_t i = 0;
    while (string[i] != 0) i++;
    return i;
}

int strcmp (const char *t, const char *s) {
	while (*t==*s && *t!=0){
		t++;
		s++;
	}
	return *t-*s;
}

char* saveTwoDigit(int number, char * string) {
	if (number < 10) {
		itoa(0, string, 10);
		itoa(number, string + 1, 10);
	} else {
		itoa(number, string, 10);
	}
	return string;
}

int isNumber(char character){
	if (character >= '0' && character <= '9')
		return 1;
	return 0;
}

uint64_t getRGB(uint8_t r, uint8_t g, uint8_t b) {
	uint64_t aux = r;
	aux = (aux << 8) + g;
	return (aux << 8) + b;
}