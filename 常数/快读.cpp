/*
快读板子

支持类型：

1. 整数
int x;
long long y;
read(x);
read(y);

2. __int128 大整数
__int128 x;
read(x);

注意：
cout 不能直接输出 __int128，需要单独写输出函数。

3. 任意位数大整数

如果整数可能超过 __int128，用 string 保存：

string big_number;
read(big_number);

4. 小数
float a;
double b;
long double c;

read(a);
read(b);
read(c);

支持：
-12.34
3.14159
1.2e10
-2.5E-6

5. 单个非空白字符

char c;
read(c);

会自动跳过空格和换行。

6. 字符串

string s;
read(s);

读取到下一个空格或换行为止。

7. 读取原始字符

char c;
readRawChar(c);

不会跳过空格和换行。

8. 判断是否到达 EOF

int x;

if (!read(x))
{
    // 已到达文件末尾
}

注意：

使用本快读后，输入统一使用 read(x)。

不要再混用：
cin
scanf
getchar

输出可以继续使用：
cout
printf
puts

如果使用：
#define int long long

建议把本板子放在宏定义之前。
*/

namespace FastIO
{
    // 输入缓冲区大小：1MB
    const int SIZE = 1 << 20;

    char buf[SIZE];

    std::size_t p1 = 0;
    std::size_t p2 = 0;

    // 读取一个原始字符
    inline int gc()
    {
        if (p1 == p2)
        {
            p2 = fread(buf, 1, SIZE, stdin);
            p1 = 0;

            if (p1 == p2)
            {
                return EOF;
            }
        }

        return (unsigned char)buf[p1++];
    }

    // 读取整数
    // 支持 int、long long、__int128 等整数类型
    template<class T>
    inline bool read(T &x)
    {
        x = 0;

        bool negative = false;
        int c = gc();

        // 跳过非数字字符
        while (c != EOF &&
               c != '-' &&
               c != '+' &&
               (c < '0' || c > '9'))
        {
            c = gc();
        }

        if (c == EOF)
        {
            return false;
        }

        if (c == '-')
        {
            negative = true;
            c = gc();
        }
        else if (c == '+')
        {
            c = gc();
        }

        while (c >= '0' && c <= '9')
        {
            x = x * 10 + (c - '0');
            c = gc();
        }

        if (negative)
        {
            x = -x;
        }

        return true;
    }

    // 读取 long double
    // float 和 double 会调用这个函数
    inline bool read(long double &x)
    {
        x = 0;

        bool negative = false;
        int c = gc();

        // 寻找小数开头
        while (c != EOF &&
               c != '-' &&
               c != '+' &&
               c != '.' &&
               (c < '0' || c > '9'))
        {
            c = gc();
        }

        if (c == EOF)
        {
            return false;
        }

        if (c == '-')
        {
            negative = true;
            c = gc();
        }
        else if (c == '+')
        {
            c = gc();
        }

        long double value = 0;

        // 读取整数部分
        while (c >= '0' && c <= '9')
        {
            value = value * 10 + (c - '0');
            c = gc();
        }

        // 读取小数部分
        if (c == '.')
        {
            long double base = 0.1L;

            c = gc();

            while (c >= '0' && c <= '9')
            {
                value += (c - '0') * base;
                base *= 0.1L;

                c = gc();
            }
        }

        // 读取科学计数法
        if (c == 'e' || c == 'E')
        {
            c = gc();

            bool exponent_negative = false;

            if (c == '-')
            {
                exponent_negative = true;
                c = gc();
            }
            else if (c == '+')
            {
                c = gc();
            }

            int exponent = 0;

            while (c >= '0' && c <= '9')
            {
                exponent = exponent * 10 + (c - '0');
                c = gc();
            }

            if (exponent_negative)
            {
                exponent = -exponent;
            }

            value *= std::pow(10.0L, exponent);
        }

        if (negative)
        {
            value = -value;
        }

        x = value;

        return true;
    }

    // 读取 double
    inline bool read(double &x)
    {
        long double value;

        if (!read(value))
        {
            return false;
        }

        x = (double)value;

        return true;
    }

    // 读取 float
    inline bool read(float &x)
    {
        long double value;

        if (!read(value))
        {
            return false;
        }

        x = (float)value;

        return true;
    }

    // 读取一个非空白字符
    inline bool read(char &c)
    {
        int now = gc();

        while (now != EOF && now <= ' ')
        {
            now = gc();
        }

        if (now == EOF)
        {
            return false;
        }

        c = (char)now;

        return true;
    }

    // 读取字符串
    // 遇到空格、换行、Tab 时停止
    inline bool read(std::string &s)
    {
        s.clear();

        int c = gc();

        while (c != EOF && c <= ' ')
        {
            c = gc();
        }

        if (c == EOF)
        {
            return false;
        }

        while (c > ' ')
        {
            s.push_back((char)c);
            c = gc();
        }

        return true;
    }

    // 读取原始字符
    // 不跳过空格和换行
    inline bool readRawChar(char &c)
    {
        int now = gc();

        if (now == EOF)
        {
            return false;
        }

        c = (char)now;

        return true;
    }
}

// 可以直接写 read(x)
using FastIO::read;

// 原始字符读取
using FastIO::readRawChar;
