#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cstdlib>
#include <io.h>
#include <fcntl.h>

class MemoryGame {
private:
    static int gameIdCounter;
    int currentGameId;
    std::vector<int> ledSequence;
    std::vector<int> userSequence;
    int totalLEDs;
    int ledsHit;
    bool success;
    std::chrono::time_point<std::chrono::steady_clock> gameStartTime;
    const int TIME_LIMIT_SECONDS = 30;
    const int GRID_SIZE = 8; // 8 LEDs (positions 1-8)

    // 🔽 Read last Game ID from CSV
    int getLastGameIdFromCSV() {
        std::ifstream file("data/memory_game_data.csv");
        std::string line;
        int lastId = 0;

        if (file.is_open()) {
            std::getline(file, line);
            while (std::getline(file, line)) {
                std::stringstream ss(line);
                std::string idStr;
                std::getline(ss, idStr, ',');

                try {
                    int id = std::stoi(idStr);
                    if (id > lastId) {
                        lastId = id;
                    }
                } catch (...) {
                    continue;
                }
            }
        }

        return lastId;
    }
    
public:
    MemoryGame() {
        checkAndCreateCSV();

        // Initialize Game ID from file if needed
        if (gameIdCounter == 0) {
            gameIdCounter = getLastGameIdFromCSV();
        }

        currentGameId = ++gameIdCounter;
        totalLEDs = 0;
        ledsHit = 0;
        success = false;
    }
    
    void checkAndCreateCSV() {
        std::ifstream file("data/memory_game_data.csv");
        if (!file.is_open()) {
            // Create data folder if it doesn't exist
            system("mkdir -p data");
            
            // Create CSV file with headers
            std::ofstream csvFile("data/memory_game_data.csv");
            if (csvFile.is_open()) {
                csvFile << "Game ID,Total LEDs,LEDs Hit,Success\n";
                csvFile.close();
                std::cout << " CSV file created successfully!\n";
            }
        }
        file.close();
    }
    
