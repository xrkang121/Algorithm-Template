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

// omega[i]：i 的质因数个数（包含重复）
// 例如：omega[12] = 3，因为 12 = 2 * 2 * 3
// du[i]：i 的最小质因子

// 核心递推：
// omega[1] = 0
// omega[i] = omega[i / du[i]] + 1

// 性质：omega(a * b) = omega(a) + omega(b)
// 若 d | x，则 omega(x / d) = omega(x) - omega(d)

// 用途：快速计算质因数个数、因子费用、乘积的质因数个数
// 预处理 O(MAXN)，单次查询 O(1)

vector<int> omega(MAXN, 0);

for (int i = 2; i < MAXN; i++)
{
    omega[i] = omega[i / du[i]] + 1;
}
