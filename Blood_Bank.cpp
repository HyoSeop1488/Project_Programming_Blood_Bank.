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
    string nextEligible;
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
    cout << "Riwayat Penyakit ('-' jika tidak ada): ";
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

// ========================= PROFILE =============================

void menuProfile() {
    int pilih;

    do {
        cout << "\n=== PROFIL ===\n";
        cout << "Nama: " << currentUser->nama << endl;
        cout << "Usia: " << currentUser->usia << endl;
        cout << "Golongan Darah: " << currentUser->golongan << endl;
        cout << "Riwayat Penyakit: " << currentUser->riwayatPenyakit << endl;

        cout << "\n1. Edit Profil\n";
        cout << "2. Kembali\n";
        cout << "Pilih: ";
        cin >> pilih;

        if (pilih == 1) {
            cout << "Nama Baru: ";
            cin.ignore();
            getline(cin, currentUser->nama);

            cout << "Usia Baru: ";
            cin >> currentUser->usia;

            cout << "Golongan Darah Baru: ";
            cin >> currentUser->golongan;

            cout << "Riwayat Penyakit Baru: ";
            cin.ignore();
            getline(cin, currentUser->riwayatPenyakit);

            cout << "Profil berhasil diperbarui!\n";
        }

    } while (pilih != 2);
}

// ========================= DAFTAR RS =============================

void daftarRSLangsung(string namaRS) {
    string now = today();
    string next = addDays(now, 40);

    currentUser->riwayat.push_back({namaRS, now, next});

    cout << "Pendaftaran donor berhasil di " << namaRS << "!\n";
    cout << "Tanggal Donor: " << now << endl;
    cout << "Boleh donor lagi mulai: " << next << endl;
}

void cariRS() {
    vector<string> rs = {
        "RSUD Sardjito",
        "RS Panti Rapih",
        "RS Bethesda"
    };

    vector<string> kontak = {
        "0812-1111-2222",
        "0813-3333-4444",
        "0812-5555-6666"
    };

    int pilih;

    while (true) {
        cout << "\n=== CARI RUMAH SAKIT ===\n";
        for (int i = 0; i < rs.size(); i++)
            cout << i+1 << ". " << rs[i] << endl;
        cout << "4. Kembali\n";
        cout << "Pilih: ";
        cin >> pilih;

        if (pilih == 4) break;
        if (pilih < 1 || pilih > 3) {
            cout << "Pilihan tidak valid!\n";
            continue;
        }

        int idx = pilih - 1;

        cout << "\n=== " << rs[idx] << " ===\n";
        cout << "1. Daftar Donor Sekarang\n";
        cout << "2. Kontak Rumah Sakit (" << kontak[idx] << ")\n";
        cout << "3. Kembali\n";
        cout << "Pilih: ";
        int sub;
        cin >> sub;

        if (sub == 1) daftarRSLangsung(rs[idx]);
        else if (sub == 2)
            cout << "Anda dapat menghubungi: " << kontak[idx] << "\n";
        else if (sub == 3)
            continue;
        else
            cout << "Pilihan tidak valid!\n";
    }
}

// ========================= EVENT DONOR =============================

void daftarEventLangsung(string event) {
    string now = today();
    string next = addDays(now, 40);

    currentUser->riwayat.push_back({event, now, next});

    cout << "Pendaftaran donor berhasil untuk event '" << event << "'!\n";
    cout << "Tanggal Donor: " << now << endl;
    cout << "Boleh donor lagi mulai: " << next << endl;
}

void eventDonor() {
    vector<string> event = {
        "Donor Darah Kampus UGM",
        "Donor Darah SMAN 1",
        "Donor Darah PMI Kota"
    };

    vector<string> kontak = {
        "0896-2222-1111",
        "0895-4444-3333",
        "0897-6666-5555"
    };

    int pilih;

    while (true) {
        cout << "\n=== EVENT DONOR ===\n";
        for (int i = 0; i < event.size(); i++)
            cout << i+1 << ". " << event[i] << endl;
        cout << "4. Kembali\n";
        cout << "Pilih: ";
        cin >> pilih;

        if (pilih == 4) break;
        if (pilih < 1 || pilih > 3) {
            cout << "Pilihan tidak valid!\n";
            continue;
        }

        int idx = pilih - 1;

        cout << "\n=== " << event[idx] << " ===\n";
        cout << "1. Daftar Donor Sekarang\n";
        cout << "2. Kontak Panitia (" << kontak[idx] << ")\n";
        cout << "3. Kembali\n";
        cout << "Pilih: ";
        int sub;
        cin >> sub;

        if (sub == 1) daftarEventLangsung(event[idx]);
        else if (sub == 2)
            cout << "Anda dapat menghubungi: " << kontak[idx] << "\n";
        else if (sub == 3)
            continue;
        else
            cout << "Pilihan tidak valid!\n";
    }
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
        cout << "1. Profil\n";
        cout << "2. Cari RS (Daftar Donor)\n";
        cout << "3. Event Donor\n";
        cout << "4. History Donor\n";
        cout << "5. Logout\n";
        cout << "Pilih: ";
        cin >> pilih;

        if (pilih == 1) menuProfile();
        else if (pilih == 2) cariRS();
        else if (pilih == 3) eventDonor();
        else if (pilih == 4) lihatHistory();
        else if (pilih == 5) {
            cout << "\nSehat selalu wahai pejuang darah!\n";
            currentUser = nullptr;
        }
    } while (pilih != 5);
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
