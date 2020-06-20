#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <tuple>
#include <algorithm>
using namespace std;

struct record
{
    int year;
    string vendor;
    string brand;
    double length;
};

/*
  1997,Ford Noah,E350,2.26
  2000,Mercury,Cougar,2.38
 */

void getquotedstring(string& name, istream& fin)
{

    while (true)
    {
        char f = fin.get();
        if (f == '"')
        {
            f = fin.get();
            if (f == '"')
                name.push_back(f);
            else if (f == ',')
                break;
            else
                throw std::invalid_argument("Error");
        }
        else
            name.push_back(f);
    }
}

void getbarestring(string& name, istream& fin)
{
    for (;;)
    {
        char f = fin.get();
        if (f == ',')
            break;
        name.push_back(f);
    }
}
void getstring(string& name, istream& fin)
{
    switch (fin.get())
    {
    case ',': break;
    case '"': getquotedstring(name, fin); break;
    default: fin.unget(); getbarestring(name, fin);
    }
}

record getrecord(istream& fin)
{

    record r;
    fin >> r.year;
    if (fin.get() == ',')
    {
        getstring(r.vendor, fin);
        getstring(r.brand, fin);
    }
    fin >> r.length;
    fin >> ws;
    fin.get();
    if (fin)
        fin.unget();

    return r;
}

vector<record> get_car_records(istream& fin)
{
    vector<record> v;
    while (fin)
    {
        v.push_back(getrecord(fin));
    }
    return v;
}
tuple<int, int> getoldestandnewest(vector<record> const& v)
{
    int y1 = v[0].year;
    int y2 = v[0].year;
    for (size_t i = 1; i < v.size(); i++)
    {
        y1 = min(v[i].year, y1);
        y2 = max(v[i].year, y2);
    }
    return tuple(y1, y2);
}
void outputwithyear(vector<record> const& v, int y)
{
    for (size_t i = 0; i < v.size(); i++)
    {
        if (v[i].year == y)
        {
            cout << y << " " << v[i].vendor << endl;
        }
    }
}
double getaveragelength(vector<record> const& v)
{
    double sum = 0;
    for (size_t i = 0; i < v.size(); i++)
    {
        sum += v[i].length;
    }
    return sum / v.size();
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        cerr << "Error" << endl;
        return 2;
    }

    fstream fin(argv[1]);
    auto v = get_car_records(fin);
    auto [a, b] = getoldestandnewest(v);
    cout << "Oldest: " << endl;
    outputwithyear(v, a);
    cout << "Newest: " << endl;
    outputwithyear(v, b);
    auto ave = getaveragelength(v);
    cout.precision(3);
    cout << "Average " << endl << ave << endl;
}
