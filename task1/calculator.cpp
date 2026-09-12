#include <iostream>
using namespace std;

float subtract(float a, float b){
    return a - b;
}

float divide(float a, float b){
    if (b == 0){
        cout << "Error: Cannot divide by zero." << endl;
        return 0;
    }
    return a / b;
}

float addition(float a, float b){
    return a + b;
}

float multiplication(float a, float b){
    return a * b;
}


int main()
{
    int choice;
    int a, b;

    cout << "===== CALCULATOR =====" << endl;
    cout << "1. Addition" << endl;
    cout << "2. Subtraction" << endl;
    cout << "3. Multiplication" << endl;
    cout << "4. Division" << endl;

    cout << "Enter your choice: ";
    cin >> choice;

    cout << "Enter two numbers: ";
    cin >> a >> b;

    switch (choice){
        case 1:
            cout << "Result = " << addition(a, b) << endl;
            break;

        case 2:
            cout << "Result = " << subtract(a, b) << endl;
            break;

        case 3:
            cout << "Result = " << multiplication(a, b) << endl;
            break;

        case 4:
            cout << "Result = " << divide(a, b) << endl;
            break;
        
        default:
            cout << "That operation is not implemented by you yet." << endl;
            break;
    }

    return 0;
}
