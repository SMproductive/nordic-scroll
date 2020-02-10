#include <sys/wait.h>

#include <errno.h>
#include <inttypes.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <util.h>

void
die(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	if (fmt[0] && fmt[strlen(fmt)-1] == ':') {
		fputc(' ', stderr);
		perror(NULL);
	} else {
		fputc('\n', stderr);
	}

	exit(EXIT_FAILURE);
}

void
usage(void)
{
	fputs("ptty cmd\n", stderr);
	exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
	struct winsize ws = {.ws_row = 25, .ws_col = 80, 0, 0};
	int ch;

	while ((ch = getopt(argc, argv, "c:r:h")) != -1) {
		switch (ch) {
		case 'c':	/* cols */
			ws.ws_col = strtoimax(optarg, NULL, 10);
			if (errno != 0)
				die("strtoimax: %s", optarg);
			break;
		case 'r':	/* lines */
			ws.ws_row = strtoimax(optarg, NULL, 10);
			if (errno != 0)
				die("strtoimax: %s", optarg);
			break;
		case 'h':
		default:
			usage();
		}
	}
	argc -= optind;
	argv += optind;

	if (argc < 1)
		usage();

	int mfd;
	pid_t pid = forkpty(&mfd, NULL, NULL, &ws);
	switch (pid) {
	case -1:
		die("forkpty");
	case  0: /* child */
		execvp(argv[0], argv);
		die("exec");
	}

	/* parent */
	FILE *fh = fdopen(mfd, "rw");
	if (fh == NULL)
		die("fdopen");

	char buf[BUFSIZ];
	while (fgets(buf, sizeof buf, fh) != NULL);

	int status;
	waitpid(pid, &status, 0);

	return WEXITSTATUS(status);
}
