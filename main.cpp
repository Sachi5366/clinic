// clinic.cpp
// Clinic Management System (CLI)
// Compile: g++ -std=c++17 clinic.cpp -o clinic
// Run: ./clinic
//
// Simple file-based persistence (patients.txt, appointments.txt).
// Fields are stored as pipe-separated values. This is a teaching/demo project.

#include <bits/stdc++.h>
using namespace std;

static const string PATIENTS_FILE = "patients.txt";
static const string APPOINTMENTS_FILE = "appointments.txt";

struct Patient {
    int id;
    string name;
    int age;
    string gender;
    string contact;
    string notes;
};

struct Appointment {
    int id;
    int patient_id;
    string doctor;
    string date; // yyyy-mm-dd
    string time; // HH:MM
    string reason;
};

// Utility: escape pipe char in text
string escapeField(const string &s) {
    string out;
    for (char c : s) {
        if (c == '|') out += "\\|";
        else if (c == '\n') out += "\\n";
        else out += c;
    }
    return out;
}

// Unescape
string unescapeField(const string &s) {
    string out;
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '\\' && i + 1 < s.size()) {
            if (s[i+1] == '|') { out += '|'; ++i; continue; }
            if (s[i+1] == 'n') { out += '\n'; ++i; continue; }
        }
        out += s[i];
    }
    return out;
}

vector<string> splitLine(const string &line) {
    vector<string> parts;
    string cur;
    bool esc = false;
    for (size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (esc) {
            cur.push_back(c);
            esc = false;
        } else if (c == '\\') {
            esc = true;
        } else if (c == '|') {
            parts.push_back(cur);
            cur.clear();
        } else {
            cur.push_back(c);
        }
    }
    parts.push_back(cur);
    return parts;
}

// Load patients
vector<Patient> loadPatients() {
    vector<Patient> v;
    ifstream ifs(PATIENTS_FILE);
    if (!ifs) return v;
    string line;
    while (getline(ifs, line)) {
        if (line.empty()) continue;
        auto f = splitLine(line);
        if (f.size() < 6) continue;
        Patient p;
        p.id = stoi(unescapeField(f[0]));
        p.name = unescapeField(f[1]);
        p.age = stoi(unescapeField(f[2]));
        p.gender = unescapeField(f[3]);
        p.contact = unescapeField(f[4]);
        p.notes = unescapeField(f[5]);
        v.push_back(p);
    }
    return v;
}

// Save patients
void savePatients(const vector<Patient>& v) {
    ofstream ofs(PATIENTS_FILE, ios::trunc);
    for (const auto &p : v) {
        ofs << escapeField(to_string(p.id)) << "|"
            << escapeField(p.name) << "|"
            << escapeField(to_string(p.age)) << "|"
            << escapeField(p.gender) << "|"
            << escapeField(p.contact) << "|"
            << escapeField(p.notes) << "\n";
    }
}

// Load appointments
vector<Appointment> loadAppointments() {
    vector<Appointment> v;
    ifstream ifs(APPOINTMENTS_FILE);
    if (!ifs) return v;
    string line;
    while (getline(ifs, line)) {
        if (line.empty()) continue;
        auto f = splitLine(line);
        if (f.size() < 6) continue;
        Appointment a;
        a.id = stoi(unescapeField(f[0]));
        a.patient_id = stoi(unescapeField(f[1]));
        a.doctor = unescapeField(f[2]);
        a.date = unescapeField(f[3]);
        a.time = unescapeField(f[4]);
        a.reason = unescapeField(f[5]);
        v.push_back(a);
    }
    return v;
}

// Save appointments
void saveAppointments(const vector<Appointment>& v) {
    ofstream ofs(APPOINTMENTS_FILE, ios::trunc);
    for (const auto &a : v) {
        ofs << escapeField(to_string(a.id)) << "|"
            << escapeField(to_string(a.patient_id)) << "|"
            << escapeField(a.doctor) << "|"
            << escapeField(a.date) << "|"
            << escapeField(a.time) << "|"
            << escapeField(a.reason) << "\n";
    }
}

int nextPatientId(const vector<Patient>& v) {
    int m = 0;
    for (auto &p : v) m = max(m, p.id);
    return m + 1;
}
int nextAppointmentId(const vector<Appointment>& v) {
    int m = 0;
    for (auto &a : v) m = max(m, a.id);
    return m + 1;
}

// Simple helpers
string inputLine(const string &prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}
int inputInt(const string &prompt) {
    while (true) {
        string s = inputLine(prompt);
        try {
            return stoi(s);
        } catch (...) {
            cout << "Invalid number. Try again.\n";
        }
    }
}

