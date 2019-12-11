const int A = 10;       // global
int a = 20;             // global
static int b = 30;
int c;

int main(int argc, char const *argv[])
{
    static int e = 40;      // local
    char d[] = "hello";
    register int f = 50;
    return 0;
}