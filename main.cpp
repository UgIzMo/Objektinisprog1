#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <numeric>

using namespace std;

struct Studentas
{
    string vardas;
    string pavarde;
    vector<int> namuDarbai;
    int egzaminas;
    double vidurkis;
    double galutinis;
};

double skaiciuotiVidurki(vector<int>& namuDarbai)
{
    double suma=0.0;
    for(int i=0; i<namuDarbai.size(); i++)
    {
        suma+=namuDarbai[i];
    }
    return suma / namuDarbai.size();
}

double skaiciuotiGalutini(int egzaminas, double vidurkis)
{
    return 0.4 * vidurkis + 0.6 * egzaminas;
}

void spausdinimas(vector<Studentas> studentai)
{
    cout << setw(13) << left << "Pavarde" << setw(13) << left << "Vardas" << setw(13) << left << "Galutinis (vid.)" << endl;
    cout << "----------------------------------------------------------------------" <<endl;
    for(int i=0; i<studentai.size(); i++)
    {
        cout << setw(13) << left << studentai[i].vardas << setw(13) << left << studentai[i].pavarde << setw(13) << left << studentai[i].galutinis << endl;
    }
}

int main()
{
    vector<Studentas> studentai; ///studentu duomenys
    int n; ///studentu sk
    cout << "Kiek yra studentu: ";
    cin>> n;
    for(int i=0; i<n; i++)
    {
        Studentas naujas;

        cout << "Iveskite studento varda: ";
        cin >> naujas.vardas;

        cout << "Iveskite studento pavarde: ";
        cin >> naujas.pavarde;

        naujas.namuDarbai.clear(); /// Isvalome namu darbu pazymiu vektoriu

        int pazymys;
        int m; ///namu darbu pazymiai
        cout << "Kiek yra namu darbu: ";
        cin>> m;

        for(int j=0; j<m; j++)
        {
            cout << j+1 << "-asis namu darbas: ";
            cin >> pazymys;
            naujas.namuDarbai.push_back(pazymys);
        }

        naujas.vidurkis=skaiciuotiVidurki(naujas.namuDarbai);

        cout << "Iveskite egzamino rezultata: ";
        cin >> naujas.egzaminas;

        naujas.galutinis=skaiciuotiGalutini(naujas.egzaminas, naujas.vidurkis);

        studentai.push_back(naujas);

    }
    spausdinimas(studentai);

    return 0;
}
