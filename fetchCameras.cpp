FILE *fp = NULL;
char data[100] = {'0'};
fp = popen("ls /dev/ttyUSB*", "r");
if (fp == NULL)
{
	printf("popen error!\n");
	return 1;
}
while (fgets(data, sizeof(data), fp) != NULL)
{
	printf("%s", data);
}
pclose(fp);
