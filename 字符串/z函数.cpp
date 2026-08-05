/*
Z 函数：

z[i] 表示：
从字符串下标 i 开始的后缀，
与整个字符串前缀的最长公共前缀长度。

字符串下标从 0 开始。

使用方法：

vector<int> z = getZ(s);

z[i]：
s[0 ... z[i] - 1]
和
s[i ... i + z[i] - 1]
完全相同。
*/

vector<int> getZ(string s)
{
    int n = s.size();

    vector<int> z(n);
    z[0] = n;

    int l = 0;
    int r = 0;

    for (int i = 1; i < n; i++)
    {
        if (i <= r)
        {
            z[i] = min(z[i - l], r - i + 1);
        }

        while (i + z[i] < n && s[z[i]] == s[i + z[i]])
        {
            z[i]++;
        }

        if (i + z[i] - 1 > r)
        {
            l = i;
            r = i + z[i] - 1;
        }
    }

    return z;
}
