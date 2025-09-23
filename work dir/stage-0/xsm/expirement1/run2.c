#include <stdio.h>
#include <stdlib.h>

int main(void) {
    FILE *target_file = fopen("test2.xsm", "w");
    if (!target_file) {
        perror("fopen");
        return 1;
    }

    /* write the 8-word header (reserve first 8 words) */
    fprintf(target_file, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",
            0,     // 2048
            2056,  // entry point (first instruction)
            0, 0, 0, 0, 0, 0);

    /* Instructions (each 2 words in memory model) */
    fprintf(target_file, "BRKP\n");                // 2056
    fprintf(target_file, "MOV R0, 2\n");           // 2058
    fprintf(target_file, "MOV R1, 4\n");           // 2060
    fprintf(target_file, "MOV R2, 5\n");           // 2062
    fprintf(target_file, "MOV R3, R2\n");          // 2064
    fprintf(target_file, "GT R3, R0\n");           // 2066
    fprintf(target_file, "JNZ R3, 2080\n");        // 2068
    fprintf(target_file, "MOV R3, R1\n");          // 2070
    fprintf(target_file, "GT R3, R0\n");           // 2072
    fprintf(target_file, "JZ R3, 2090\n");         // 2074
    fprintf(target_file, "MOV R4, R1\n");          // 2076
    fprintf(target_file, "JNZ R3, 5000\n");        // 2078
    fprintf(target_file, "MOV R3, R2\n");          // 2080
    fprintf(target_file, "GT R3, R1\n");           // 2082
    fprintf(target_file, "JNZ R3, 2094\n");        // 2084
    fprintf(target_file, "MOV R4, R1\n");          // 2086
    fprintf(target_file, "JMP 5000\n");            // 2088
    fprintf(target_file, "MOV R4, R0\n");          // 2090
    fprintf(target_file, "JMP 5000\n");            // 2092
    fprintf(target_file, "MOV R4, R2\n");          // 2094

    fclose(target_file);
    printf("Wrote test2.xsm with header + instructions\n");
    return 0;
}
