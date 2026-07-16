#include <bits/stdc++.h>
using namespace std;

#define int long long

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);

    string a, b;
    cin >> a >> b;

    int len_a = a.size();
    int len_b = b.size();

    vector<vector<int>> dp(len_a + 1, vector<int>(len_b + 1, 0));
    for (int i = 1; i <= len_a; i++)
    {
        for (int j = 1; j <= len_b; j++)
        {
            if (a[i - 1] == b[j - 1])
            {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            }
            else
            {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }

    cout << dp[len_a][len_b] << "\n";

    return 0;
}
