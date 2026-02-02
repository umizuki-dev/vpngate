#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct CSVData {
  std::vector<std::string> headerLines;
  std::vector<std::vector<std::string>> rows;
};

CSVData readCSV(const std::string& filename) {
  std::ifstream file(filename);
  CSVData result;
  std::string line;


  /* header is first two lines */
  std::getline(file, line);
  result.headerLines.push_back(line);

  std::getline(file, line);
  result.headerLines.push_back(line);

  while (std::getline(file, line)) {
    std::vector<std::string> row;
    std::string cell;
    std::stringstream ss(line);

    while (std::getline(ss, cell, ',')) {
      row.push_back(cell);
    }
    result.rows.push_back(row);
  }

  return result;
}

void trimCSV(const std::string& filename, const CSVData& csv) {
  std::ofstream file(filename);

  for (const auto& header : csv.headerLines) {
    file << header << '\n';
  }


  /* 
   * Conditions:
   * 1. Server in Japan
   * 2. HostName does not start with "public-"
   */
  for (const auto& row : csv.rows) {
    if (row.size() > 5 &&
        row[5] == "Japan" &&
        row[0].substr(0, 7) != "public-" &&
        std::stod(row[4]) > 500000000.0) {

      for (size_t i = 0; i < row.size(); ++i) {
        file << row[i];
        if (i < row.size() - 1) {
          file << ",";
        }
      }
      file << '\n';
    }
  }
}

int main() {
  const std::string listFile = "list.csv";
  const std::string trimmedFile = "listTrimmed.csv";

  CSVData csv = readCSV(listFile);
  trimCSV(trimmedFile, csv);

  return 0;
}
