#include "functions_vector.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <chrono>

void readDataVector(std::vector<Studentas> &studentai, const std::string &failoVardas)
{
    std::ifstream file(failoVardas);
    if (!file)
    {
        std::cerr << "Nepavyko atidaryti failo: " << failoVardas << std::endl;
        return; 
    }

    Studentas studentas;
    std::string line;
    while (getline(file, line))
    {
        if (line.empty())
            continue; 

        std::istringstream iss(line);
        iss >> studentas.vardas >> studentas.pavarde;

        int pazymys;
        studentas.namuDarbai.clear();
        while (iss >> pazymys)
        {
            studentas.namuDarbai.push_back(pazymys);
        }

        if (!studentas.namuDarbai.empty())
        {
            studentas.egzaminas = studentas.namuDarbai.back();
            studentas.namuDarbai.pop_back();
            studentai.push_back(studentas);
        }
    }
    file.close();
}

void generateStudentFilesVector(int size)
{
    std::string fileName = "studentai" + std::to_string(size) + ".txt";
    std::ofstream outFile(fileName);

    if (!outFile)
    {
        std::cerr << "Nepavyko atidaryti failo: " << fileName << std::endl;
        return;
    }

    outFile << std::left << std::setw(15) << "Vardas" << std::setw(15) << "Pavardė";
    for (int i = 1; i <= 15; ++i)
    {
        outFile << std::setw(10) << "ND" + std::to_string(i);
    }
    outFile << std::setw(10) << "Egz." << std::endl;

    for (int i = 1; i <= size; i++)
    {
        outFile << std::left << std::setw(15) << "Vardas" + std::to_string(i)
                << std::setw(15) << "Pavardė" + std::to_string(i);
        for (int j = 0; j < 15; j++)
        {
            outFile << std::setw(10) << (rand() % 10 + 1);
        }
        outFile << std::setw(10) << (rand() % 10 + 1);
        outFile << std::endl;
    }

    outFile.close();
}


// 1 strategija
void rusiuotStudentusVector1(const std::string &failoVardas) {
    std::vector<Studentas> studentai;

    // Start timer for reading data
    auto readStart = std::chrono::high_resolution_clock::now();
    readDataVector(studentai, failoVardas);
    auto readEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> readElapsed = readEnd - readStart;
    std::cout << "Skaitymas uztruko: " << readElapsed.count() << "s\n";

    // Start timer for sorting data
    auto sortStart = std::chrono::high_resolution_clock::now();
    std::sort(studentai.begin(), studentai.end(), [](const Studentas &a, const Studentas &b)
              { return (0.4 * skaiciuotiVidurki(a.namuDarbai) + 0.6 * a.egzaminas) < (0.4 * skaiciuotiVidurki(b.namuDarbai) + 0.6 * b.egzaminas); });
    auto sortEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> sortElapsed = sortEnd - sortStart;
    std::cout << "Rusiavimas uztruko: " << sortElapsed.count() << "s\n";

    // Start timer for dividing students
    auto divideStart = std::chrono::high_resolution_clock::now();
    std::vector<Studentas> kietiakai, vargsiukai;
    for (const auto &studentas : studentai) {
        double galutinisBalas = 0.4 * skaiciuotiVidurki(studentas.namuDarbai) + 0.6 * studentas.egzaminas;
        if (galutinisBalas < 5.0) {
            vargsiukai.push_back(studentas);
        } else {
            kietiakai.push_back(studentas);
        }
    }
    auto divideEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> divideElapsed = divideEnd - divideStart;
    std::cout << "Studentu skirstymas uztruko: " << divideElapsed.count() << "s\n";

    // Writing sorted students into separate files
    std::ofstream kietiakaiFile("kietiakai.txt"), vargsiukaiFile("vargsiukai.txt");

    for (const auto &studentas : kietiakai) {
        kietiakaiFile << studentas.vardas << " " << studentas.pavarde << " "
                       << std::fixed << std::setprecision(2)
                       << (0.4 * skaiciuotiVidurki(studentas.namuDarbai) + 0.6 * studentas.egzaminas)
                       << std::endl;
    }

    for (const auto &studentas : vargsiukai) {
        vargsiukaiFile << studentas.vardas << " " << studentas.pavarde << " "
                       << std::fixed << std::setprecision(2)
                       << (0.4 * skaiciuotiVidurki(studentas.namuDarbai) + 0.6 * studentas.egzaminas)
                       << std::endl;
    }

    kietiakaiFile.close();
    vargsiukaiFile.close();
}

