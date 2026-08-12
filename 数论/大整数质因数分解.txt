作用：分解一个或少量不超过 1e18 的整数。

把下面整个 namespace 复制进代码，不需要修改里面的内容。

调用：

auto factor = PollardRho::getFactor(x);

factor 中保存 {质因数, 出现次数}，并且已经按照质因数排序。

例如 x=360，返回 {{2,3},{3,2},{5,1}}。


namespace PollardRho
{
    using u64 = unsigned long long;
    using u128 = __uint128_t;

    mt19937_64 rnd(chrono::steady_clock::now().time_since_epoch().count());

    u64 mulMod(u64 a, u64 b, u64 mod)
    {
        return (u128)a * b % mod;
    }

    u64 qmi(u64 a, u64 b, u64 mod)
    {
        u64 res = 1;

        while (b > 0)
        {
            if (b & 1)
            {
                res = mulMod(res, a, mod);
            }

            a = mulMod(a, a, mod);
            b /= 2;
        }

        return res;
    }

    bool isPrime(u64 n)
    {
        if (n < 2)
        {
            return false;
        }

        for (u64 p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL})
        {
            if (n % p == 0)
            {
                return n == p;
            }
        }

        u64 d = n - 1;
        int s = 0;

        while (d % 2 == 0)
        {
            d /= 2;
            s++;
        }

        for (u64 a : {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL})
        {
            if (a % n == 0)
            {
                continue;
            }

            u64 x = qmi(a % n, d, n);

            if (x == 1 || x == n - 1)
            {
                continue;
            }

            int i = 1;

            while (i < s && x != n - 1)
            {
                x = mulMod(x, x, n);
                i++;
            }

            if (x != n - 1)
            {
                return false;
            }
        }

        return true;
    }

    u64 rho(u64 n)
    {
        if (n % 2 == 0)
        {
            return 2;
        }

        while (true)
        {
            u64 c = rnd() % (n - 1) + 1;
            u64 x = rnd() % n;
            u64 y = x;
            u64 d = 1;

            auto f = [&](u64 v)
            {
                return (mulMod(v, v, n) + c) % n;
            };

            while (d == 1)
            {
                x = f(x);
                y = f(f(y));
                d = gcd(x > y ? x - y : y - x, n);
            }

            if (d != n)
            {
                return d;
            }
        }
    }

    void divide(u64 n, vector<u64> &factor)
    {
        if (n == 1)
        {
            return;
        }

        if (isPrime(n))
        {
            factor.push_back(n);
            return;
        }

        u64 d = rho(n);

        divide(d, factor);
        divide(n / d, factor);
    }

    vector<pair<u64, int>> getFactor(u64 n)
    {
        vector<u64> all;
        divide(n, all);
        sort(all.begin(), all.end());

        vector<pair<u64, int>> factor;

        for (u64 p : all)
        {
            if (factor.empty() || factor.back().first != p)
            {
                factor.push_back({p, 1});
            }
            else
            {
                factor.back().second++;
            }
        }

        return factor;
    }
}


使用示例：

unsigned long long x;
cin >> x;

auto factor = PollardRho::getFactor(x);

for (auto [p, cnt] : factor)
{
    cout << p << " " << cnt << "\n";
}
