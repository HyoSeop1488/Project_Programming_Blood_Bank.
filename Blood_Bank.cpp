#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>

using namespace std;

// ========================= STRUCT USER & HISTORY =============================

struct History {
    string tempat;
    string tanggalDonor;
    string nextEligible; // tanggal boleh donor lagi (40 hari)
};

struct User {
    string email;
    string password;
    string nama;
    int usia;
    string golongan;
    string riwayatPenyakit;

    vector<History> riwayat;
};

// ========================= GLOBAL DATA =============================
vector<User> users;
User* currentUser = nullptr;

// ========================= UTIL FUNCTIONS =============================

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

    time_t base = mktime(&t);
    base += days * 24 * 3600;

    tm *newT = localtime(&base);
    char buf[20];
    strftime(buf, 20, "%Y-%m-%d", newT);
    return string(buf);
}

User* findUser(string email, string pass) {
    for (auto &u : users)
        if (u.email == email && u.password == pass)
            return &u;
    return nullptr;
}

// ========================= SIGN UP =============================

void signUp() {
    cout << "\n=== SIGN UP ===\n";

    User u;
    cout << "Email: ";
    cin >> u.email;
    cout << "Password: ";
    cin >> u.password;
    cout << "Nama Lengkap: ";
    cin.ignore();
    getline(cin, u.nama);
    cout << "Usia: ";
    cin >> u.usia;
    cout << "Golongan Darah (A+/O-/dst): ";
    cin >> u.golongan;
    cout << "Riwayat Penyakit (tulis '-' bila tidak ada): ";
    cin.ignore();
    getline(cin, u.riwayatPenyakit);

    users.push_back(u);

    cout << "Akun berhasil dibuat!\n";
}

// ========================= LOGIN =============================

void login() {
    cout << "\n=== LOGIN ===\n";

    string email, pass;
    cout << "Email: ";
    cin >> email;
    cout << "Password: ";
    cin >> pass;

    User* u = findUser(email, pass);
    if (u == nullptr) {
        cout << "Email atau password salah!\n";
        return;
    }

    currentUser = u;
    cout << "Login berhasil! Selamat datang, " << u->nama << "\n";
}

// ========================= DAFTAR RS =============================

void daftarRS() {
    cout << "\n=== CARI RUMAH SAKIT ===\n";
    vector<string> rs = {
        "RSUD Sardjito",
        "RS Panti Rapih",
        "RS Bethesda"
    };

    for (int i = 0; i < rs.size(); i++)
        cout << i+1 << ". " << rs[i] << endl;

    int pilih;
    cout << "Pilih RS: ";
    cin >> pilih;
    if (pilih < 1 || pilih > rs.size()) {
        cout << "Pilihan tidak valid!\n";
        return;
    }

    string now = today();
    string next = addDays(now, 40);

    currentUser->riwayat.push_back({rs[pilih-1], now, next});

    cout << "Pendaftaran berhasil!\n";
    cout << "Tanggal Donor: " << now << endl;
    cout << "Boleh donor lagi mulai: " << next << endl;
}

// ========================= DAFTAR EVENT =============================

void daftarEvent() {
    cout << "\n=== EVENT DONOR DARAH ===\n";
    vector<string> event = {
        "Donor Darah Kampus UGM",
        "Donor Darah SMAN 1",
        "Donor Darah PMI Kota"
    };

    for (int i = 0; i < event.size(); i++)
        cout << i+1 << ". " << event[i] << endl;

    int pilih;
    cout << "Pilih Event: ";
    cin >> pilih;

    if (pilih < 1 || pilih > event.size()) {
        cout << "Pilihan tidak valid!\n";
        return;
    }

    string now = today();
    string next = addDays(now, 40);

    currentUser->riwayat.push_back({event[pilih-1], now, next});

    cout << "Pendaftaran berhasil!\n";
    cout << "Tanggal Donor: " << now << endl;
    cout << "Boleh donor lagi mulai: " << next << endl;
}

// ========================= HISTORY =============================

void lihatHistory() {
    cout << "\n=== HISTORY DONOR ===\n";

    if (currentUser->riwayat.empty()) {
        cout << "Belum pernah donor.\n";
        return;
    }

    for (auto &h : currentUser->riwayat) {
        cout << "- " << h.tempat
             << " | Donor: " << h.tanggalDonor
             << " | Boleh donor lagi: " << h.nextEligible << endl;
    }
}

// ========================= MENU REGISTERED =============================

void menuRegistered() {
    int pilih;
    do {
        cout << "\n=== MENU UTAMA ===\n";
        cout << "1. Cari RS (Daftar Donor)\n";
        cout << "2. Event Donor Darah\n";
        cout << "3. History Donor\n";
        cout << "4. Logout\n";
        cout << "Pilih: ";
        cin >> pilih;

        if (pilih == 1) daftarRS();
        else if (pilih == 2) daftarEvent();
        else if (pilih == 3) lihatHistory();
        else if (pilih == 4) {
            cout << "\nSehat selalu wahai pejuang darah!\n";
            currentUser = nullptr;
        }
    } while (pilih != 4);
}

// ========================= MAIN MENU =============================

int main() {
    int pilih;

    while (true) {
        cout << "\n=== APLIKASI DONOR DARAH ===\n";
        cout << "1. Sign Up\n";
        cout << "2. Login\n";
        cout << "3. Keluar\n";
        cout << "Pilih: ";
        cin >> pilih;

        if (pilih == 1) signUp();
        else if (pilih == 2) {
            login();
            if (currentUser != nullptr) menuRegistered();
        }
        else if (pilih == 3) {
            cout << "Program selesai. Terima kasih!\n";
            break;
        }
        else
            cout << "Pilihan tidak valid!\n";
    }

    return 0;
}