    void playSound(const std::string& soundType) {
        if (soundType == "led_on") {
            std::cout << " *BEEP* ";
        } else if (soundType == "correct") {
            std::cout << " *TICK* ";
        } else if (soundType == "wrong") {
            std::cout << " *BUZZ*  ";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    
    void displayGrid(const std::vector<int>& activeLEDs = {}) {
        std::cout << "      MEMORY GAME GRID       \n";
        // std::cout << "\n┌─────────────────────────────┐\n";
        // std::cout << "│      MEMORY GAME GRID       │\n";
        // std::cout << "└─────────────────────────────┘\n";
        
        // Display 2x4 grid
        for (int row = 0; row < 2; row++) {
            std::cout << "  ";
            for (int col = 0; col < 4; col++) {
                int ledNumber = row * 4 + col + 1;
                bool isActive = std::find(activeLEDs.begin(), activeLEDs.end(), ledNumber) != activeLEDs.end();
                
                if (isActive) {
                    std::cout << "ON[" << ledNumber << "]ON  ";
                } else {
                    std::cout << "OFF[" << ledNumber << "]OFF  ";
                }
            }
            std::cout << "\n\n";
        }
        // std::cout << "─────────────────────────────────\n";
    }
    
    void generateRandomSequence() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> lengthDist(2, 8);
        std::uniform_int_distribution<> ledDist(1, 8);
        
        totalLEDs = lengthDist(gen);
        ledSequence.clear();
        
        for (int i = 0; i < totalLEDs; i++) {
            int led = ledDist(gen);
            ledSequence.push_back(led);
        }
        
        std::cout << "New game sequence generated with " << totalLEDs << " LEDs!\n";
    }
    
    void showSequence() {
        std::cout << "\n Watch the sequence carefully!\n";
        std::cout << "Press Enter to start...";
        std::cin.ignore();
        std::cin.get();
        
        for (int i = 0; i < ledSequence.size(); i++) {
            // system("clear"); // Use "cls" on Windows
            std::cout << "\n\n";
            std::cout << " Game ID: " << currentGameId << " | Sequence: " << (i + 1) << "/" << totalLEDs << "\n";
            
            std::vector<int> currentLED = {ledSequence[i]};
            displayGrid(currentLED);
            
            playSound("led_on");
            std::cout << "LED " << ledSequence[i] << " is ON!\n";
            
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        
        // system("clear");
        std::cout << "\n\n";
        std::cout << " Game ID: " << currentGameId << " | Now it's your turn!\n";
        displayGrid();
    }
    
    bool getUserInput() {
        userSequence.clear();
        gameStartTime = std::chrono::steady_clock::now();
        
        std::cout << "\n You have " << TIME_LIMIT_SECONDS << " seconds to repeat the sequence!\n";
        std::cout << "Enter the LED numbers (1-8) in the correct order:\n";
        std::cout << "Type the numbers separated by spaces, then press Enter\n";
        std::cout << "Example: 3 1 7 2\n\n";
        std::cout << "Your input: ";
        
        std::string input;
        std::getline(std::cin, input);
        
        // Check time limit
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - gameStartTime).count();
        
        if (elapsedTime > TIME_LIMIT_SECONDS) {
            std::cout << "\n TIME'S UP! Game over.\n";
            playSound("wrong");
            return false;
        }
        
        // Parse input
        std::stringstream ss(input);
        std::string token;
        while (std::getline(ss, token, ' ')) {
            if (!token.empty()) {
                try {
                    int led = std::stoi(token);
                    if (led >= 1 && led <= 8) {
                        userSequence.push_back(led);
                    }
                } catch (const std::exception& e) {
                    // Invalid input, ignore
                }
            }
        }
        
        return true;
    }
    
    void checkSequence() {
        ledsHit = 0;
        
        std::cout << "\n Checking your sequence...\n";
        std::cout << "Expected: ";
        for (int led : ledSequence) {
            std::cout << led << " ";
        }
        std::cout << "\nYour input: ";
        for (int led : userSequence) {
            std::cout << led << " ";
        }
        std::cout << "\n\n";
        
        // Count correct LEDs in correct positions
        int minSize = std::min(ledSequence.size(), userSequence.size());
        for (int i = 0; i < minSize; i++) {
            if (ledSequence[i] == userSequence[i]) {
                ledsHit++;
                playSound("correct");
                std::cout << "Position " << (i + 1) << ": *TICK* Correct!\n";
            } else {
                playSound("wrong");
                std::cout << "Position " << (i + 1) << ": *BUZZ* Expected " << ledSequence[i] << ", got " << userSequence[i] << "\n";
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        
        // Check if sequence lengths match
        if (userSequence.size() != ledSequence.size()) {
            std::cout << "\n  Sequence length mismatch! Expected " << ledSequence.size() << " LEDs, got " << userSequence.size() << "\n";
        }
        
        success = (ledsHit == totalLEDs && userSequence.size() == ledSequence.size());
        
        std::cout << "\n RESULTS:\n";
        // std::cout << "═════════════════════════════\n";
        std::cout << " Game ID: " << currentGameId << "\n";
        std::cout << " Total LEDs: " << totalLEDs << "\n";
        std::cout << " LEDs Hit: " << ledsHit << "\n";
        std::cout << " Success: " << (success ? "YES" : "NO") << "\n";
        std::cout << " Accuracy: " << std::fixed << std::setprecision(1) << ((float)ledsHit / totalLEDs * 100) << "%\n";
        // std::cout << "═════════════════════════════\n";
        
        if (success) {
            std::cout << "\n CONGRATULATIONS! Perfect sequence! 🎉\n";
            playSound("correct");
        } else {
            std::cout << "\n Good try! Practice makes perfect!\n";
        }
    }
    
    void saveToCSV() {
        std::ofstream csvFile("data/memory_game_data.csv", std::ios::app);
        if (csvFile.is_open()) {
            csvFile << currentGameId << "," << totalLEDs << "," << ledsHit << "," << (success ? "true" : "false") << "\n";
            csvFile.close();
            std::cout << "\n Game data saved to CSV file!\n";
        } else {
            std::cout << "\n Error saving data to CSV file!\n";
        }
    }
    
    void displayCSVData() {
        std::ifstream csvFile("data/memory_game_data.csv");
        if (csvFile.is_open()) {
            std::cout << "\n GAME HISTORY:\n";
            // std::cout << "════════════════════════════════════════\n";
            
            std::string line;
            bool isFirstLine = true;
            while (std::getline(csvFile, line)) {
                if (isFirstLine) {
                    std::cout << "| " << std::setw(8) << "Game ID" << " | " 
                              << std::setw(10) << "Total LEDs" << " | " 
                              << std::setw(9) << "LEDs Hit" << " | " 
                              << std::setw(7) << "Success" << " |\n";
                    std::cout << "|----------|------------|-----------|----------|\n";
                    isFirstLine = false;
                    continue;
                }
                
                std::stringstream ss(line);
                std::string gameId, totalLeds, ledsHit, success;
                
                std::getline(ss, gameId, ',');
                std::getline(ss, totalLeds, ',');
                std::getline(ss, ledsHit, ',');
                std::getline(ss, success, ',');
                
                std::cout << "| " << std::setw(8) << gameId << " | " 
                          << std::setw(10) << totalLeds << " | " 
                          << std::setw(9) << ledsHit << " | " 
                          << std::setw(7) << success << " |\n";
            }
            // std::cout << "════════════════════════════════════════\n";
            csvFile.close();
        }
    }
    
    void playGame() {
        // system("clear"); // Use "cls" on Windows
        
        std::cout << "ARDUINO MEMORY GAME SIMULATOR \n";
        // std::cout << "═══════════════════════════════════════\n";
        std::cout << "Welcome to the virtual memory game!\n";
        std::cout << "This simulates the exact Arduino hardware experience.\n\n";
        
        generateRandomSequence();
        showSequence();
        
        if (getUserInput()) {
            checkSequence();
            saveToCSV();
        } else {
            // Time limit exceeded
            success = false;
            ledsHit = 0;
            saveToCSV();
        }
        
        displayCSVData();
        
        std::cout << "\n Would you like to play again? (y/n): ";
        char choice;
        std::cin >> choice;
        std::cin.ignore();
        
        if (choice == 'y' || choice == 'Y') {
            playGame();
        } else {
            std::cout << "\n Thanks for playing! Check your data folder for game statistics.\n";
            std::cout << "Data saved in: data/memory_game_data.csv\n";
        }
    }
};
int getLastGameIdFromCSV() {
    std::ifstream file("data/memory_game_data.csv");
    std::string line;
    int lastId = 0;

    if (file.is_open()) {
        // Skip header
        std::getline(file, line);

        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string idStr;
            std::getline(ss, idStr, ',');

            try {
                int id = std::stoi(idStr);
                if (id > lastId) {
                    lastId = id;
                }
            } catch (...) {
                continue; // skip malformed lines
            }
        }
    }

    return lastId;
}

// Static member initialization
int MemoryGame::gameIdCounter = 0;

int main() {
    std::cout << "Starting Memory Game Simulator...\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    
    MemoryGame game;
    game.playGame();
    
    return 0;
}
