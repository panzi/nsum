#include <stdio.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static long long int sum_int(long long int accumulate, FILE *fp, const char *filename) {
	for (;;) {
		long long int value = 0;
		int count = fscanf(fp, "%lld", &value);
		if (count < 1) {
			if (count == EOF && feof(fp)) {
				break;
			}
			perror(filename);
			exit(1);
		}

		accumulate += value;
	}

	return accumulate;
}

static double sum_float(double accumulate, FILE *fp, const char *filename) {
	for (;;) {
		double value = 0;
		int count = fscanf(fp, "%lf", &value);
		if (count < 1) {
			if (count == EOF && feof(fp)) {
				break;
			}
			perror(filename);
			exit(1);
		}

		accumulate += value;
	}

	return accumulate;
}

int main(int argc, char *argv[]) {
	static const struct option long_options[] = {
		{"help",  no_argument, 0, 'h'},
		{"int",   no_argument, 0, 'i'},
		{"float", no_argument, 0, 'f'},
		{0,       0,           0,  0 }
	};
	bool use_float = argc > 0 && strcmp(argv[0], "fsum") == 0 ? true : false;

	for (;;) {
		int opt = getopt_long(argc, argv, "hif", long_options, NULL);

		if (opt == -1)
			break;

		switch (opt) {
			case 'h':
				printf(
					"usage: %s [OPTIONS] [FILE...]\n"
					"\n"
					"OPTIONS:\n"
					"\t-h, --help     print this help message.\n"
					"\t-i, --int      parse numbers as integers (long long int).\n"
					"\t-f, --float    parse numbers as floating point (double).\n",
					argc > 0 ? argv[0] : "nsum");
				return 0;

			case 'i':
				use_float = false;
				break;

			case 'f':
				use_float = true;
				break;

			case '?':
				break;

			default:
				fprintf(stderr, "?? getopt returned character code %d ??\n", opt);
				return 1;
		}
	}

	if (use_float) {
		double accumulate = 0;
		if (optind < argc) {
			for (; optind < argc; ++ optind) {
				const char *filename = argv[optind];
				FILE *fp = fopen(filename, "r");
				if (!fp) {
					perror(filename);
					exit(1);
				}
				accumulate += sum_float(accumulate, fp, filename);
			}
		}
		else {
			accumulate += sum_float(accumulate, stdin, "<stdin>");
		}
		printf("%g\n", accumulate);
	} else {
		long long int accumulate = 0;
		if (optind < argc) {
			for (; optind < argc; ++ optind) {
				const char *filename = argv[optind];
				FILE *fp = fopen(filename, "r");
				if (!fp) {
					perror(filename);
					exit(1);
				}
				accumulate += sum_int(accumulate, fp, filename);
			}
		}
		else {
			accumulate += sum_int(accumulate, stdin, "<stdin>");
		}
		printf("%lld\n", accumulate);

	}
	return 0;
}
