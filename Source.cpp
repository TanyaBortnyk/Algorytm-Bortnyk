#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

void bubbleSort(vector<int>& a, long long& cmp, long long& swaps)
{
    for (int i = 0; i < a.size() - 1; i++)
    {
        for (int j = 0; j < a.size() - i - 1; j++)
        {
            cmp++;
            if (a[j] > a[j + 1])
            {
                swap(a[j], a[j + 1]);
                swaps++;
            }
        }
    }
}
void mergeSort(vector<int>& a, int l, int r,
    long long& cmp, long long& swaps)
{
    if (l >= r)
        return;

    int m = (l + r) / 2;

    mergeSort(a, l, m, cmp, swaps);
    mergeSort(a, m + 1, r, cmp, swaps);

    vector<int> temp;

    int i = l;
    int j = m + 1;

    while (i <= m && j <= r)
    {
        cmp++;

        if (a[i] <= a[j])
            temp.push_back(a[i++]);
        else
            temp.push_back(a[j++]);
    }
    while (i <= m)
        temp.push_back(a[i++]);

    while (j <= r)
        temp.push_back(a[j++]);

    for (int k = 0; k < temp.size(); k++)
    {
        a[l + k] = temp[k];
        swaps++;
    }
}

void countingSort(vector<int>& a, long long& cmp, long long& swaps)
{
    int min = a[0];
    int max = a[0];

    for (int x : a)
    {
        cmp++;

        if (x < min) min = x;
        if (x > max) max = x;
    }

    vector<int> count(max - min + 1, 0);

    for (int x : a)
        count[x - min]++;

    int k = 0;

    for (int i = 0; i < count.size(); i++)
    {
        while (count[i] > 0)
        {
            a[k++] = i + min;
            count[i]--;
            swaps++;
        }
    }
}

vector<int> makeArray(int n, int type)
{
    vector<int> a(n);

    if (type == 0)
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> dist(-50, 50);

        for (int i = 0; i < n; i++)
            a[i] = dist(gen);
    }

    if (type == 1) 
    {
        for (int i = 0; i < n; i++)
            a[i] = i;
    }

    if (type == 2) 
    {
        for (int i = 0; i < n; i++)
            a[i] = n - i;
    }

    return a;
}

int main()
{
    setlocale(LC_ALL, "Ukrainian");
    int sizes[] = { 100, 1000, 10000 };
    for (int n : sizes)
    {
        for (int type = 0; type < 3; type++)
        {
            vector<int> original = makeArray(n, type);

            cout << "\n========================================\n";

            if (type == 0)
                cout << "Випадковий масив, N = " << n << endl;

            if (type == 1)
                cout << "Вiдсортований масив, N = " << n << endl;

            if (type == 2)
                cout << "Зворотний масив, N = " << n << endl;

            cout << "========================================\n";

            // Бульбашкове
            {
                vector<int> a = original;
                long long cmp = 0, swaps = 0;

                auto start = chrono::high_resolution_clock::now();

                bubbleSort(a, cmp, swaps);

                auto finish = chrono::high_resolution_clock::now();

                double time =
                    chrono::duration<double, milli>(finish - start).count();

                cout << "Бульбашкове:  "
                    << time << " мс, "
                    << cmp << " порiвнянь, "
                    << swaps << " перестановок\n";
            }
            {
                vector<int> a = original;
                long long cmp = 0, swaps = 0;
                auto start = chrono::high_resolution_clock::now();
                mergeSort(a, 0, n - 1, cmp, swaps);
                auto finish = chrono::high_resolution_clock::now();
                double time =
                    chrono::duration<double, milli>(finish - start).count();
                    cout << "Злиттям:      "
                    << time << " мс, "
                    << cmp << " порiвнянь, "
                    << swaps << " перестановок\n";
            }
            {
                vector<int> a = original;
                long long cmp = 0, swaps = 0;
                auto start = chrono::high_resolution_clock::now();
                countingSort(a, cmp, swaps);
                auto finish = chrono::high_resolution_clock::now();
                double time = chrono::duration<double, milli>(finish - start).count();

                cout << "Пiдрахунком:  "
                    << time << " мс, "
                    << cmp << " порiвнянь, "
                    << swaps << " перестановок\n";
            }
        }
    }

    return 0;
}
