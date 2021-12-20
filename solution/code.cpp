#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <utility> // std::pair
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream

std::vector<std::pair<std::string, std::vector<int>>> read_csv(std::string filename) {

	// Creating vector to store the result
	std::vector<std::pair<std::string, std::vector<int>>> result;

	// Create an input filestream
	//std::ifstream myFile(filename);
	std::ifstream myFile(filename);
	constexpr size_t bufferSize = 1024 * 1024;
	std::unique_ptr<char[]> buffer(new char[bufferSize]);
	while (myFile)
	{
		myFile.read(buffer.get(), bufferSize);
		// process data in buffer
	}


	// Check if the file is open
	if (!myFile.is_open()) throw std::runtime_error("Could not open file");

	// Helping variables
	std::string line, colname;
	int value;

	// Read column names
	if (myFile.good()) {
		// Extract the first line
		std::getline(myFile, line);

		// Create a stringstream
		std::stringstream ss(line);

		// Extract each column name
		while (std::getline(ss, colname, ',')) {
			// Initialize and add <colname, int vector> pairs to result
			result.push_back({ colname, std::vector<int>{} });
		}
	}

	// Read data
	while (std::getline(myFile, line)) {
		// Create a stringstream of the current line
		std::stringstream ss(line);

		// Keep track of the current column index
		int colIdx = 0;

		// Extract each integer
		while (ss >> value) {

			// Add the current integer to the 'colIdx' column's values vector
			result.at(colIdx).second.push_back(value);

			// If the next token is a comma, ignore it and move on
			if (ss.peek() == ',') ss.ignore();

			// Increment the column index
			colIdx++;
		}
	}

	// Close file
	myFile.close();

	return result;
}

void process_input(std::vector<std::pair<std::string, std::vector<int>>> result) {
	std::vector<std::pair<std::string, std::vector<int>>> ids = read_csv("ids.csv");

	std::ofstream myFile("Result.csv");

	// Create column names
	myFile << "id" << "," << "last-durations" << "," << "longest-durations";

	// Go to next row
	myFile << "\n";

	// Make unordered map to store all IDs in key and all durations to a vector
	std::map<int, std::vector<int>> inputs_in_map;
	for (size_t i = 0; i < result.at(0).second.size(); ++i) {
		inputs_in_map[result.at(0).second.at(i)].push_back(result.at(1).second.at(i));
	}

	// Store all IDs from ids.cvs to a vector
	std::vector<int> ID_in_vector;
	for (size_t i = 0; i < ids.at(0).second.size(); ++i) {
		ID_in_vector.push_back(ids.at(0).second.at(i));
	}

	// Process input to make last-durations and longest-durations values
	for (auto& item : inputs_in_map) {
		// Check if the current item in unordered map is available in the ID_in_vector 
		if (std::find(ID_in_vector.begin(), ID_in_vector.end(), item.first) != ID_in_vector.end()) {
			// Store the name and shift to next cell
			myFile << item.first << ",";
			// Store last-durations values
			for (size_t i = 1; i <= 30; i++) {
				myFile << item.second.at(item.second.size() - i);
				if (i != 30) myFile << "|"; // No | at the end
			}
			// Sort durations vector to get longest duration
			std::sort(item.second.begin(), item.second.end());
			// Shift to next cell
			myFile << ",";
			// Store longest-durations values
			for (size_t i = 1; i <= 30; i++) {
				myFile << item.second.at(item.second.size() - i);
				if (i != 30) myFile << "|"; // No | at the end
			}
			// Go to next row
			myFile << "\n";
		}
	}

	// Close file
	myFile.close();
}

int main() {
	// Read input
	std::vector<std::pair<std::string, std::vector<int>>> input = read_csv("input0.csv");

	// Process input to get the result file
	process_input(input);

	return 0;
}
