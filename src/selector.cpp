#include <iostream>
#include <vector>
#include <string>

class AutonSelector {
private:
    std::vector<std::string> autonOptions;
    int selectedIndex;

public:
    AutonSelector(const std::vector<std::string>& options)
        : autonOptions(options), selectedIndex(0) {}

    void displayOptions() const {
        std::cout << "Autonomous Selector:\n";
        for (size_t i = 0; i < autonOptions.size(); ++i) {
            if (i == selectedIndex)
                std::cout << " > ";
            else
                std::cout << "   ";
            std::cout << autonOptions[i] << "\n";
        }
    }

    void next() {
        selectedIndex = (selectedIndex + 1) % autonOptions.size();
    }

    void previous() {
        selectedIndex = (selectedIndex - 1 + autonOptions.size()) % autonOptions.size();
    }

    std::string getSelected() const {
        return autonOptions[selectedIndex];
    }
};

// Example usage
int main() {
    std::vector<std::string> autons = {
        "Red Left",
        "Red Right",
        "Blue Left",
        "Blue Right",
        "Skills"
    };

    AutonSelector selector(autons);
    char input;
    do {
        selector.displayOptions();
        std::cout << "n: next, p: previous, s: select\n";
        std::cin >> input;
        if (input == 'n') selector.next();
        else if (input == 'p') selector.previous();
    } while (input != 's');

    std::cout << "Selected: " << selector.getSelected() << std::endl;
    return 0;
}