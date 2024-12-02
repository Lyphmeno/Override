int main(int argc, const char **argv, const char **envp)
{
	char s[100];
	unsigned int i;

	i = 0;
	fgets(s, 100, stdin);
	for ( i = 0; i < strlen(s); ++i )
	{
		if ( s[i] > 64 && s[i] <= 90 )
			s[i] ^= 0x20u;
	}
	printf(s);
	exit(0);
}
