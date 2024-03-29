#include "functions_old.h"
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
#include <cstdlib>
#include <math.h>

void surusiuotiKategorijas(const std::vector<Studentas> &studentai, std::vector<Studentas> &vargsiukai, std::vector<Studentas> &kietiakai, double &laikas)
{
    auto start = std::chrono::high_resolution_clock::now();

    for (const auto &studentas : studentai)
    {
        double galutinisBalas = skaiciuotiGalutini(studentas.namuDarbai, studentas.egzaminas, true);
        if (galutinisBalas < 5.0)
        {
            vargsiukai.push_back(studentas);
        }
        else
        {
            kietiakai.push_back(studentas);
        }
    }

    auto end = std::chrono::high_resolution_clock::now(); /// baigti laiko skaiciavima
    std::chrono::duration<double> time = end - start;     /// laikas
    laikas = time.count();
    std::cout << "Suskirstymas i vargsiukai.txt ir kietiakai.txt uztruko: " << laikas << " sekundziu." << std::endl;

    start = std::chrono::high_resolution_clock::now();
    
    // didejimo tvarka
    std::sort(kietiakai.begin(), kietiakai.end(), [](const Studentas &a, const Studentas &b)
              { return (0.4 * skaiciuotiVidurki(a.namuDarbai) + 0.6 * a.egzaminas) < (0.4 * skaiciuotiVidurki(b.namuDarbai) + 0.6 * b.egzaminas); });

    std::sort(vargsiukai.begin(), vargsiukai.end(), [](const Studentas &a, const Studentas &b)
              { return (0.4 * skaiciuotiVidurki(a.namuDarbai) + 0.6 * a.egzaminas) < (0.4 * skaiciuotiVidurki(b.namuDarbai) + 0.6 * b.egzaminas); });

    end = std::chrono::high_resolution_clock::now(); /// baigti laiko skaiciavima
    time = end - start;     /// laikas
    laikas = time.count();
    std::cout << "Didejimo tvarka rusiavimas " << laikas << " sekundziu." << std::endl;
}

void irasymasFaile(const std::vector<Studentas> &studentai, const std::string &failoPavadinimas, double &laikas)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::ofstream out(failoPavadinimas);
    if (!out.is_open())
    {
        std::cerr << "Nepavyko atidaryti failo rašymui: " << failoPavadinimas << std::endl;
        return;
    }

    out << std::left << std::setw(15) << "Vardas" << std::setw(15) << "Pavarde" << std::setw(20) << "Galutinis" << std::endl;
    out << "----------------------------------------------------------------\n";
    for (const auto &studentas : studentai)
    {
        double galutinisBalas = skaiciuotiGalutini(studentas.namuDarbai, studentas.egzaminas, true);
        out << std::left << std::setw(15) << studentas.vardas << std::setw(15) << studentas.pavarde << std::setw(15) << std::fixed << std::setprecision(2) << galutinisBalas << std::endl;
    }
    out.close();
    auto end = std::chrono::high_resolution_clock::now(); /// baigti laiko skaiciavima
    std::chrono::duration<double> time = end - start;     /// laikas
    laikas = time.count();
}

void rusiuotiStudentusIrIrasymas(const std::vector<Studentas> &studentai, double &laikas)
{
    std::vector<Studentas> vargsiukai, kietiakai;
    surusiuotiKategorijas(studentai, vargsiukai, kietiakai, laikas);
    auto start = std::chrono::high_resolution_clock::now();

    irasymasFaile(vargsiukai, "vargsiukai.txt", laikas);
    irasymasFaile(kietiakai, "kietiakai.txt", laikas);
    auto end = std::chrono::high_resolution_clock::now(); /// baigti laiko skaiciavima
    std::chrono::duration<double> time = end - start;     /// laikas
    laikas = time.count();
}

void generuotiStudentuFailus(const std::vector<int> &sizes)
{
    double laikas;
    for (int size : sizes)
    {
        auto start = std::chrono::high_resolution_clock::now();
        std::string fileName = "studentai" + std::to_string(size) + ".txt";
        std::ofstream outFile(fileName);
        // stringstream temp; headerio vietoj outfile ->temp - kitam labore irgi stringstream
        outFile << std::left << std::setw(15) << "Vardas" << std::setw(15) << "Pavardė";
        for (int i = 1; i <= 15; ++i)
        {
            outFile << std::setw(10) << "ND" + std::to_string(i);
        }
        outFile << std::setw(10) << "Egz." << std::endl;

        for (int i = 1; i <= size; i++)
        {
            std::string vardas = "Vardas" + std::to_string(i);
            std::string pavarde = "Pavardė" + std::to_string(i);

            outFile << std::left << std::setw(15) << vardas << std::setw(15) << pavarde;
            for (int j = 0; j < 15; j++)
            {
                outFile << std::setw(10) << (rand() % 10 + 1);
            }
            outFile << std::setw(10) << (rand() % 10 + 1);
            outFile << std::endl;
        }

        outFile.close();

        auto end = std::chrono::high_resolution_clock::now(); /// baigti laiko skaiciavima
        std::chrono::duration<double> time = end - start;     /// laikas
        laikas = time.count();

        std::cout << "Sekmingai sugeneruotas failas: " << fileName << ". Failo generavimas uztruko: " << laikas << " sekundziu. " << std::endl;
    }
}

void nuskaitymas(std::vector<Studentas> &studentai, const std::string &failoPavadinimas, double &laikas)
{
    auto start = std::chrono::high_resolution_clock::now();
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
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        double time = duration.count();
        laikas = time;
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
    out << std::left << std::setw(15) << "Vardas" << std::setw(15) << "Pavarde" << std::setw(20) << "Galutinis (Vid.)"
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
