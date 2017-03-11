#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include <algorithm>
using namespace std;

class Automat
{
public:
    int q, n;
    vector <int> f;
    vector <pair<int, char> > v[10000];
public:
    Automat(){};
    Automat(char c);
    Automat (const Automat & a);
    friend ostream& operator <<(ostream & out, Automat a);
    Automat operator +(Automat a);
    Automat operator -(Automat a);
    Automat operator *();
    Automat operator = (Automat a);
};

Automat ans;

Automat::Automat(char c)
{
    q = 0;
    n = 2;
    f.push_back(1);
    v[0].push_back(make_pair(1, c));
}

Automat Automat::operator = (Automat a)
{
    while(!f.empty())
    {
        f.pop_back();
    }
    for(unsigned int i = 0; i < a.f.size(); i++)
    {
        f.push_back(a.f[i]);
    }
    for(int i = 0; i < n; i++)
    {
        while(!v[i].empty())
        {
            v[i].pop_back();
        }
    }
    for(int i = 0; i < a.n; i++)
    {
        for(unsigned int j = 0; j < a.v[i].size(); j++)
        {
            v[i].push_back(a.v[i][j]);
        }
    }
    q = a.q;
    n = a.n;
    return *this;
}

Automat::Automat (const Automat & a)
{
    for(unsigned int i = 0; i < a.f.size(); i++)
    {
        f.push_back(a.f[i]);
    }
    for(int i = 0; i < a.n; i++)
    {
        for(unsigned int j = 0; j < a.v[i].size(); j++)
        {
            v[i].push_back(a.v[i][j]);
        }
    }
    q = a.q;
    n = a.n;
}

ostream& operator <<(ostream & out, Automat a)
{
    out << "Stare initiala: " << a.q << endl;
    out << "Stari finale: ";
    for(unsigned int i = 0; i < a.f.size(); i++)
    {
        out << a.f[i] << " ";
    }
    out << endl;
    out << "Muchii:" << endl;
    for(int i = 0; i < a.n; i++)
    {
        out << i << " : ";
        for(unsigned int j = 0; j < a.v[i].size(); j++)
        {
            out << "(" << a.v[i][j].first << " , " << a.v[i][j].second << ") ";
        }
        out << endl;
    }
    return out;
}

Automat Automat::operator +(Automat a)
{
    for(int i = n - 1; i >= 0; i--)
    {
        for(int j = v[i].size() - 1; j >= 0; j--)
        {
            v[i + 1].push_back(make_pair(v[i][j].first + 1, v[i][j].second));
            v[i].pop_back();
        }
    }
    n++;
    for(unsigned int i = 0; i < f.size(); i++)
    {
        f[i] ++;
    }
    v[0].push_back(make_pair(1,'^'));
    int m = n;
    for(int i = 0; i < a.n; i++)
    {
        for(unsigned int j = 0; j < a.v[i].size(); j++)
        {
            v[n].push_back(make_pair(a.v[i][j].first + m,a.v[i][j].second));
        }
        n++;
    }
    for(unsigned int i = 0; i < a.f.size(); i++)
    {
        f.push_back(a.f[i] + m);
    }
    v[0].push_back(make_pair(m,'^'));
    for(int i = f.size() - 1; i >= 0; i--)
    {
        v[f[i]].push_back(make_pair(n,'^'));
        f.pop_back();
    }
    f.push_back(n);
    n++;
    return *this;
}

Automat Automat::operator -(Automat a)
{
    for(int i = f.size() - 1; i >= 0; i--)
    {
        v[f[i]].push_back(make_pair(a.q + n,'^'));
        f.pop_back();
    }
    int m = n;
    for(int i = 0; i < a.n; i++)
    {
        for(unsigned int j = 0; j < a.v[i].size(); j++)
        {
            v[n].push_back(make_pair(a.v[i][j].first + m,a.v[i][j].second));
        }
        n++;
    }
    for(unsigned int i = 0; i < a.f.size(); i++)
    {
        f.push_back(a.f[i] + m);
    }
    return *this;
}

Automat Automat::operator *()
{
    for(int i = n - 1; i >= 0; i--)
    {
        for(int j = v[i].size() - 1; j >= 0; j--)
        {
            v[i + 1].push_back(make_pair(v[i][j].first + 1, v[i][j].second));
            v[i].pop_back();
        }
    }
    n++;
    v[0].push_back(make_pair(1, '^'));
    v[f[0] + 1].push_back(make_pair(0, '^'));
    f[0] = 0;
    return *this;
}

vector <int> viz;

void verificare_cuvant_afn(unsigned int poz, int sol, string s,int &ok)
{
    if(!ok)
    {
        if(poz <= s.size())
        {
            for(unsigned int i = 0; i < ans.v[sol].size(); i++)
            {
                if(ans.v[sol][i].second == s[poz])
                {
                    for(int j = 1; j <= ans.n; j++)
                    {
                        viz[j] = 0;
                    }
                    verificare_cuvant_afn(poz + 1, ans.v[sol][i].first, s, ok);
                }
                else
                    if(ans.v[sol][i].second == '^' && viz[ans.v[sol][i].first] == 0)
                    {
                        viz[ans.v[sol][i].first] = 1;
                        verificare_cuvant_afn(poz, ans.v[sol][i].first, s, ok);
                    }
            }
        }

        for(unsigned int i = 0; i < ans.f.size(); i++)
        {
            if(sol == ans.f[i])
            {
                ok = 1;
            }
        }
    }
}

int main()
{
    ifstream f("date.in");
    string s;
    stack <Automat> s1;
    stack <char> s2;
    f >> s;
    int ok = 0;
    s2.push('(');
    s.push_back(')');

    for(unsigned int i = 0; i < s.length(); i++)
    {
        if(s[i] == '*')
        {
            Automat a = s1.top();
            s1.pop();
            s1.push(*a);
        }
        if(s[i] == '+')
        {
            s2.push('+');
        }
        if(s[i] >= 'a' && s[i] <= 'z')
        {
            if(ok == 0)
            {
                s1.push(s[i]);
                ok = 1;
            }
            else
            {
                s1.push(s[i]);
                if(s[i - 1] != '+' && s[i - 1] != '(')
                {
                    s2.push('-');
                }
            }
        }
        if(s[i] == '(')
        {
            if(s[i - 1] != '+' && s[i - 1] != '(' && ok != 0)
            {
                s2.push('-');
            }
            s2.push('(');
        }
        if(s[i] == ')')
        {
            while(s2.top() != '(')
            {
                Automat a, b;
                a = s1.top();
                s1.pop();
                b = s1.top();
                s1.pop();
                if(s2.top() == '+')
                {
                    s1.push(b + a);
                    s2.pop();
                }
                else
                    if(s2.top() == '-')
                    {
                        s1.push(b - a);
                        s2.pop();
                    }
            }
            s2.pop();
        }
    }

    ans = s1.top();
    cout << ans;

    f >> s;
    ok = 0;
    for(int i = 0; i <= ans.n; i++)
    {
        viz.push_back(0);
    }
    verificare_cuvant_afn(0, ans.q, s, ok);
    if(ok == 1)
    {
        cout << "Cuvant acceptat!";
    }
    else
    {
        cout << "Cuvantul nu este acceptat!";
    }

    return 0;
}
