/*
快读板子

使用方法：

1. 读取 int

int x;
read(x);

2. 读取 long long

long long x;
read(x);

3. 读取多个整数

int n, m;
read(n);
read(m);

4. 读取一条边

int u, v;
read(u);
read(v);

5. 多组测试数据

int t;
read(t);

while (t--)
{
    int n;
    read(n);
}

6. 判断是否读到文件末尾

int x;

if (!read(x))
{
    // 已经到达 EOF
}

注意：

使用本快读以后，输入统一使用 read(x)。

不要再混用：
cin
scanf
getchar

输出可以使用：
cout
printf
puts

如果使用 #define int long long，
建议把本板子放在 #define int long long 前面。
*/

namespace FastIO
{
    // 每次从输入中读取 1MB 数据
    const int SIZE = 1 << 20;

    // 输入缓冲区
    char buf[SIZE];

    // p1：当前读取位置
    // p2：当前缓冲区中的有效数据长度
    int p1 = 0;
    int p2 = 0;

    // 从缓冲区中读取一个字符
    inline int gc()
    {
        // 当前缓冲区已经读完
        if (p1 == p2)
        {
            // 重新从标准输入读取一块数据
            p2 = fread(buf, 1, SIZE, stdin);
            p1 = 0;

            // 没有读到任何数据，说明到达 EOF
            if (p1 == p2)
            {
                return EOF;
            }
        }

        // 返回当前字符，并移动读取位置
        return buf[p1++];
    }

    // 读取一个整数
    // 支持 int、long long 和负数
    // 读取成功返回 true
    // 到达 EOF 返回 false
    template<class T>
    inline bool read(T &x)
    {
        x = 0;

        int sign = 1;
        int c = gc();

        // 跳过空格、换行等非数字字符
        while (c != EOF &&
               c != '-' &&
               (c < '0' || c > '9'))
        {
            c = gc();
        }

        // 到达文件末尾
        if (c == EOF)
        {
            return false;
        }

        // 处理负数
        if (c == '-')
        {
            sign = -1;
            c = gc();
        }

        // 读取整数的每一位
        while (c >= '0' && c <= '9')
        {
            x = x * 10 + c - '0';
            c = gc();
        }

        x *= sign;

        return true;
    }
}

// 使用后可以直接写 read(x)，不需要写 FastIO::read(x)
using FastIO::read;
