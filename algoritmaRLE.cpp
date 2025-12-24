#include <cctype>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

std::string kompresiRLE(const std::string &dataAsli) {
  if (dataAsli.empty()) {
    return "";
  }

  std::stringstream hasilKompresi;
  int jumlahKarakter = dataAsli.length();
  int indeks = 0;

  while (indeks < jumlahKarakter) {
    char karakterSekarang = dataAsli[indeks];
    int hitungPengulangan = 1;

    while (indeks + 1 < jumlahKarakter &&
           dataAsli[indeks + 1] == karakterSekarang) {
      hitungPengulangan++;
      indeks++;
    }

    std::cout << "ditemukan " << hitungPengulangan << " x " << karakterSekarang
              << " " << std::endl;
    hasilKompresi << hitungPengulangan << karakterSekarang;
    indeks++;
  }
  return hasilKompresi.str();
}

std::string dekompresRLE(const std::string &dataTerkompresi) {
  std::stringstream hasilDekompresi;
  int jumlahKarakter = dataTerkompresi.length();
  int indeks = 0;

  while (indeks < jumlahKarakter) {
    std::string jumlahStr = "";
    while (indeks < jumlahKarakter && isdigit(dataTerkompresi[indeks])) {
      jumlahStr += dataTerkompresi[indeks];
      indeks++;
    }

    if (indeks >= jumlahKarakter) {
      std::cerr << "format kompresi salah" << std::endl;
      break;
    }

    char karakter = dataTerkompresi[indeks];
    int jumlah = std::stoi(jumlahStr);

    std::cout << "balikin: " << jumlah << " x " << karakter << std::endl;

    for (int i = 0; i < jumlah; i++) {
      hasilDekompresi << karakter;
    }
    indeks++;
  }

  return hasilDekompresi.str();
}

void hitungRasioKompresiData(const std::string dataAsli, const std::string dataTerkompresi) {
  int ukuranAsli = dataAsli.length();
  int ukuranTerkompresi = dataTerkompresi.length();

  double rasioKompresi = (1.0 - (double)ukuranTerkompresi / ukuranAsli) - 100;

  std::cout << "ukuran aslinya adalah: " << ukuranAsli << " byte" << std::endl;
  std::cout << "ukuran terkompresi: " << ukuranTerkompresi << " byte" << std::endl;

  std::cout << "rasio kompresi sebesar: " << rasioKompresi << " persen" << std::endl;

  if (rasioKompresi > 0) {
    std::cout << "hemat " << (ukuranTerkompresi - ukuranAsli) <<  " byte" << std::endl;
  } else if (rasioKompresi < 0) {
    std::cout << "kompresi membuat data lebih gede" << std::endl;
  } else {
    std::cout << "tidak ada perubahan kompresi sama sekali" << std::endl;
  }
}

void contoh() {
  std::vector<std::string> contohData = {
      "AAAABBBBBCCEEEFFFFFFFGGGGG",
      "SSAAWWIITT",
  };

  std::vector<std::string> namaContoh = {
      "Contoh 1: data ulang sederahana",
      "Contoh 2: data ulang sederhana",
  };

  for (size_t i = 0; i < contohData.size(); i++) {
    std::cout << "\n" << namaContoh[i] << std::endl;
    std::cout << "data asli: " << contohData[i] << std::endl;

    std::string hasilKompresi = kompresiRLE(contohData[i]);
    std::cout << "hasil kompresi: " << hasilKompresi << std::endl;

    std::string hasilDekompresi = dekompresRLE(hasilKompresi);
    std::cout << "hasil dekompresi: " << hasilDekompresi << std::endl;

    hitungRasioKompresiData(contohData[i], hasilKompresi);
  }
}

int main() {
  contoh();

  return 0;
}