//2 startegija
void rusiuotStudentusVector2(const std::string &failoVardas)
{
    std::vector<Studentas> studentai;

    // Start timer for reading data
    auto readStart = std::chrono::high_resolution_clock::now();
    readDataVector(studentai, failoVardas);
    auto readEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> readElapsed = readEnd - readStart;
    std::cout << "Skaitymas uztruko: " << readElapsed.count() << "s\n";

    // Start timer for sorting data
    auto sortStart = std::chrono::high_resolution_clock::now();
    std::sort(studentai.begin(), studentai.end(), [](const Studentas &a, const Studentas &b)
              { return (0.4 * skaiciuotiVidurki(a.namuDarbai) + 0.6 * a.egzaminas) < (0.4 * skaiciuotiVidurki(b.namuDarbai) + 0.6 * b.egzaminas); });
    auto sortEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> sortElapsed = sortEnd - sortStart;
    std::cout << "Rusiavimas uztruko: " << sortElapsed.count() << "s\n";

    // Start timer for dividing students
    auto divideStart = std::chrono::high_resolution_clock::now();
    std::vector<Studentas> vargsiukai;
    auto it = std::remove_if(studentai.begin(), studentai.end(), [&vargsiukai](const Studentas &studentas) {
        double galutinisBalas = 0.4 * skaiciuotiVidurki(studentas.namuDarbai) + 0.6 * studentas.egzaminas;
        if (galutinisBalas < 5.0)
        {
            vargsiukai.push_back(studentas);
            return true;
        }
        return false;
    });
    studentai.erase(it, studentai.end());
    auto divideEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> divideElapsed = divideEnd - divideStart;
    std::cout << "Studentu skirstymas uztruko: " << divideElapsed.count() << "s\n";

    // Writing sorted students into separate files
    std::ofstream kietiakaiFile("kietiakai.txt"), vargsiukaiFile("vargsiukai.txt");

    for (const auto &studentas : studentai)
    {
        kietiakaiFile << studentas.vardas << " " << studentas.pavarde << " "
                      << std::fixed << std::setprecision(2)
                      << (0.4 * skaiciuotiVidurki(studentas.namuDarbai) + 0.6 * studentas.egzaminas)
                      << std::endl;
    }

    for (const auto &studentas : vargsiukai)
    {
        vargsiukaiFile << studentas.vardas << " " << studentas.pavarde << " "
                       << std::fixed << std::setprecision(2)
                       << (0.4 * skaiciuotiVidurki(studentas.namuDarbai) + 0.6 * studentas.egzaminas)
                       << std::endl;
    }

    kietiakaiFile.close();
    vargsiukaiFile.close();
}


//3 strategija
void rusiuotStudentusVector3(const std::string &failoVardas)
{
    std::vector<Studentas> studentai;

    // Start timer for reading data
    auto readStart = std::chrono::high_resolution_clock::now();
    readDataVector(studentai, failoVardas);
    auto readEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> readElapsed = readEnd - readStart;
    std::cout << "Skaitymas uztruko (Vector): " << readElapsed.count() << "s\n";

    // Start timer for sorting data
    auto sortStart = std::chrono::high_resolution_clock::now();
    std::sort(studentai.begin(), studentai.end(), [](const Studentas &a, const Studentas &b)
              { return (0.4 * skaiciuotiVidurki(a.namuDarbai) + 0.6 * a.egzaminas) < (0.4 * skaiciuotiVidurki(b.namuDarbai) + 0.6 * b.egzaminas); });
    auto sortEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> sortElapsed = sortEnd - sortStart;
    std::cout << "Rusiavimas uztruko (Vector): " << sortElapsed.count() << "s\n";

    // Start timer for dividing students
    auto divideStart = std::chrono::high_resolution_clock::now();
    std::vector<Studentas> kietiakai, vargsiukai;
    auto iter = std::stable_partition(studentai.begin(), studentai.end(), [](const Studentas &studentas)
                                      { return 0.4 * skaiciuotiVidurki(studentas.namuDarbai) + 0.6 * studentas.egzaminas >= 5.0; });
    kietiakai.assign(studentai.begin(), iter);
    vargsiukai.assign(iter, studentai.end());
    auto divideEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> divideElapsed = divideEnd - divideStart;
    std::cout << "Skirstymas uztruko (Vector): " << divideElapsed.count() << "s\n";

    // Writing sorted students into separate files
    std::ofstream kietiakaiFile("kietiakai.txt"), vargsiukaiFile("vargsiukai.txt");

    for (const auto &studentas : kietiakai)
    {
        kietiakaiFile << studentas.vardas << " " << studentas.pavarde << " "
                      << std::fixed << std::setprecision(2)
                      << (0.4 * skaiciuotiVidurki(studentas.namuDarbai) + 0.6 * studentas.egzaminas)
                      << std::endl;
    }

    for (const auto &studentas : vargsiukai)
    {
        vargsiukaiFile << studentas.vardas << " " << studentas.pavarde << " "
                       << std::fixed << std::setprecision(2)
                       << (0.4 * skaiciuotiVidurki(studentas.namuDarbai) + 0.6 * studentas.egzaminas)
                       << std::endl;
    }

    kietiakaiFile.close();
    vargsiukaiFile.close();
}
