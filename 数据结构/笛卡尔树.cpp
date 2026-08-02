#include <bits/stdc++.h>
using namespace std;

#define int long long

const int MAXN = 2e5 + 5;

vector<vector<int>> tree(MAXN, vector<int>());

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);

    int t;
    cin >> t;

    while (t--)
    {
        int n, q;
        cin >> n >> q;

        vector<int> arr(n + 1);
        for (int i = 1; i <= n; i++)
        {
            cin >> arr[i];
        }

        // 单调队列求笛卡尔树
        stack<pair<int, int>> stk;
        for (int i = 1; i <= n; i++)
        {

            pair<int, int> it;
            it.first = -1, it.second = -1;

            // 小根笛卡尔树
            while (!stk.empty() && stk.top().second > arr[i])
            {
                it = stk.top();
                stk.pop();
            }

            if (it.first != -1)
            {
                tree[i].push_back(it.first);
            }

            stk.push({i, arr[i]});
        }

        pair<int, int> it;
        if (!stk.empty())
        {
            it = stk.top();
            stk.pop();
        }

        while (!stk.empty())
        {
            tree[stk.top().first].push_back(it.first);
            it = stk.top();
            stk.pop();
        }
    }

    return 0;
}
