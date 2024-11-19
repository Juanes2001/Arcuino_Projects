#include <iostream>
#include <sstream>
#include <vector>

std::string command = "TASK1 39 40 10";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.println(F(">> Initialized Serial"));
  


}

void loop() {
  // put your main code here, to run repeatedly:

  processCommand(command);

  delay(100);

}



void processCommand(const std::string& command) {
    std::istringstream iss(command);
    std::string task;
    int number;
    std::vector<int> numbers;

    // Extract the task identifier
    iss >> task;

    // Extract the numbers following the task identifier
    while (iss >> number) {
        numbers.push_back(number);
    }

    // Process based on task
    if (task == "TASK1") {
        std::cout << "Processing TASK1 with numbers: ";
        for (int num : numbers) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    } else if (task == "TASK2") {
        std::cout << "Processing TASK2 with numbers: ";
        for (int num : numbers) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "Unknown task." << std::endl;
    }
}