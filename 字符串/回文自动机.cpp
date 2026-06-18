#include <bits/stdc++.h>
using namespace std;

#define int long long

const int MAXN = 3e5 + 5;

typedef struct Node
{
    int len;    // 当前节点回文串长度
    int fail;   // 当前回文串的最长真回文后缀
    int ch[26]; // 两边加某个字符后转移到哪个回文串
    int cnt;    // 当前回文串出现的次数
} Node;

vector<Node> tree(MAXN); // 回文自动机的所有节点
vector<char> s(MAXN);    // 存字符串，使用1下标

int tot;  // 当前最后一个节点编号
int last; // 当前字符串的最长回文后缀节点

void init()
{
    for (int i = 0; i < MAXN; i++)
    {
        tree[i].len = 0;
        tree[i].fail = 0;

        for (int j = 0; j < 26; j++)
        {
            tree[i].ch[j] = 0;
        }

        tree[i].cnt = 0;
    }

    tree[0].len = 0;  // 0节点表示偶根，表示空串
    tree[1].len = -1; // 1节点表示奇根，是虚拟节点

    tree[0].fail = 1;
    tree[1].fail = 1;

    tot = 1;
    last = 0;

    s[0] = '#';
}

// 从节点x开始，不断跳fail，直到找到一个可以被当前字符扩展的回文串
int getfail(int x, int i)
{
    while (s[i - tree[x].len - 1] != s[i])
    {
        x = tree[x].fail;
    }

    return x;
}

void insert_char(int i)
{
    int c = s[i] - 'a';

    // 找到以新加字符串为结尾的最长回文串
    int cur = getfail(last, i);

    if (tree[cur].ch[c] == 0)
    {
        int now = ++tot;

        // 是前后都要加，所以长度为2
        tree[now].len = tree[cur].len + 2;

        if (tree[now].len == 1)
        {
            // 长度为1的，真后缀为空
            tree[now].fail = 0;
        }
        else
        {
            // 继续找cur的下一个能拼接的
            int fail_node = getfail(tree[cur].fail, i);
            tree[now].fail = tree[fail_node].ch[c];
        }

        tree[cur].ch[c] = now;
    }

    last = tree[cur].ch[c];
    tree[last].cnt++;
}

signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);

    init();

    string str;
    cin >> str;

    int n = str.size();

    for (int i = 1; i <= n; i++)
    {
        s[i] = str[i - 1];
        insert_char(i);
    }

    // 通过后缀累加出现次数
    for (int i = tot; i >= 2; i--)
    {
        tree[tree[i].fail].cnt += tree[i].cnt;
    }

    return 0;
}
