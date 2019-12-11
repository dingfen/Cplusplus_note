// volatile 用法
// 表示编译器不能对声明的变量作进一步优化
// 该变量必须从内存中读取数据 并写入数据到内存
// 多用于设备的读取地址、写入地址
volatile unsigned char recv;
volatile unsigned char send;

unsigned char buf[3];

int main(void)
{
    buf[0] = recv;
    buf[1] = recv;
    buf[2] = recv;
    send = -buf[0];
    send = -buf[1];
    send = -buf[2];
}