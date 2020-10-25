#include <iostream>
#include <cstdlib>

using std::cout;
using std::endl;

int a[8] = {0};
int count = 0;

bool iscorrect(int line, int pos) {
    if (line == 0)
        return true;
    for(int i = 0; i < line; i++) {
        if (abs(i-line) == abs(pos-a[i]) || pos == a[i])
            return false;
    }
    return true;
}

void printT(int n) {
    for(int i = 0; i < n; i++) {
        int p = a[i];
        for(int j = 1; j < p; j++)
            printf(".");
        printf("*");
        for(int j = p+1; j <=n; j++)
            printf(".");
        printf("\n");
    }
}

void queen(int n, int step) {
    if (step == 0) {
        printT(n);
        printf("\n");
        count++;
    } else {
        for(int i = 1; i <= n; i++) {
            int line = n-step;
            if (iscorrect(line, i)) {
                a[line] = i;
                queen(n, step-1);
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        printf("Usage: Input the n of queens\n");
        return 0;
    }
    int q = std::stoi(argv[1]);
    queen(q, q);
    printf("%d queens problem has %d answers.\n", q, count);
    return 0;
}
