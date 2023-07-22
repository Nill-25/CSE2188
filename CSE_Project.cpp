#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

const int MAX_CAPACITY = 100;
const int CHARGE_PER_HOUR = 50;

int readParkingStatus(string carNumbers[], string parkingTimes[], int charges[]) {
    ifstream inFile("parking_status.txt");
    int numCars = 0;
    if (inFile.is_open()) {
        inFile >> numCars;
        for (int i = 0; i < numCars; i++) {
            inFile >> carNumbers[i] >> parkingTimes[i] >> charges[i];
        }
        inFile.close();
    }
    return numCars;
}


void updateParkingStatus(int numCars, string carNumbers[], string parkingTimes[], int charges[]) {
    ofstream outFile("parking_status.txt");
    if (outFile.is_open()) {
        outFile << numCars << endl;
        for (int i = 0; i < numCars; i++) {
            outFile << carNumbers[i] << " " << parkingTimes[i] << " " << charges[i] << endl;
        }
        outFile.close();
    }
}

int main() {
    int numCars;
    string carNumbers[MAX_CAPACITY];
    string parkingTimes[MAX_CAPACITY];
    int charges[MAX_CAPACITY];
    numCars = readParkingStatus(carNumbers, parkingTimes, charges);

    int choice;
    string carNumber;
    time_t now;
    struct tm parkingTimeInfo;

    while (true) {
        cout << "\nCar Parking System" << endl;
        cout << "==================" << endl;
        cout << "1. Park a Car" << endl;
        cout << "2. Remove a Car" << endl;
        cout << "3. Check Available Parking Spaces" << endl;
        cout << "4. Calculate Parking Bill" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (numCars < MAX_CAPACITY) {
                    cout << "Enter the car number: ";
                    cin >> carNumber;

                    now = time(0);
                    parkingTimeInfo = *localtime(&now);
                    string parkingTime = asctime(&parkingTimeInfo);

                    carNumbers[numCars] = carNumber;
                    parkingTimes[numCars] = parkingTime;
                    charges[numCars] = 0;
                    numCars++;
                    cout << "Car parked successfully!" << endl;
                } else {
                    cout << "Parking is full. Cannot park a car." << endl;
                }
                break;
            case 2:
                if (numCars > 0) {
                    cout << "Enter the car number to remove: ";
                    cin >> carNumber;
                    bool found = false;
                    for (int i = 0; i < numCars; i++) {
                        if (carNumbers[i] == carNumber) {
                            now = time(0);
                            double parkingDuration = difftime(now, mktime(&parkingTimeInfo)) / 3600;
                            charges[i] = static_cast<int>(parkingDuration) * CHARGE_PER_HOUR;

                            carNumbers[i] = carNumbers[numCars - 1];
                            parkingTimes[i] = parkingTimes[numCars - 1];
                            charges[i] = charges[numCars - 1];
                            numCars--;
                            cout << "Car removed successfully!" << endl;
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        cout << "Car number not found in the parking." << endl;
                    }
                } else {
                    cout << "No car parked. Cannot remove a car." << endl;
                }
                break;
            case 3:
                cout << "Available parking spaces: " << (MAX_CAPACITY - numCars) << endl;
                break;
            case 4:
                if (numCars > 0) {
                    cout << "Enter the car number to calculate parking bill: ";
                    cin >> carNumber;
                    bool found = false;
                    for (int i = 0; i < numCars; i++) {
                        if (carNumbers[i] == carNumber) {
                            now = time(0);
                            double parkingDuration = difftime(now, mktime(&parkingTimeInfo)) / 3600;
                            int totalBill = static_cast<int>(parkingDuration) * CHARGE_PER_HOUR;

                            cout << "Parking bill for Car " << carNumber << ": " << totalBill << " Tk" << endl;
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        cout << "Car number not found in the parking." << endl;
                    }
                } else {
                    cout << "No car parked. Cannot calculate parking bill." << endl;
                }
                break;
            case 5:
                updateParkingStatus(numCars, carNumbers, parkingTimes, charges);
                cout << "Exiting Car Parking System. Have a nice day!" << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
