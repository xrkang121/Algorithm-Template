#include <bits/stdc++.h>
using namespace std;

#define int long long

/*
使用方法：

StringHash Hash(s);

Hash.query(l, r)
返回字符串中下标 [l, r] 的哈希值，下标从 1 开始。

判断两个子串是否相同：

if (Hash.query(l1, r1) == Hash.query(l2, r2))
{
    // 两个子串相同
}

也可以比较两个不同字符串的子串：

StringHash A(a);
StringHash B(b);

if (A.query(l1, r1) == B.query(l2, r2))
{
    // 两个子串相同
}
*/

struct StringHash
{
    const int base = 131;
    const int mod1 = 1000000007;
    const int mod2 = 1000000009;

    vector<int> Hash1;
    vector<int> Hash2;

    vector<int> power1;
    vector<int> power2;

    StringHash(string s)
    {
        int n = s.size();
        s = " " + s;

        Hash1.resize(n + 1);
        Hash2.resize(n + 1);

        power1.resize(n + 1);
        power2.resize(n + 1);

        power1[0] = 1;
        power2[0] = 1;

        for (int i = 1; i <= n; i++)
        {
            power1[i] = power1[i - 1] * base % mod1;
            power2[i] = power2[i - 1] * base % mod2;

            int value = s[i];

            Hash1[i] = (Hash1[i - 1] * base + value) % mod1;
            Hash2[i] = (Hash2[i - 1] * base + value) % mod2;
        }
    }

    pair<int, int> query(int l, int r)
    {
        int len = r - l + 1;

        int res1 = Hash1[r] - Hash1[l - 1] * power1[len] % mod1;
        int res2 = Hash2[r] - Hash2[l - 1] * power2[len] % mod2;

        res1 = (res1 + mod1) % mod1;
        res2 = (res2 + mod2) % mod2;

        return {res1, res2};
    }
};

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);

    string s;
    cin >> s;

    StringHash Hash(s);

    int q;
    cin >> q;

    while (q--)
    {
        int l1, r1;
        int l2, r2;

        cin >> l1 >> r1;
        cin >> l2 >> r2;

        if (Hash.query(l1, r1) == Hash.query(l2, r2))
        {
            cout << "Yes\n";
        }
        else
        {
            cout << "No\n";
        }
    }

    return 0;
}