// Display patient brief
void showPatientBrief(const Patient &p) {
    cout << "ID: " << p.id << " | Name: " << p.name << " | Age: " << p.age << " | Gender: " << p.gender << " | Contact: " << p.contact << "\n";
}

void showPatientFull(const Patient &p) {
    cout << "----- Patient -----\n";
    cout << "ID      : " << p.id << "\n";
    cout << "Name    : " << p.name << "\n";
    cout << "Age     : " << p.age << "\n";
    cout << "Gender  : " << p.gender << "\n";
    cout << "Contact : " << p.contact << "\n";
    cout << "Notes   : " << p.notes << "\n";
    cout << "-------------------\n";
}

void showAppointment(const Appointment &a) {
    cout << "Appt ID: " << a.id << " | Patient ID: " << a.patient_id << " | Doctor: " << a.doctor
         << " | Date: " << a.date << " | Time: " << a.time << " | Reason: " << a.reason << "\n";
}

// CRUD operations
void addPatient(vector<Patient>& patients) {
    Patient p;
    p.id = nextPatientId(patients);
    cout << "Adding new patient (ID " << p.id << ")\n";
    cout << "Name: "; getline(cin, p.name);
    p.age = inputInt("Age: ");
    cout << "Gender: "; getline(cin, p.gender);
    cout << "Contact: "; getline(cin, p.contact);
    cout << "Notes: "; getline(cin, p.notes);
    patients.push_back(p);
    savePatients(patients);
    cout << "Patient added.\n";
}

void listPatients(const vector<Patient>& patients) {
    cout << "=== Patients (" << patients.size() << ") ===\n";
    for (const auto &p : patients) showPatientBrief(p);
}

Patient* findPatientById(vector<Patient>& patients, int id) {
    for (auto &p : patients) if (p.id == id) return &p;
    return nullptr;
}

vector<Patient*> searchPatientsByName(vector<Patient>& patients, const string &term) {
    vector<Patient*> res;
    string low = term;
    transform(low.begin(), low.end(), low.begin(), ::tolower);
    for (auto &p : patients) {
        string name = p.name;
        transform(name.begin(), name.end(), name.begin(), ::tolower);
        if (name.find(low) != string::npos) res.push_back(&p);
    }
    return res;
}

void editPatient(vector<Patient>& patients) {
    int id = inputInt("Enter patient ID to edit: ");
    Patient* p = findPatientById(patients, id);
    if (!p) { cout << "Patient not found.\n"; return; }
    showPatientFull(*p);
    cout << "Leave blank to keep current value.\n";
    string s;
    cout << "Name (" << p->name << "): "; getline(cin, s); if (!s.empty()) p->name = s;
    cout << "Age (" << p->age << "): "; getline(cin, s); if (!s.empty()) p->age = stoi(s);
    cout << "Gender (" << p->gender << "): "; getline(cin, s); if (!s.empty()) p->gender = s;
    cout << "Contact (" << p->contact << "): "; getline(cin, s); if (!s.empty()) p->contact = s;
    cout << "Notes (" << p->notes << "): "; getline(cin, s); if (!s.empty()) p->notes = s;
    savePatients(patients);
    cout << "Patient updated.\n";
}

void deletePatient(vector<Patient>& patients, vector<Appointment>& appointments) {
    int id = inputInt("Enter patient ID to delete: ");
    auto it = remove_if(patients.begin(), patients.end(), [&](const Patient &p){ return p.id == id; });
    if (it == patients.end()) { cout << "No such patient.\n"; return; }
    patients.erase(it, patients.end());
    // remove related appointments
    auto it2 = remove_if(appointments.begin(), appointments.end(), [&](const Appointment &a){ return a.patient_id == id; });
    if (it2 != appointments.end()) {
        appointments.erase(it2, appointments.end());
        saveAppointments(appointments);
    }
    savePatients(patients);
    cout << "Patient and related appointments removed.\n";
}

void addAppointment(vector<Appointment>& appointments, const vector<Patient>& patients) {
    Appointment a;
    a.id = nextAppointmentId(appointments);
    cout << "Adding appointment (ID " << a.id << ")\n";
    int pid = inputInt("Patient ID: ");
    bool found = false;
    for (const auto &p : patients) if (p.id == pid) { found = true; break; }
    if (!found) { cout << "No patient with that ID. Cancelled.\n"; return; }
    a.patient_id = pid;
    cout << "Doctor: "; getline(cin, a.doctor);
    cout << "Date (YYYY-MM-DD): "; getline(cin, a.date);
    cout << "Time (HH:MM): "; getline(cin, a.time);
    cout << "Reason: "; getline(cin, a.reason);
    appointments.push_back(a);
    saveAppointments(appointments);
    cout << "Appointment scheduled.\n";
}

void listAppointments(const vector<Appointment>& appointments) {
    cout << "=== Appointments (" << appointments.size() << ") ===\n";
    for (const auto &a : appointments) showAppointment(a);
}

