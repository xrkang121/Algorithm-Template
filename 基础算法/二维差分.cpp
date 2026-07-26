#include <bits/stdc++.h>
using namespace std;

#define int long long

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);

    int n, m, q;
    cin >> n >> m >> q;

    vector<vector<int>> arr(n + 2, vector<int>(m + 2, 0));
    vector<vector<int>> diff(n + 2, vector<int>(m + 2, 0));

    // 输入原矩阵
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            cin >> arr[i][j];
        }
    }

    // 根据原矩阵构造二维差分数组
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            diff[i][j] = arr[i][j]
                       - arr[i - 1][j]
                       - arr[i][j - 1]
                       + arr[i - 1][j - 1];
        }
    }

    // q 次矩形加法
    while (q--)
    {
        int x1, y1, x2, y2, c;
        cin >> x1 >> y1 >> x2 >> y2 >> c;

        diff[x1][y1] += c;
        diff[x1][y2 + 1] -= c;
        diff[x2 + 1][y1] -= c;
        diff[x2 + 1][y2 + 1] += c;
    }

    // 对差分数组求二维前缀和，还原最终矩阵
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            diff[i][j] += diff[i - 1][j]
                        + diff[i][j - 1]
                        - diff[i - 1][j - 1];

            cout << diff[i][j] << " ";
        }

        cout << "\n";
    }

    return 0;
}
