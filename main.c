#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_LINE_LEN 128
#define MAX_LINES 16

int parseIntArray(const char* str, int** arr) {
    int count = 1;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ' ') {
            count++;
        }
    }

    *arr = (int*)malloc(count * sizeof(int));
    if (*arr == NULL) return -1;

    char* strCopy = strdup(str);
    char* token = strtok(strCopy, " ");
    int index = 0;
    while (token != NULL) {
        char* endptr;
        (*arr)[index++] = strtol(token, &endptr, 10);
        token = strtok(NULL, " ");
    }
    free(strCopy);
    return count;
}

void printOptimalParenthesization(int** splitPoints, int i, int j) {
    if (i == j) {
        printf("A%d", i);
    } else {
        printf("(");
        printOptimalParenthesization(splitPoints, i, splitPoints[i][j]);
        printf(".");
        printOptimalParenthesization(splitPoints, splitPoints[i][j] + 1, j);
        printf(")");
    }
}

void matrixChainMultiplication(int* dimensions, int matrixSeqLen) {
    int** minCosts = (int**)malloc(matrixSeqLen * sizeof(int*));
    int** splitPoints = (int**)malloc(matrixSeqLen * sizeof(int*));
    for (int i = 0; i < matrixSeqLen; i++) {
        minCosts[i] = (int*)malloc(matrixSeqLen * sizeof(int));
        splitPoints[i] = (int*)malloc(matrixSeqLen * sizeof(int));
    }

    for (int i = 0; i < matrixSeqLen; i++) {
        minCosts[i][i] = 0;
    }

    for (int length = 2; length <= matrixSeqLen; length++) {
        for (int i = 0; i <= matrixSeqLen - length; i++) {
            int j = i + length - 1;
            minCosts[i][j] = INT_MAX;
            for (int k = i; k < j; k++) {
                int cost = minCosts[i][k] + minCosts[k + 1][j] + dimensions[i] * dimensions[k + 1] * dimensions[j + 1];
                if (cost < minCosts[i][j]) {
                    minCosts[i][j] = cost;
                    splitPoints[i][j] = k;
                }
            }
        }
    }

    printf("Minimum multiplications: %d\n", minCosts[0][matrixSeqLen - 1]);
    printf("Optimal parenthesization: ");
    printOptimalParenthesization(splitPoints, 0, matrixSeqLen - 1);
    printf("\n");

    for (int i = 0; i < matrixSeqLen; i++) {
        free(minCosts[i]);
        free(splitPoints[i]);
    }
    free(minCosts);
    free(splitPoints);
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printf("No datafile provided\n");
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file \"%s\"\n", argv[1]);
        return 1;
    }

    char line[MAX_LINE_LEN];
    int* dimensionsArrays[MAX_LINES];
    int dimensionsLenghts[MAX_LINES];
    int lineCount = 0;

    while (fgets(line, MAX_LINE_LEN, file) != NULL && lineCount < MAX_LINES) {
        dimensionsLenghts[lineCount] = parseIntArray(line, &dimensionsArrays[lineCount]);
        if (dimensionsLenghts[lineCount] == -1) {
            printf("Memory allocation error for line %d\n", lineCount + 1);
            fclose(file);
            for (int i = 0; i < lineCount; i++) free(dimensionsArrays[i]);
            return 1;
        }
        lineCount++;
    }
    fclose(file);

    for (int i = 0; i < lineCount; i++) {
        printf("\nLine %d: ", i + 1);

        for (int j = 0; j < dimensionsLenghts[i]; j++) {
            printf("%d ", dimensionsArrays[i][j]);
        }
        printf("\n");

        if (dimensionsLenghts[i] > 1) {
            matrixChainMultiplication(dimensionsArrays[i], dimensionsLenghts[i] - 1);
        } else {
            printf("Not enough dimensions for matrix chain multiplication.\n");
        }
    }

    for (int i = 0; i < lineCount; i++) {
        free(dimensionsArrays[i]);
    }

    printf("\n");
    return 0;
}
