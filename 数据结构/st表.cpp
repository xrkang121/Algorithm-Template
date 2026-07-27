#include <bits/stdc++.h>
using namespace std;

#define int long long

/*
使用方法：

1. 把数组存入 arr[1...n]
2. 调用 init(n) 进行预处理
3. query(l, r) 返回区间 [l, r] 的最大值

如果要求区间最小值：
把代码中的两个 max 全部改成 min

如果要求区间最大公约数：
把代码中的两个 max 全部改成 gcd
*/

const int MAXN = 2e5 + 5;
const int LOG = 20;

int arr[MAXN];
int st[MAXN][LOG + 1];

int power[LOG + 1];
int lg[MAXN];

void init(int n)
{
    power[0] = 1;
    for (int i = 1; i <= LOG; i++)
    {
        power[i] = power[i - 1] * 2;
    }

    lg[1] = 0;
    for (int i = 2; i <= n; i++)
    {
        lg[i] = lg[i / 2] + 1;
    }

    for (int i = 1; i <= n; i++)
    {
        st[i][0] = arr[i];
    }

    for (int j = 1; power[j] <= n; j++)
    {
        for (int i = 1; i + power[j] - 1 <= n; i++)
        {
            st[i][j] = max(
                st[i][j - 1],
                st[i + power[j - 1]][j - 1]
            );
        }
    }
}

int query(int l, int r)
{
    int len = r - l + 1;
    int k = lg[len];

    return max(
        st[l][k],
        st[r - power[k] + 1][k]
    );
}

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);

    int n, q;
    cin >> n >> q;

    for (int i = 1; i <= n; i++)
    {
        cin >> arr[i];
    }

    init(n);

    while (q--)
    {
        int l, r;
        cin >> l >> r;

        cout << query(l, r) << '\n';
    }

    return 0;
}
