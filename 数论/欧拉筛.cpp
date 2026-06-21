const int MAXN = 5e5 + 5;

vector<int> du(MAXN, 0);
vector<int> prime;

void sieve(){

    for(int i = 2;i <= 5e5;i++){
        if(du[i] == 0){
            prime.push_back(i);
            du[i] = i;
        }

        for(int p : prime){
            if(p > du[i] || p * i > 5e5){
                break;
            }

            du[i * p] = p;
        }
    }
}
