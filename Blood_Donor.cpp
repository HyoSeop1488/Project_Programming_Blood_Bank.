#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <algorithm>
#include <iomanip>

using namespace std;

// ---------------------------------------------------------
// 1. CLASS UTILITY: Untuk menangani Tanggal
// ---------------------------------------------------------
class DateHelper {
public:
    static string getToday() {
        time_t t = time(0);
        tm *now = localtime(&t);
        char buf[20];
        strftime(buf, 20, "%Y-%m-%d", now);
        return string(buf);
    }

    static string addDays(string date, int days) {
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
};

// ---------------------------------------------------------
// 2. CLASS DATA: Menangani Struktur Data User & History
// ---------------------------------------------------------
struct HistoryLog {
    string tempat;
    string tanggalDonor;
    string nextEligible;
};

class User {
private:
    string email;
    string password;
    string nama;
    int usia;
    string golongan;
    string riwayatPenyakit;
    vector<HistoryLog> historyList;

public:
    // Constructor
    User(string e, string p, string n, int u, string g, string r) 
        : email(e), password(p), nama(n), usia(u), golongan(g), riwayatPenyakit(r) {}

    // Getters
    string getEmail() const { return email; }
    string getNama() const { return nama; }
    
    // Validasi Login
    bool checkCredentials(string inputEmail, string inputPass) {
        return (email == inputEmail && password == inputPass);
    }

    // Update Profile
    void updateProfile(string n, int u, string g, string r) {
        nama = n;
        usia = u;
        golongan = g;
        riwayatPenyakit = r;
    }

    // Menambah History Donor
    void addDonationHistory(string tempat) {
        string now = DateHelper::getToday();
        string next = DateHelper::addDays(now, 40); // Asumsi interval 40 hari
        
        historyList.push_back({tempat, now, next});
        
        cout << "Pendaftaran donor berhasil di " << tempat << "!\n";
        cout << "Tanggal Donor: " << now << endl;
        cout << "Boleh donor lagi mulai: " << next << endl;
    }

    // Menampilkan Info User
    void showProfile() const {
        cout << "\n### PROFIL PENGGUNA ###\n";
        cout << "Nama            : " << nama << endl;
        cout << "Usia            : " << usia << endl;
        cout << "Golongan Darah  : " << golongan << endl;
        cout << "Riwayat Penyakit: " << riwayatPenyakit << endl;
    }

    // Menampilkan History
    void showHistory() const {
        cout << "\n### HISTORY DONOR ###\n";
        if (historyList.empty()) {
            cout << "Anda belum pernah donor.\n";
            return;
        }
        for (const auto &h : historyList) {
            cout << "- " << h.tempat 
                 << " | Tgl: " << h.tanggalDonor 
                 << " | Next: " << h.nextEligible << endl;
        }
    }
};

// ---------------------------------------------------------
// 3. CLASS CONTROLLER: Menangani Flow Aplikasi
// ---------------------------------------------------------
class BloodDonationApp {
private:
    vector<User> userDatabase;
    User* currentUser; // Pointer ke user yang sedang login

    // Helper untuk input string dengan spasi
    string inputString(string prompt) {
        string val;
        cout << prompt;
        getline(cin, val);
        return val;
    }

    void registerUser() {
        cout << "\n### SIGN UP ###\n";
        string email, pass, nama, gol, riwayat;
        int usia;

        cout << "Email: "; cin >> email;
        cout << "Password: "; cin >> pass;
        cin.ignore(); // Bersihkan buffer
        nama = inputString("Nama Lengkap: ");
        cout << "Usia: "; cin >> usia;
        cout << "Golongan Darah (A+/O-/dst): "; cin >> gol;
        cin.ignore();
        riwayat = inputString("Riwayat Penyakit ('-' jika aman): ");

        User newUser(email, pass, nama, usia, gol, riwayat);
        userDatabase.push_back(newUser);
        cout << "Akun berhasil dibuat! Silakan login.\n";
    }

    void loginUser() {
        cout << "\n### LOGIN ###\n";
        string email, pass;
        cout << "Email: "; cin >> email;
        cout << "Password: "; cin >> pass;

        for (auto &u : userDatabase) {
            if (u.checkCredentials(email, pass)) {
                currentUser = &u;
                cout << "Login berhasil! Selamat datang, " << currentUser->getNama() << "\n";
                return;
            }
        }
        cout << "Email atau password salah!\n";
    }

