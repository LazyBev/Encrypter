#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/random.h>
#include <string.h>

int random_number(int min_num, int max_num) {
    int result = 0, low_num = 0, hi_num = 0;

    if (min_num < max_num) {
        low_num = min_num;
        hi_num = max_num + 1;
    } else {
        low_num = max_num + 1;
        hi_num = min_num;
    }

    srand(time(NULL));
    result = (rand() % (hi_num - low_num)) + low_num;
    return result;
}

int main(int argc, char *argv[]) {
    srand(time(NULL) + getpid());

    FILE *conf = fopen("config.txt", "r");
    if (conf == NULL) {
        perror("Failed to open config.txt");
        return 1;
    }

    char input_file[256];
    char output_file[256];

    if (fgets(input_file, sizeof(input_file), conf) == NULL) {
        perror("Failed to read input file name from config.txt");
        fclose(conf);
        return 1;
    }

    if (fgets(output_file, sizeof(output_file), conf) == NULL) {
        perror("Failed to read output file name from config.txt");
        fclose(conf);
        return 1;
    }

    // Remove newline character from the file names
    input_file[strcspn(input_file, "\n")] = '\0';
    output_file[strcspn(output_file, "\n")] = '\0';

    fclose(conf);

    FILE *in = fopen(input_file, "r");
    if (in == NULL) {
        perror("Failed to open input file");
        return 1;
    }

    FILE *out = fopen(output_file, "w");
    if (out == NULL) {
        perror("Failed to open output file");
        fclose(in);
        return 1;
    }

    int ch;
    int ent = (int)((rand() * (rand() / getpid())) * getpid() + (getpid() * getppid()));

    int i = 0;
    int randomiz = ((random_number(1, ent) * (getpid() % 2) * getpid()) + (++i + (ent - (i - (i / 2)))));

    while ((ch = fgetc(in)) != EOF) {
        fputc(ch + randomiz, out);
        randomiz = (i % getpid() == 1) ? ((random_number(1, ent)) * (getpid() % 2) + (++i + (ent - i))) + ++i : ((random_number(1, ent)) + (getpid() % 2) - (++i + (ent - (i * i)))) + --i;
        randomiz = (i % 2 == 0) ? ((random_number(1, ent)) / (getpid() % 2) + (++i + (ent - i))) + ++i : ((random_number(1, ent)) + (getpid() % 2) - (++i + (ent - (i * i)))) + --i;
        randomiz = (randomiz < 0) ? -randomiz : randomiz;
        randomiz *= (getpid() * random_number(getpid(), rand())) - getpid();
    }

    fclose(in);
    fclose(out);

    return 0;
}
