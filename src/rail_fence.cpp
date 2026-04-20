#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// ================= VALIDATION =================
bool is_valid_message(const string &text) {
    for (char c : text) {
        if (!isalpha(static_cast<unsigned char>(c)) && c != ' ') {
            return false;
        }
    }
    return true;
}

// ================= CAESAR CIPHER =================
char shift_char(char c, int shift) {
    if (!isalpha(static_cast<unsigned char>(c))) return c;

    char base = isupper(static_cast<unsigned char>(c)) ? 'A' : 'a';
    shift %= 26;
    if (shift < 0) shift += 26;

    return static_cast<char>((c - base + shift) % 26 + base);
}

string caesar_encrypt(const string &plaintext, int shift) {
    string ciphertext;
    for (char c : plaintext) {
        ciphertext += shift_char(c, shift);
    }
    return ciphertext;
}

string caesar_decrypt(const string &ciphertext, int shift) {
    return caesar_encrypt(ciphertext, -shift);
}

// ================= RAIL FENCE ENCRYPT =================
string rail_fence_encrypt(const string &plaintext, int rails) {
    if (rails <= 1 || plaintext.empty()) return plaintext;

    vector<string> fence(rails, "");
    int rail = 0;
    int direction = 1;

    for (char c : plaintext) {
        fence[rail] += c;

        if (rail == 0) direction = 1;
        else if (rail == rails - 1) direction = -1;

        rail += direction;
    }

    string ciphertext;
    for (const string &row : fence) {
        ciphertext += row;
    }
    return ciphertext;
}

// ================= RAIL FENCE DECRYPT =================
string rail_fence_decrypt(const string &ciphertext, int rails) {
    if (rails <= 1 || ciphertext.empty()) return ciphertext;

    int n = ciphertext.length();
    vector<vector<char>> fence(rails, vector<char>(n, '\n'));

    // Step 1: mark zigzag
    int rail = 0, direction = 1;
    for (int i = 0; i < n; i++) {
        fence[rail][i] = '*';

        if (rail == 0) direction = 1;
        else if (rail == rails - 1) direction = -1;

        rail += direction;
    }

    // Step 2: fill ciphertext
    int index = 0;
    for (int r = 0; r < rails; r++) {
        for (int c = 0; c < n; c++) {
            if (fence[r][c] == '*' && index < n) {
                fence[r][c] = ciphertext[index++];
            }
        }
    }

    // Step 3: read zigzag
    string plaintext;
    rail = 0;
    direction = 1;

    for (int i = 0; i < n; i++) {
        plaintext += fence[rail][i];

        if (rail == 0) direction = 1;
        else if (rail == rails - 1) direction = -1;

        rail += direction;
    }

    return plaintext;
}

// ================= FILE INPUT =================
string read_message_from_file(const string &path) {
    ifstream fin(path);
    if (!fin) {
        cout << "Error: Cannot open file.\n";
        return "";
    }
    string line;
    getline(fin, line);
    return line;
}

// ================= MAIN =================
int main() {
    cout << "=== Cipher Program ===\n";
    cout << "1. Caesar Encrypt\n";
    cout << "2. Caesar Decrypt\n";
    cout << "3. Rail Fence Encrypt\n";
    cout << "4. Rail Fence Decrypt\n";
    cout << "5. Read from file and Rail Fence Encrypt\n";
    cout << "Choose: ";

    int choice;
    cin >> choice;
    cin.ignore();

    string message;
    int key;

    if (choice == 5) {
        message = read_message_from_file("data/input.txt");
        if (message.empty()) return 0;
        cout << "Message from file: " << message << "\n";
    } else {
        cout << "Enter message: ";
        getline(cin, message);
    }

    cout << "Enter key (shift or rails): ";
    cin >> key;

    if (!is_valid_message(message)) {
        cout << "Invalid input. Only letters and spaces are allowed.\n";
        return 0;
    }

    switch (choice) {
        case 1:
            cout << "Ciphertext: " << caesar_encrypt(message, key) << "\n";
            break;
        case 2:
            cout << "Plaintext: " << caesar_decrypt(message, key) << "\n";
            break;
        case 3:
            cout << "Ciphertext: " << rail_fence_encrypt(message, key) << "\n";
            break;
        case 4:
            cout << "Plaintext: " << rail_fence_decrypt(message, key) << "\n";
            break;
        case 5:
            cout << "Ciphertext: " << rail_fence_encrypt(message, key) << "\n";
            break;
        default:
            cout << "Invalid choice.\n";
    }

    return 0;
}