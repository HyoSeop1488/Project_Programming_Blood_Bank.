#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <ctime>

using namespace std;

// ======================== CLASS DEFINITIONS ============================

class KantongDarah {
public:
    string id;
    string golongan;
    string rhesus;
    string tanggalDonasi;
    string tanggalExp;

    KantongDarah(string _id, string _gol, string _rh, string _donasi, string _exp)
        : id(_id), golongan(_gol), rhesus(_rh), tanggalDonasi(_donasi), tanggalExp(_exp) {}
};

class Donor {
public:
    string id;
    string nama;
    int usia;
    int berat;
    string golongan;
    string riwayat;

    Donor(string _id, string _nama, int _usia, int _berat, string _gol, string _riw)
        : id(_id), nama(_nama), usia(_usia), berat(_berat), golongan(_gol), riwayat(_riw) {}
};

class Pasien {
public:
    string id;
    string nama;
    string golongan;
    int kebutuhan;

    Pasien(string _id, string _nama, string _gol, int _keb)
        : id(_id), nama(_nama), golongan(_gol), kebutuhan(_keb) {}
};

// ======================== GLOBAL DATA ============================

vector<Donor> daftarDonor;
vector<KantongDarah> stok;
vector<string> logTransaksi;

// ======================== UTIL FUNCTIONS ============================

string today() {
    time_t t = time(0);
    tm *now = localtime(&t);
    char buf[20];
    strftime(buf, 20, "%Y-%m-%d", now);
    return string(buf);
}

string addDays(string date, int days) {
    tm t = {};
    sscanf(date.c_str(), "%d-%d-%d", &t.tm_year, &t.tm_mon, &t.tm_mday);
    t.tm_year -= 1900;
    t.tm_mon -= 1;

    time_t timeOrig = mktime(&t);
    timeOrig += days * 24 * 3600;

    tm *newTime = localtime(&timeOrig);
    char buf[20];
    strftime(buf, 20, "%Y-%m-%d", newTime);
    return string(buf);
}

bool compatible(string pasien, string donor) {
    if (pasien == donor) return true;

    if (pasien == "A+" && (donor == "A+" || donor == "A-" || donor == "O+" || donor == "O-")) return true;
    if (pasien == "A-" && (donor == "A-" || donor == "O-")) return true;

    if (pasien == "B+" && (donor == "B+" || donor == "B-" || donor == "O+" || donor == "O-")) return true;
    if (pasien == "B-" && (donor == "B-" || donor == "O-")) return true;

    if (pasien == "AB+" ) return true; 
    if (pasien == "AB-" && (donor == "AB-" || donor == "A-" || donor == "B-" || donor == "O-")) return true;

    if (pasien == "O+" && (donor == "O+" || donor == "O-")) return true;
    if (pasien == "O-" && donor == "O-") return true;

    return false;
}

string generateID(string gol, string rh, int urut) {
    return gol + "_" + rh + "_" + to_string(urut);
}

// ======================== FITUR 1: DONASI ============================

void tambahDonasi() {
    cout << "\n=== TRANSAKSI DONASI (BARANG MASUK) ===\n";

    cout << "Pilih Donor berdasarkan index:\n";
    for (int i = 0; i < daftarDonor.size(); i++) {
        cout << i << ". " << daftarDonor[i].nama
             << " (" << daftarDonor[i].golongan << ")\n";
    }

    int idx;
    cout << "Masukkan index donor: ";
    cin >> idx;
    if (idx < 0 || idx >= daftarDonor.size()) {
        cout << "Index tidak valid!\n";
        return;
    }

    Donor d = daftarDonor[idx];

    string gol = d.golongan.substr(0, d.golongan.size() - 1);
    string rh = d.golongan.back() == '+' ? "POS" : "NEG";

    int urut = stok.size() + 1;

    string idkantong = generateID(gol, rh, urut);

    string tDonasi = today();
    string tExp = addDays(tDonasi, 35);

    stok.push_back(KantongDarah(idkantong, gol, rh, tDonasi, tExp));

    cout << "Donasi berhasil! ID Kantong: " << idkantong << endl;

    logTransaksi.push_back("Donasi: " + idkantong + " oleh " + d.nama);
}

