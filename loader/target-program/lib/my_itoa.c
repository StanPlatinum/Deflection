char *my_itoa(unsigned long long val, char *buf, unsigned radix)
{
	char   *p;
	char   *firstdig;
	char   temp;
	unsigned   digval;
	p = buf;
	firstdig = p;
	do{
		digval = (unsigned)(val % radix);
		val /= radix;

		if  (digval > 9)
			*p++ = (char)(digval - 10 + 'a');
		else
			*p++ = (char)(digval + '0');
	}while(val > 0);
	*p-- = '\0 ';
	do{
		temp = *p;
		*p = *firstdig;
		*firstdig = temp;
		--p;
		++firstdig;
	}while(firstdig < p);
	return buf;
}
