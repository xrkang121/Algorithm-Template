#include <bits/stdc++.h>
using namespace std;

#define int long long

const int MAXN = 2e4 + 5;

typedef struct Node{
    int ch[26];
    int flag; // 标记有无字符串结尾
}Node;

vector<Node> trie(MAXN);
int tot;

void init(){
    for(int i = 0;i < MAXN;i++){
        
        for(int j = 0;j < 26;j++){
            trie[i].ch[j] = 0;
        }

        trie[i].flag = 0;
    }

    tot = 0;
}

void insert(string str){
    int p = 0;

    for(int i = 0;i < str.size();i++){
        int c = str[i] - 'A';

        if(trie[p].ch[c] == 0){
            trie[p].ch[c] = ++tot;
        }

        p = trie[p].ch[c];
    }

    trie[p].flag = 1;
}

signed main(){
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);

    init();

    int n;
    cin >> n;

    vector<string> arr(n + 1);
    for(int i = 1;i <= n;i++){
        cin >> arr[i];
    }

    for(int i = 1;i <= n;i++){
        insert(arr[i]);
    }

    return 0;
}
