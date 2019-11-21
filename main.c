#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define DATE_SIZE (27)
#define DIRECTORY_SLOG ("C:\\Cst\\log.txt")
#define DIRECTORY_SLAST ("C:\\Cst\\last.txt")

void setlog(int);
void removelog();
void closelog(int);
int installtoregistry();
int uninstallfromregistry();
char *getdate();
void savedate();
char *getlast();
char *readlast();
void printlog();
int streq(const char *, const char *);

enum {
	log, last, loglegnth
};

FILE **sdlog;

int main(int varc, char **argv) {
	if (varc <= 1)
		return 0;
	if (streq(argv[1], "-h")) {
		printlog();
		return 0;
	}
	if (streq(argv[1], "-start")) {
		(installtoregistry()) ?
				printf("the tracker has started") : printf("failed to start");
		return 0;
	}
	if (streq(argv[1], "-stop")) {
		(uninstallfromregistry()) ?
				printf("the tracker has stopped") : printf("failed to stop");
		return 0;
	}
	if (streq(argv[1], "-c")) {
		printf("Are you sure you want to clear the log? (Y/N) ");
		char c = getchar();
		if (c != 'Y' && c != 'y')
			return 0;
		removelog();
		printf("history cleared\n");
		return 0;
	}
	int islast = streq(argv[1], "-l");
	setlog(islast);
	if (islast) {
		char *s = readlast();
		if (s != NULL) {
			printf("Last time this computer was started: %s", s);
			free(s);
		} else
			printf("No data found\n");
	} else if (streq(argv[1], "-r"))
		savedate();
	closelog(islast);
	return 0;
}

void setlog(int islast) {
	sdlog = malloc(sizeof(FILE *) * loglegnth);
	if (islast)
		return;
	sdlog[log] = fopen(DIRECTORY_SLOG, "a+");
	fseek(sdlog[log], 0, SEEK_SET);
	sdlog[last] = fopen(DIRECTORY_SLAST, "w");
}

void removelog() {
	remove(DIRECTORY_SLOG);
	remove(DIRECTORY_SLAST);
}

void closelog(int islast) {
	if (!islast) {
		int i;
		for (i = 0; i < loglegnth; ++i)
			fclose(sdlog[i]);
	}
	free(sdlog);
}

int installtoregistry() {
	HKEY hk;
	int status = 1;
	char *address = "\"C:\\Cst\\cst.exe\" -r";
	if (RegOpenKeyExA(HKEY_CURRENT_USER,
			"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0,
			KEY_ALL_ACCESS, &hk) != ERROR_SUCCESS) {
		return 0;
	}
	if (RegSetValueExA(hk, "Cst", 0, REG_SZ, (BYTE *) address,
			strlen(address)) != ERROR_SUCCESS) {
		status = 0;
	}
	RegCloseKey(hk);
	return status;
}

int uninstallfromregistry() {
	HKEY hk;
	int status = 1;
	if (RegOpenKeyExA(HKEY_CURRENT_USER,
			"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0,
			KEY_ALL_ACCESS, &hk) != ERROR_SUCCESS) {
		return 0;
	}
	if (RegDeleteValueA(hk, "Cst") != ERROR_SUCCESS) {
		status = 0;
	}
	RegCloseKey(hk);
	return status;
}

char *getdate() {
	time_t t = time(NULL);
	return ctime(&t);
}

void savedate() {
	char *l = getlast();
	if (l != NULL) {
		fprintf(sdlog[last], "%s", l);
		free(l);
	}
	fprintf(sdlog[log], "%s", getdate());
}

char *getlast() {
	int flag = 0;
	char *buf;
	buf = malloc(sizeof(char) * DATE_SIZE);
	while (fgets(buf, DATE_SIZE, sdlog[log]) != NULL)
		if (!flag)
			flag = 1;
	if (flag)
		return buf;
	free(buf);
	return NULL;
}

char *readlast() {
	sdlog[last] = fopen(DIRECTORY_SLAST, "r");
	char *buf;
	buf = malloc(sizeof(char) * DATE_SIZE);
	if (fgets(buf, DATE_SIZE, sdlog[last]) != NULL) {
		fclose(sdlog[last]);
		return buf;
	}
	sdlog[log] = fopen(DIRECTORY_SLOG, "r");
	int flag = 0;
	while (fgets(buf, DATE_SIZE, sdlog[log]) != NULL)
		if (!flag)
			flag = 1;
	int i;
	for (i = 0; i < loglegnth; ++i) {
		fclose(sdlog[i]);
	}
	if (flag)
		return buf;
	free(buf);
	return NULL;
}

void printlog() {
	FILE *l = fopen(DIRECTORY_SLOG, "r");
	char *d = malloc(sizeof(char) * DATE_SIZE);
	while (fgets(d, DATE_SIZE, l) != NULL)
		printf("%s", d);
	printf("END OF HISTORY\n");
	free(d);
	fclose(l);
}

int streq(const char *s1, const char *s2) {
	return strcmp(s1, s2) == 0;
}
