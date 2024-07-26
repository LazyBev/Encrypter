#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/random.h>

int random_number(int min_num, int max_num)
    {
        int result = 0, low_num = 0, hi_num = 0;

        if (min_num < max_num)
        {
            low_num = min_num;
            hi_num = max_num + 1; // include max_num in output
        } else {
            low_num = max_num + 1; // include max_num in output
            hi_num = min_num;
        }

        srand(time(NULL));
        result = (rand() % (hi_num - low_num)) + low_num;
        return result;
    }

int main(int argc, char *argv[]) {
    FILE *in, *out;
    int ch;
    int ent = atoi(argv[3]);

    srand(time(NULL) + getpid());

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <input file> <output file> <integer>\n", argv[0]);
        return 1;
    }

    in = fopen(argv[1], "r");
    if (in == NULL) {
        perror("Error opening input file");
        return 1;
    }

    out = fopen(argv[2], "w");
    if (out == NULL) {
        perror("Error opening output file");
        fclose(in);
        return 1;
    }

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
