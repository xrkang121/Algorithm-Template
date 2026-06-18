#include <bits/stdc++.h>
using namespace std;

#define int long long

const int MAXN = 1e7 + 5;

typedef struct Node{
    int ch[26]; // 当前节点往后接某个字符会走到哪个节点
    int fail; // 当前节点代表的字符串的最长后缀，这个后缀代表trie树种的某个前缀
    int cnt; // 有多少个模式串在这个节点结尾
}Node;

vector<Node> tree(MAXN); // ac自动机所有节点
vector<int> order; // 用来保存bfs序，后面统计cnt用
 
int tot; // 当前最后一个节点的编号

void init(){
    for(int i = 0;i < MAXN;i++){
        for(int j = 0;j < 26;j++){
            tree[i].ch[j] = 0;
        }

        tree[i].fail = 0;
        tree[i].cnt = 0;
    }

    order.clear();
    tot = 0;
}

// 构建字典树
void insert(string str){
    int p = 0;

    for(int i = 0;i < str.size();i++){
        int c = str[i] - 'a';

        if(tree[p].ch[c] == 0){
            tree[p].ch[c] = ++tot;
        }

        p = tree[p].ch[c];
    }

    tree[p].ch[c]++;
}

void build(){
    queue<int> q;

    // 所有有后续的节点全部加入bfs
    for(int i = 0;i < 26;i++){
        int v = tree[0].ch[i];

        if(v != 0){
            tree[v].fail = 0;
            q.push(v);
        }
    }

    while(!q.empty()){
        int u = q.front();
        q.pop();

        order.push_back(u);

        for(int i = 0;i < 26;i++){
            int v = tree[u].ch[i];

            if(v != 0){
                //如果这个节点被使用，那么这个节点的后缀就是前一个节点的后缀的相同字母走向
                tree[v].fail = tree[tree[u].fail].ch[i];
                q.push(v);
            }
            else{
                // 如果这个节点没有被使用，那么把这个节点指向当前字符串后缀的那个节点
                tree[u].ch[i] = tree[tree[u].fail].ch[i];
            }
        }
    }

    for(int i = 0;i < order.size();i++){
        int u = order[i];
        tree[u].cnt += tree[tree[u].fail].cnt;
    }
}

int query(string str){
    int p = 0;
    int res = 0;

    for(int i = 0;i < str.size();i++){
        int c = str[i] - 'a';

        p = tree[p].ch[c];

        res += tree[p].cnt;
    }

    return res;
}


signed main()
{
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);

    init();

    int n;
    cin >> n;

    for(int i = 1;i <= n;i++){
        string str;
        cin >> str;

        insert(str);
    }

    build();

    string text;
    cin >> text;

    // 输出全部模式串在测试串中出现的次数
    cout << query(text) << "\n";
    

    return 0;
}
