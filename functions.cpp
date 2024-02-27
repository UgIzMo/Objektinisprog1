#include "functions.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <random>
#include <ctime>
#include <sstream>
#include <chrono>

void nuskaitymas(std::vector<Studentas> &studentai, const std::string &failoPavadinimas)
{
    try
    {
        std::ifstream fd(failoPavadinimas);
        if (!fd.is_open())
        {
            throw std::runtime_error("Failas nerastas.");
        }

        Studentas studentas;
        std::string eilute;
        getline(fd, eilute); // praleidzia pirmaja eil.

        while (getline(fd, eilute))
        {
            std::istringstream eilutesSrautas(eilute);
            eilutesSrautas >> studentas.vardas >> studentas.pavarde;

            int pazymys;
            studentas.namuDarbai.clear();
            while (eilutesSrautas >> pazymys && pazymys != -1)
            {
                studentas.namuDarbai.push_back(pazymys);
            }

            studentas.egzaminas = pazymys;
            studentai.push_back(studentas);
        }

        fd.close();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Klaida skaitymo metu: " << e.what() << std::endl;
    }
}

bool rusiuotiPagalVarda(const Studentas &a, const Studentas &b)
{
    return a.vardas < b.vardas;
}

bool rusiuotiPagalPavarde(const Studentas &a, const Studentas &b)
{
    return a.pavarde < b.pavarde;
}

bool rusiuotiPagalGalutiniVidurki(const Studentas &a, const Studentas &b)
{
    return skaiciuotiGalutini(a.namuDarbai, a.egzaminas, true) < skaiciuotiGalutini(b.namuDarbai, b.egzaminas, true);
}

bool rusiuotiPagalGalutiniMediana(const Studentas &a, const Studentas &b)
{
    return skaiciuotiGalutini(a.namuDarbai, a.egzaminas, false) < skaiciuotiGalutini(b.namuDarbai, b.egzaminas, false);
}

void spausdinimas(const std::vector<Studentas> &studentai, const std::string &isvedimoFailas)
{
    std::ostream &out = isvedimoFailas.empty() ? std::cout : *new std::ofstream(isvedimoFailas);

    /// rusiavimas atitinkamai pagal naudotojo pasirirnkima
    std::vector<Studentas> surusiuotiStudentai = studentai;

    int pasirinkimas;
    std::cout << "Pasirinkite kaip rusiuoti studentus:\n"
              << "1 - Pagal varda\n"
              << "2 - Pagal pavarde\n"
              << "3 - Pagal vidurki\n"
              << "4 - Pagal mediana\n";
    std::cin >> pasirinkimas;

    switch (pasirinkimas)
    {
    case 1:
        std::sort(surusiuotiStudentai.begin(), surusiuotiStudentai.end(), rusiuotiPagalVarda);
        break;
    case 2:
        std::sort(surusiuotiStudentai.begin(), surusiuotiStudentai.end(), rusiuotiPagalPavarde);
        break;
    case 3:
        std::sort(surusiuotiStudentai.begin(), surusiuotiStudentai.end(), rusiuotiPagalGalutiniVidurki);
        break;
    case 4:
        std::sort(surusiuotiStudentai.begin(), surusiuotiStudentai.end(), rusiuotiPagalGalutiniMediana);
        break;
    default:
        std::cout << "Pasirinkimas netinkamas, surusiuota pagal varda.\n";
        std::sort(surusiuotiStudentai.begin(), surusiuotiStudentai.end(), rusiuotiPagalVarda);
    }

    out << std::fixed << std::setprecision(2);
    out << "Studentu galutiniai balai:\n";
    out << "----------------------------------------------------------------\n";
    out << std::left << std::setw(15) << "Vardas" << std::setw(15) << "Pavardė" << std::setw(20) << "Galutinis (Vid.)"
        << std::setw(20) << "Galutinis (Med.)" << std::endl;
    out << "----------------------------------------------------------------\n";

    for (const Studentas &studentas : surusiuotiStudentai)
    {
        double galutinisVidurkis = skaiciuotiGalutini(studentas.namuDarbai, studentas.egzaminas, true);
        double galutineMediana = skaiciuotiGalutini(studentas.namuDarbai, studentas.egzaminas, false);
        out << std::left << std::setw(15) << studentas.vardas << std::setw(15) << studentas.pavarde << std::setw(20)
            << galutinisVidurkis << std::setw(20) << galutineMediana << "\n";
    }
    out << "----------------------------------------------------------------\n";

    if (!isvedimoFailas.empty())
    {
        delete &out;
    }
}

std::string pasirinktiFaila()
{
    std::string failoPavadinimas;

    std::cout << "Pasirinkite faila is kurio norite skaityti duomenis:\n"
              << "1 - kursiokai.txt\n"
              << "2 - studentai10000.txt\n"
              << "3 - studentai100000.txt\n"
              << "4 - studentai1000000.txt\n";

    int pasirinkimas;
    std::cin >> pasirinkimas;

    switch (pasirinkimas)
    {
    case 1:
        failoPavadinimas = "kursiokai.txt";
        break;
    case 2:
        failoPavadinimas = "studentai10000.txt";
        break;
    case 3:
        failoPavadinimas = "studentai100000.txt";
        break;
    case 4:
        failoPavadinimas = "studentai1000000.txt";
        break;
    default:
        std::cout << "Neteisingas pasirinkimas. Skaitoma is kursiokai.txt\n";
        failoPavadinimas = "kursiokai.txt";
    }

    return failoPavadinimas;
}