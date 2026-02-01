#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <filesystem>

static const std::string base64Chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

inline bool isBase64(unsigned char c) {
    return (isalnum(c) || c == '+' || c == '/');
}

std::string base64Decode(const std::string &encodedString) {
    int inLen = encodedString.size();
    int i = 0;
    int in = 0;
    unsigned char charArray4[4], charArray3[3];
    std::string result;

    while (inLen-- && encodedString[in] != '=' && isBase64(encodedString[in])) {
        charArray4[i++] = encodedString[in++];
        if (i == 4) {
            for (i = 0; i < 4; i++)
                charArray4[i] = base64Chars.find(charArray4[i]);

            charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
            charArray3[1] = ((charArray4[1] & 0x0f) << 4) + ((charArray4[2] & 0x3c) >> 2);
            charArray3[2] = ((charArray4[2] & 0x03) << 6) + charArray4[3];

            for (i = 0; i < 3; i++)
                result += charArray3[i];

            i = 0;
        }
    }

    if (i) {
        for (int j = i; j < 4; j++)
            charArray4[j] = 0;

        for (int j = 0; j < 4; j++)
            charArray4[j] = base64Chars.find(charArray4[j]);

        charArray3[0] = (charArray4[0] << 2) + ((charArray4[1] & 0x30) >> 4);
        charArray3[1] = ((charArray4[1] & 0x0f) << 4) + ((charArray4[2] & 0x3c) >> 2);
        charArray3[2] = ((charArray4[2] & 0x03) << 6) + charArray4[3];

        for (int j = 0; j < i - 1; j++)
            result += charArray3[j];
    }

    return result;
}

std::vector<std::string> splitCsvLine(const std::string &line) {
    std::vector<std::string> fields;
    std::stringstream stringStream(line);
    std::string item;

    while (std::getline(stringStream, item, ',')) {
        fields.push_back(item);
    }

    return fields;
}

int main() {
    std::ifstream inputFile("listTrimmed.csv");
    if (!inputFile) {
        std::cerr << "Failed to open listTrimmed.csv\n";
        return 1;
    }

    std::filesystem::create_directory("configs");

    std::string line;

    while (std::getline(inputFile, line)) {
        if (line.empty())
            continue;

        auto fields = splitCsvLine(line);
        if (fields.empty())
            continue;

        if (!fields[0].empty() && fields[0][0] == '#')
            continue;

        if (fields.size() < 15)
            continue;

        std::string hostName = fields[0];
        std::string encodedConfig = fields.back();

        std::string decodedConfig = base64Decode(encodedConfig);

        std::ofstream outputFile("configs/" + hostName + ".ovpn");
        outputFile << decodedConfig;
    }
    return 0;
}
