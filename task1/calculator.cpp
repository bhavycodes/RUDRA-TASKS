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



// MAKE THE CODE FOR ADDITION AND DIVISION AND THEN REMOVE THIS COMMENT LINE


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
        case 2:
            cout << "Result = " << subtract(a, b) << endl;
            break;

        case 4:
            cout << "Result = " << divide(a, b) << endl;
            break;
	// add case 1 and case 3 and then remove this comment and change default because i have written that so that i know that has been not implemented, when u finish msg me after pushing, and then we will send it to that bhaiya.
        default:
            cout << "That operation is not implemented by you yet." << endl;
    }

    return 0;
}
