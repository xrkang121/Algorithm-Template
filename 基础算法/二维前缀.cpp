#include <bits/stdc++.h>
using namespace std;

#define int long long

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> arr(n + 1, vector<int>(m + 1));
    vector<vector<int>> pre(n + 1, vector<int>(m + 1, 0));

    // 输入矩阵，同时预处理二维前缀和
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            cin >> arr[i][j];

            pre[i][j] = pre[i - 1][j]
                      + pre[i][j - 1]
                      - pre[i - 1][j - 1]
                      + arr[i][j];
        }
    }

    int q;
    cin >> q;

    while (q--)
    {
        // (x1,y1) 是左上角
        // (x2,y2) 是右下角
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;

        int sum = pre[x2][y2]
                - pre[x1 - 1][y2]
                - pre[x2][y1 - 1]
                + pre[x1 - 1][y1 - 1];

        cout << sum << "\n";
    }

    return 0;
}