// ======================== FITUR 2: PERMINTAAN ============================

void transfusi() {
    cout << "\n=== TRANSAKSI TRANSFUSI (BARANG KELUAR) ===\n";

    string gol;
    int jumlah;

    cout << "Masukkan golongan darah pasien (misal A+): ";
    cin >> gol;
    cout << "Jumlah kebutuhan: ";
    cin >> jumlah;

    vector<int> kandidat;
    for (int i = 0; i < stok.size(); i++) {
        string donor = stok[i].golongan + (stok[i].rhesus == "POS" ? "+" : "-");
        if (compatible(gol, donor)) {
            kandidat.push_back(i);
        }
    }

    if (kandidat.empty()) {
        cout << "Tidak ada stok yang kompatibel!\n";
        return;
    }

    sort(kandidat.begin(), kandidat.end(),
         [&](int a, int b) { return stok[a].tanggalExp < stok[b].tanggalExp; });

    cout << "Kantong darah yang diambil:\n";
    int taken = 0;
    vector<int> toRemove;

    for (int idx : kandidat) {
        if (taken == jumlah) break;

        cout << "- " << stok[idx].id << " (Exp: " << stok[idx].tanggalExp << ")\n";
        logTransaksi.push_back("Transfusi: " + stok[idx].id);

        toRemove.push_back(idx);
        taken++;
    }

    if (taken < jumlah) {
        cout << "Stok tidak mencukupi! Hanya mendapat " << taken << " dari " << jumlah << endl;
    }

    sort(toRemove.rbegin(), toRemove.rend());
    for (int x : toRemove) stok.erase(stok.begin() + x);
}

// ======================== FILE HANDLING ============================

void saveData() {
    ofstream out("output.txt");

    out << "=== STOK DARAH ===\n";
    for (auto &k : stok) {
        out << k.id << " " << k.golongan << " " << k.rhesus
            << " " << k.tanggalDonasi << " " << k.tanggalExp << "\n";
    }

    out << "\n=== LOG TRANSAKSI ===\n";
    for (auto &l : logTransaksi) out << l << "\n";

    out.close();
    cout << "Data berhasil disimpan ke output.txt!\n";
}

// ======================== MENU ============================

void menu() {
    int pilih;
    do {
        cout << "\n=========== BLOOD BANK SYSTEM ===========\n";
        cout << "1. Input Donor\n";
        cout << "2. Transaksi Donasi (Barang Masuk)\n";
        cout << "3. Transaksi Transfusi (Barang Keluar)\n";
        cout << "4. Tampilkan Stok Darah\n";
        cout << "5. Simpan Data ke File\n";
        cout << "0. Keluar\n";
        cout << "Pilih menu: ";
        cin >> pilih;

        if (pilih == 1) {
            string id,nama, gol, riw;
            int usia, berat;
            cout << "ID Donor: "; cin >> id;
            cout << "Nama: "; cin >> nama;
            cout << "Usia: "; cin >> usia;
            cout << "Berat Badan: "; cin >> berat;
            cout << "Golongan Darah (A+/O-/dst): "; cin >> gol;
            cout << "Riwayat Penyakit: "; cin >> riw;

            daftarDonor.push_back(Donor(id,nama,usia,berat,gol,riw));
        }
        else if (pilih == 2) tambahDonasi();
        else if (pilih == 3) transfusi();
        else if (pilih == 4) {
            cout << "\n=== STOK SAAT INI ===\n";
            for (auto &k : stok) {
                cout << k.id << " | " << k.golongan << k.rhesus
                    << " | Donasi: " << k.tanggalDonasi
                    << " | Exp: " << k.tanggalExp << endl;
            }
        }
        else if (pilih == 5) saveData();

    } while (pilih != 0);
}

// ======================== MAIN ============================

int main() {
    menu();
    return 0;
}