Appointment* findAppointmentById(vector<Appointment>& appointments, int id) {
    for (auto &a : appointments) if (a.id == id) return &a;
    return nullptr;
}

void editAppointment(vector<Appointment>& appointments) {
    int id = inputInt("Enter appointment ID to edit: ");
    Appointment* a = findAppointmentById(appointments, id);
    if (!a) { cout << "Appointment not found.\n"; return; }
    cout << "Leave blank to keep current value.\n";
    string s;
    cout << "Doctor (" << a->doctor << "): "; getline(cin, s); if (!s.empty()) a->doctor = s;
    cout << "Date (" << a->date << "): "; getline(cin, s); if (!s.empty()) a->date = s;
    cout << "Time (" << a->time << "): "; getline(cin, s); if (!s.empty()) a->time = s;
    cout << "Reason (" << a->reason << "): "; getline(cin, s); if (!s.empty()) a->reason = s;
    saveAppointments(appointments);
    cout << "Appointment updated.\n";
}

void deleteAppointment(vector<Appointment>& appointments) {
    int id = inputInt("Enter appointment ID to delete: ");
    auto it = remove_if(appointments.begin(), appointments.end(), [&](const Appointment &a){ return a.id == id; });
    if (it == appointments.end()) { cout << "No such appointment.\n"; return; }
    appointments.erase(it, appointments.end());
    saveAppointments(appointments);
    cout << "Appointment deleted.\n";
}

// Export CSVs
void exportPatientsCSV(const vector<Patient>& patients, const string &filename) {
    ofstream ofs(filename);
    ofs << "id,name,age,gender,contact,notes\n";
    for (auto &p : patients) {
        // simple CSV escaping: wrap in quotes and replace " with ""
        auto esc = [](const string &s)->string {
            string t = s;
            size_t pos = 0;
            while ((pos = t.find('"', pos)) != string::npos) { t.insert(pos, "\""); pos += 2; }
            return "\"" + t + "\"";
        };
        ofs << p.id << "," << esc(p.name) << "," << p.age << "," << esc(p.gender) << "," << esc(p.contact) << "," << esc(p.notes) << "\n";
    }
    cout << "Patients exported to " << filename << "\n";
}

void exportAppointmentsCSV(const vector<Appointment>& appointments, const string &filename) {
    ofstream ofs(filename);
    ofs << "id,patient_id,doctor,date,time,reason\n";
    for (auto &a : appointments) {
        auto esc = [](const string &s)->string {
            string t = s;
            size_t pos = 0;
            while ((pos = t.find('"', pos)) != string::npos) { t.insert(pos, "\""); pos += 2; }
            return "\"" + t + "\"";
        };
        ofs << a.id << "," << a.patient_id << "," << esc(a.doctor) << "," << esc(a.date) << "," << esc(a.time) << "," << esc(a.reason) << "\n";
    }
    cout << "Appointments exported to " << filename << "\n";
}

void showMenu() {
    cout << "\n=== Clinic Management Menu ===\n";
    cout << "1) Add patient\n2) List patients\n3) Search patient by name\n4) Edit patient\n5) Delete patient\n";
    cout << "6) Add appointment\n7) List appointments\n8) Edit appointment\n9) Delete appointment\n";
    cout << "10) Export patients CSV\n11) Export appointments CSV\n0) Exit\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    auto patients = loadPatients();
    auto appointments = loadAppointments();

    cout << "Welcome to Clinic Management System (simple CLI)\n";

    while (true) {
        showMenu();
        int choice = inputInt("Choose an option: ");
        string dummy;
        getline(cin, dummy); // consume newline

        switch (choice) {
            case 1: addPatient(patients); break;
            case 2: listPatients(patients); break;
            case 3: {
                string term = inputLine("Enter name search term: ");
                auto res = searchPatientsByName(patients, term);
                if (res.empty()) cout << "No patients found.\n";
                else {
                    cout << res.size() << " results:\n";
                    for (auto p : res) showPatientFull(*p);
                }
                break;
            }
            case 4: editPatient(patients); break;
            case 5: deletePatient(patients, appointments); break;
            case 6: addAppointment(appointments, patients); break;
            case 7: listAppointments(appointments); break;
            case 8: editAppointment(appointments); break;
            case 9: deleteAppointment(appointments); break;
            case 10: exportPatientsCSV(patients, "patients_export.csv"); break;
            case 11: exportAppointmentsCSV(appointments, "appointments_export.csv"); break;
            case 0:
                cout << "Saving and exiting...\n";
                savePatients(patients);
                saveAppointments(appointments);
                return 0;
            default:
                cout << "Unknown choice.\n";
        }
    }

    return 0;
}