    void menuProfile() {
        int pilih;
        do {
            currentUser->showProfile();
            cout << "\n1. Edit Profil\n2. Kembali\nPilih: ";
            cin >> pilih;

            if (pilih == 1) {
                cin.ignore();
                string n = inputString("Nama Baru: ");
                int u; cout << "Usia Baru: "; cin >> u;
                string g; cout << "Golongan Darah Baru: "; cin >> g;
                cin.ignore();
                string r = inputString("Riwayat Penyakit Baru: ");
                
                currentUser->updateProfile(n, u, g, r);
                cout << "Profil berhasil diperbarui!\n";
            }
        } while (pilih != 2);
    }

    void processDonation(string locationName, string contactInfo) {
        cout << "\n### " << locationName << " ###\n";
        cout << "1. Daftar Donor Sekarang\n";
        cout << "2. Kontak Panitia/RS\n";
        cout << "3. Kembali\n";
        cout << "Pilih: ";
        int sub;
        cin >> sub;

        if (sub == 1) {
            currentUser->addDonationHistory(locationName);
        } else if (sub == 2) {
            cout << "Hubungi: " << contactInfo << "\n";
        }
    }

    void menuListLocations(bool isHospital) {
        vector<pair<string, string>> locations;
        string title;

        if (isHospital) {
            title = "CARI RUMAH SAKIT";
            locations = {
                {"RSUD Sardjito", "0812-1111-2222"},
                {"RS Panti Rapih", "0813-3333-4444"},
                {"RS Bethesda", "0812-5555-6666"}
            };
        } else {
            title = "EVENT DONOR";
            locations = {
                {"Donor Darah Kampus UGM", "0896-2222-1111"},
                {"Donor Darah SMAN 1", "0895-4444-3333"},
                {"Donor Darah PMI Kota", "0897-6666-5555"}
            };
        }

        while (true) {
            cout << "\n### " << title << " ###\n";
            for (size_t i = 0; i < locations.size(); ++i) {
                cout << i + 1 << ". " << locations[i].first << endl;
            }
            cout << "4. Kembali\n";
            cout << "Pilih: ";
            int pilih;
            cin >> pilih;

            if (pilih == 4) break;
            if (pilih >= 1 && pilih <= 3) {
                processDonation(locations[pilih-1].first, locations[pilih-1].second);
            } else {
                cout << "Pilihan tidak valid!\n";
            }
        }
    }

    void dashboardUser() {
        int pilih;
        do {
            cout << "\n--- DASHBOARD (" << currentUser->getNama() << ") ---\n";
            cout << "1. Lihat Profil\n";
            cout << "2. Cari RS (Daftar Donor)\n";
            cout << "3. Cari Event Donor\n";
            cout << "4. History Donor\n";
            cout << "5. Logout\n";
            cout << "Pilih: ";
            cin >> pilih;

            switch(pilih) {
                case 1: menuProfile(); break;
                case 2: menuListLocations(true); break; // true for RS
                case 3: menuListLocations(false); break; // false for Event
                case 4: currentUser->showHistory(); break;
                case 5: 
                    cout << "Logout berhasil. Sehat selalu!\n";
                    currentUser = nullptr;
                    break;
                default: cout << "Pilihan tidak valid!\n";
            }
        } while (pilih != 5);
    }

public:
    BloodDonationApp() {
        currentUser = nullptr;
    }

    void run() {
        int pilih;
        while (true) {
            cout << "\n=== APLIKASI DONOR DARAH ===\n";
            cout << "1. Sign Up\n";
            cout << "2. Login\n";
            cout << "3. Keluar\n";
            cout << "Pilih: ";
            cin >> pilih;

            if (pilih == 1) {
                registerUser();
            } else if (pilih == 2) {
                loginUser();
                if (currentUser != nullptr) {
                    dashboardUser();
                }
            } else if (pilih == 3) {
                cout << "Terima kasih!\n";
                break;
            } else {
                cout << "Pilihan tidak valid!\n";
            }
        }
    }
};

// ---------------------------------------------------------
// 4. MAIN PROGRAM
// ---------------------------------------------------------
int main() {
    BloodDonationApp app;
    app.run();
    return 0;
}
