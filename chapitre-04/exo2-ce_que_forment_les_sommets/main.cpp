#include <cstdio>
#include <cstring>

static int compte(const char *type, int s, int &restants, const char *&unite) {
    restants = 0;
    unite = "";
    if (strcmp(type, "POINTS") == 0) {
        unite = "POINTS";
        return s;
    }
    if (strcmp(type, "LINES") == 0) {
        unite = "SEGMENTS";
        restants = s % 2;
        return s / 2;
    }
    if (strcmp(type, "LINE_STRIP") == 0) {
        unite = "SEGMENTS";
        if (s >= 2) return s - 1;
        restants = s;
        return 0;
    }
    if (strcmp(type, "TRIANGLES") == 0) {
        unite = "TRIANGLES";
        restants = s % 3;
        return s / 3;
    }
    if (strcmp(type, "TRIANGLE_STRIP") == 0 || strcmp(type, "TRIANGLE_FAN") == 0) {
        unite = "TRIANGLES";
        if (s >= 3) return s - 2;
        restants = s;
        return 0;
    }
    return -1;
}

int main() {
    int N = 0;
    if (scanf("%d", &N) != 1) return 0;

    long totalPoints = 0, totalSegments = 0, totalTriangles = 0, totalRefuses = 0;

    for (int i = 0; i < N; ++i) {
        char type[64];
        int s;
        if (scanf("%63s %d", type, &s) != 2) return 0;

        int restants;
        const char *unite;
        int n = compte(type, s, restants, unite);

        if (n < 0) {
            printf("%s %d REFUSE\n", type, s);
            totalRefuses++;
            continue;
        }

        printf("%s %d %d %s %d\n", type, s, n, unite, restants);

        if (strcmp(unite, "POINTS") == 0) totalPoints += n;
        else if (strcmp(unite, "SEGMENTS") == 0) totalSegments += n;
        else if (strcmp(unite, "TRIANGLES") == 0) totalTriangles += n;
    }

    printf("POINTS %ld\n", totalPoints);
    printf("SEGMENTS %ld\n", totalSegments);
    printf("TRIANGLES %ld\n", totalTriangles);
    printf("REFUSES %ld\n", totalRefuses);

    return 0;
}
