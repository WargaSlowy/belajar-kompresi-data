#include <bitset>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

class BitFlagRLE {
public:
  std::vector<uint8_t> kompresi(const std::string dataAsli) {
    std::vector<uint8_t> hasil;
    int panjangData = dataAsli.length();
    int indeks = 0;

    std::cout << "mulai dari proses kompresi dari bitflag RLE" << std::endl;
    std::cout << "data input: " << dataAsli << " " << panjangData << " byte"
              << std::endl;

    while (indeks < panjangData) {
      char karakterSekarang = dataAsli[indeks];
      int panjangRun = hitungPanjangRun(dataAsli, indeks);

      std::cout << "\nPosisinya: " << indeks
                << ", karakternya: " << karakterSekarang << std::endl;
      std::cout << " panjang bitflag runnya: " << panjangRun << " karakter"
                << std::endl;

      if (panjangRun > 2) {
        kompresiRun(hasil, karakterSekarang, panjangRun);
        indeks += panjangRun;

        std::cout << "  kita pilih: run encodingnya" << std::endl;
        std::cout << "  byte headernya: "
                  << std::bitset<8>(RUN_FLAG | panjangRun)
                  << " RUN flag + count" << std::endl;
      } else {
        int panjangLiteral = hitungPanjangLiteral(dataAsli, indeks);
        kompresiLiteral(hasil, dataAsli, indeks, panjangLiteral);
        indeks += panjangLiteral;

        std::cout << "  kita pilih: literal encoding" << std::endl;
        std::cout << "  byte headernya: " << std::bitset<8>(panjangLiteral)
                  << " (literal panjang / length)" << std::endl;
      }

      std::cout << " progress: " << indeks << " / " << panjangData
                << " byte yang terproses" << std::endl;
    }

    return hasil;
  }

  std::string dekompresi(const std::vector<uint8_t> &dataTerdekompresi) {
    std::string hasil;
    size_t indeks = 0;

    std::cout << "mulai proses dari dekompresi bitflag RLE" << std::endl;

    while (indeks < dataTerdekompresi.size()) {
      uint8_t byteHeader = dataTerdekompresi[indeks];
      indeks++;

      if (byteHeader & RUN_FLAG) {
        int panjangRun = byteHeader & 0x7F;

        if (indeks >= dataTerdekompresi.size()) {
          std::cerr << "data korup gak jelas" << std::endl;
          break;
        }

        char karakter = static_cast<char>(dataTerdekompresi[indeks]);
        indeks++;

        for (int i = 0; i < panjangRun; i++) {
          hasil += karakter;
        }

        std::cout << "  runnya adalah: " << panjangRun << " x " << karakter
                  << std::endl;
      } else {
        int panjangLiteral = byteHeader;

        for (int i = 0; i < panjangLiteral; i++) {
          if (indeks >= dataTerdekompresi.size()) {
            std::cerr << "data korup gak jelas" << std::endl;
            break;
          }

          hasil += static_cast<char>(dataTerdekompresi[indeks]);
          indeks++;
        }

        std::cout << "  literalnya: " << panjangLiteral << " karakter"
                  << std::endl;
      }
    }

    return hasil;
  }

private:
  const uint8_t RUN_FLAG = 0x80; // 1000 0000
  const uint8_t MAX_RUN_LENGTH = 127;
  const uint8_t MAX_LIT_LENGTH = 127;

  int hitungPanjangRun(const std::string &data, int startPos) {
    char karakterPertama = data[startPos];
    int panjang = 1;

    while (startPos + panjang < data.length() &&
           data[startPos + panjang] == karakterPertama &&
           panjang < MAX_RUN_LENGTH) {
      panjang++;
    }

    return panjang;
  }

  int hitungPanjangLiteral(const std::string &data, int startPos) {
    int panjang = 1;
    int posisiSekarang = startPos;

    while (posisiSekarang + panjang < data.length() &&
           panjang < MAX_LIT_LENGTH) {
      if (posisiSekarang + panjang + 2 < data.length()) {
        char c1 = data[posisiSekarang + panjang];
        char c2 = data[posisiSekarang + panjang + 1];
        char c3 = data[posisiSekarang + panjang + 2];

        if (c1 == c2 && c2 == c3) {
          break;
        }
      }
      panjang++;
    }
    return panjang;
  }

  void kompresiRun(std::vector<uint8_t> &output, char karakter, int panjang) {
    uint8_t headerByte = RUN_FLAG | (panjang & 0x7F);
    output.push_back(headerByte);
    output.push_back(static_cast<uint8_t>(karakter));

    std::cout << "  byte datanya: "
              << std::bitset<8>(static_cast<uint8_t>(karakter)) << " ("
              << karakter << ")" << std::endl;
  }

  void kompresiLiteral(std::vector<uint8_t> &output, const std::string &data,
                       int startPos, int panjang) {
    output.push_back(static_cast<uint8_t>(panjang));

    for (int i = 0; i < panjang; i++) {
      output.push_back(static_cast<uint8_t>(data[startPos + i]));

      std::cout << "  byte liteeral: " << i << ": "
                << std::bitset<8>(static_cast<uint8_t>(data[startPos + i]))
                << " (" << data[startPos + i] << " )" << std::endl;
    }
  }
};

int main() {
  std::cout << "bitflag RLE" << std::endl;

  BitFlagRLE kompressor;

  {
    std::string contohPertama = "AAAABBBCCDDDDDDDD";
    std::vector<uint8_t> terkompresi = kompressor.kompresi(contohPertama);
    std::string terdekompresi = kompressor.dekompresi(terkompresi);

    std::cout << "data aslinya: " << contohPertama << std::endl;
    std::cout << "hasil: " << terdekompresi << std::endl;
  }

  return 0;
}
