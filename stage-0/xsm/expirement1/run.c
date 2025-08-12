#include <stdio.h>
#include <stdlib.h>

int main(void) {
    FILE *target_file = fopen("test.xsm", "w");
    if (!target_file) {
        perror("fopen");
        return 1;
    }

    /* write the 8-word header (reserve first 8 words) */
    fprintf(target_file, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",
            0,    /* 0 - usually reserved (e.g., magic/unused) */
            2056, /* entry point -> first instruction (as you specified) */
            0,
            0,
            0,
            0,
            0,
            0);

    /* now write instructions starting at address 2056 in memory model */
    fprintf(target_file, "BRKP\n");
    fprintf(target_file, "MOV R0, 3\n");
    fprintf(target_file, "MOV R1, 2\n");
    fprintf(target_file, "ADD R0, R1\n");

    fclose(target_file);
    printf("Wrote target_file.xsm with header + instructions\n");
    return 0;
}
