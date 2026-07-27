#include <bits/stdc++.h>
using namespace std;

#define int long long

/*
使用方法：

s 是文本串，t 是模式串。

Next[i] 表示：
t[1...i] 中，最长的相等真前缀和真后缀长度。

匹配成功时：
i - m + 1 是模式串在文本串中的起始位置。

匹配成功后写：
j = Next[j];
表示允许重复匹配。
*/

const int MAXN = 2e6 + 5;

int Next[MAXN];

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);

    string s, t;
    cin >> s >> t;

    s = " " + s;
    t = " " + t;

    int n = s.size() - 1;
    int m = t.size() - 1;

    for (int i = 2, j = 0; i <= m; i++)
    {
        while (j > 0 && t[i] != t[j + 1])
        {
            j = Next[j];
        }

        if (t[i] == t[j + 1])
        {
            j++;
        }

        Next[i] = j;
    }

    vector<int> pos;

    for (int i = 1, j = 0; i <= n; i++)
    {
        while (j > 0 && s[i] != t[j + 1])
        {
            j = Next[j];
        }

        if (s[i] == t[j + 1])
        {
            j++;
        }

        if (j == m)
        {
            pos.push_back(i - m + 1);

            j = Next[j];
        }
    }

    cout << pos.size() << '\n';

    for (int i = 0; i < pos.size(); i++)
    {
        cout << pos[i] << " ";
    }

    cout << '\n';

    return 0;
}
