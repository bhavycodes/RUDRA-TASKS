#include <iostream>
using namespace std;

enum State
{
    IDLE,
    MOVING,
    AVOID
};

int main()
{
    State state = IDLE;

    bool moveCommand;
    bool obstacle;

    while (true)
    {
        cout << "\nEnter movement command (1 = move, 0 = stop): ";
        cin >> moveCommand;

        cout << "Obstacle detected? (1 = yes, 0 = no): ";
        cin >> obstacle;

        if (!moveCommand)
        {
            state = IDLE;
        }
        else if (obstacle)
        {
            state = AVOID;
        }
        else
        {
            state = MOVING;
        }

        switch (state)
        {
            case IDLE:
                cout << "State: IDLE\n";
                break;

            case MOVING:
                cout << "State: MOVING\n";
                break;

            case AVOID:
                cout << "State: AVOID\n";
                break;
        }
    }

    return 0;
}
