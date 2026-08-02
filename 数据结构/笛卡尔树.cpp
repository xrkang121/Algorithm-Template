//笛卡尔树区间l，r节点的祖先就是l到r区间构成笛卡尔树的根节点

#include <bits/stdc++.h>
using namespace std;

#define int long long

const int MAXN = 2e5 + 5;

vector<vector<int>> tree(MAXN);

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

        for (int i = 1; i <= n; i++)
        {
            tree[i].clear();
        }

        vector<int> arr(n + 1);

        for (int i = 1; i <= n; i++)
        {
            cin >> arr[i];
        }

        vector<int> ls(n + 1, 0);
        vector<int> rs(n + 1, 0);
        vector<int> father(n + 1, 0);

        stack<pair<int, int>> stk;

        for (int i = 1; i <= n; i++)
        {
            pair<int, int> last;
            last.first = 0, last.second = 0;

            while (!stk.empty() && stk.top().second > arr[i])
            {
                last = stk.top();
                stk.pop();
            }

            /*
            弹栈结束后，如果栈不为空，
            那么 i 成为栈顶节点的右儿子。

            注意这里使用赋值，而不是 push_back，
            因为原来的右儿子可能需要被替换。
            */
            if (!stk.empty())
            {
                rs[stk.top().first] = i;
                father[i] = stk.top().first;
            }

            /*
            最后一个弹出的节点成为 i 的左儿子。
            */
            if (last.first != 0)
            {
                int x = last.first;

                ls[i] = x;
                father[x] = i;
            }

            stk.push({i, arr[i]});
        }

        /*
        笛卡尔树全部建好以后，
        再根据最终的左右儿子生成邻接表。
        */
        for (int i = 1; i <= n; i++)
        {
            if (ls[i] != 0)
            {
                tree[i].push_back(ls[i]);
            }

            if (rs[i] != 0)
            {
                tree[i].push_back(rs[i]);
            }
        }

        int root = 0;

        for (int i = 1; i <= n; i++)
        {
            if (father[i] == 0)
            {
                root = i;
                break;
            }
        }

        
    }

    return 0;
}
