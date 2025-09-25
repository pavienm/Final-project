#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

struct CarVariant {
    string model;
    string variant;
    int engine_cc;
    double km_per_l;
    double rate_per_km;
};

int main() {
    // --- Car data ---
    vector<CarVariant> cars = {
        {"Perodua Bezza", "1.0 G (M)", 998, 22.8, 1.20},
        {"Perodua Bezza", "1.3 X (A)", 1329, 21.0, 1.20},
        {"Proton Saga", "1.3 Standard M/T", 1297, 14.0, 1.20},
        {"Proton Persona", "1.6 Standard CVT", 1597, 15.2, 1.50},
        {"Toyota Vios", "1.3 XLE CVT", 1329, 15.0, 1.50},
        {"Perodua Myvi", "1.3 G (M)", 1297, 15.4, 1.20},
        {"Perodua Myvi", "1.5 X (A)", 1495, 17.5, 1.20}
    };

    cout << "==== Fare Calculator (Console Version) ====\n\n";
    cout << "Available Car Models:\n";
    for (size_t i = 0; i < cars.size(); i++) {
        cout << i+1 << ". " << cars[i].model << " - " << cars[i].variant 
             << " (" << cars[i].engine_cc << "cc, " 
             << fixed << setprecision(1) << cars[i].km_per_l << " km/L, Rate: RM" 
             << fixed << setprecision(2) << cars[i].rate_per_km << "/km)\n";
    }

    int choice;
    cout << "\nSelect a car (1-" << cars.size() << "): ";
    if (!(cin >> choice) || choice < 1 || choice > (int)cars.size()) {
        cout << "Invalid choice!\n";
        return 0;
    }
    CarVariant cv = cars[choice-1];

    double distance;
    unsigned int time;
    char traffic, night;

    cout << "Enter distance (km): ";
    if (!(cin >> distance) || distance < 0) { cout << "Invalid distance!\n"; return 0; }

    cout << "Enter time (minutes): ";
    if (!(cin >> time)) { cout << "Invalid time!\n"; return 0; }

    cout << "Traffic jam? (y/n): ";
    cin >> traffic;
    cout << "Night charge? (y/n): ";
    cin >> night;

    // --- Fare calculation ---
    double fare = (distance * cv.rate_per_km) + (time * 0.10);
    if (traffic == 'y' || traffic == 'Y') fare *= 1.20;
    if (night == 'y' || night == 'Y')     fare *= 1.30;

    cout << "\n==== Fare Result ====\n";
    cout << "Car Model    : " << cv.model << '\n';
    cout << "Variant      : " << cv.variant << '\n';
    cout << "Engine       : " << cv.engine_cc << " cc\n";
    cout << "Fuel Eff.    : " << fixed << setprecision(1) << cv.km_per_l << " km/L\n";
    cout << "Rate/km      : RM" << fixed << setprecision(2) << cv.rate_per_km << '\n';
    cout << "Distance     : " << fixed << setprecision(2) << distance << " km\n";
    cout << "Time         : " << time << " minutes\n";
    cout << "Traffic Jam  : " << ((traffic=='y'||traffic=='Y') ? "Yes" : "No") << '\n';
    cout << "Night Charge : " << ((night=='y'||night=='Y') ? "Yes" : "No") << '\n';
    cout << "-------------------------\n";
    cout << "Total Fare   : RM" << fixed << setprecision(2) << fare << '\n';
    cout << "=========================\n";

    // Save receipt to file (no auto-open)
    ofstream receipt("receipt.txt");
    receipt << "=========================\n";
    receipt << "       Fare Receipt      \n";
    receipt << "=========================\n";
    receipt << "Car Model    : " << cv.model << "\n";
    receipt << "Variant      : " << cv.variant << "\n";
    receipt << "Engine       : " << cv.engine_cc << " cc\n";
    receipt << "Fuel Eff.    : " << fixed << setprecision(1) << cv.km_per_l << " km/L\n";
    receipt << "Rate/km      : RM" << fixed << setprecision(2) << cv.rate_per_km << "\n";
    receipt << "Distance     : " << fixed << setprecision(2) << distance << " km\n";
    receipt << "Time         : " << time << " minutes\n";
    receipt << "Traffic Jam  : " << ((traffic=='y'||traffic=='Y') ? "Yes" : "No") << "\n";
    receipt << "Night Charge : " << ((night=='y'||night=='Y') ? "Yes" : "No") << "\n";
    receipt << "-------------------------\n";
    receipt << "Total Fare   : RM" << fixed << setprecision(2) << fare << "\n";
    receipt << "=========================\n";
    receipt.close();

    cout << "\nReceipt saved as receipt.txt\n";
    return 0;
}
