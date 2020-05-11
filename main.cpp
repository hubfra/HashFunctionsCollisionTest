
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <ctime>


unsigned int Adler32(const std::string& str);
unsigned int DJB(const std::string& str);
void FillWithRandomStrings(std::vector<std::string>& vec, int amount, int length);
void TestHashFunction(unsigned int (*hash_function)(const std::string&), int amount, int length);


int main()
{
	srand((unsigned int) time(nullptr));

	std::cout << "\nAdler32, 100000 strings 8 characters long\n";
	TestHashFunction(Adler32, 100000, 8);

	std::cout << "\nAdler32, 100000 strings 100 characters long\n";
	TestHashFunction(Adler32, 100000, 100);

	std::cout << "\nDJB, 100000 strings 8 characters long\n";
	TestHashFunction(DJB, 100000, 8);

	std::cout << "\nDJB, 100000 strings 100 characters long\n";
	TestHashFunction(DJB, 100000, 100);
	
	return 0;
}


unsigned int Adler32(const std::string& str)
{
	unsigned int A = 1, B = 0, P = 65521;

	for (char ch : str) {
		A = (A + ch) % P;
		B = (B + A) % P;
	}

	return (B << 16) + A;
}

unsigned int DJB(const std::string& str)
{
	unsigned int hash = 5381;

	for (char ch : str) {
		hash = (hash << 5) + hash + ch;
	}

	return hash;
}

void FillWithRandomStrings(std::vector<std::string>& vec, int amount, int length)
{
	auto random_letter = []() { return rand() % 26 + (65 + (rand() % 2 * 32)); };
	std::string str(length, ' ');

	for (int a = 0; a < amount; ++a) {
		std::generate(str.begin(), str.end(), random_letter);
		vec.push_back(str);
	}
}

void TestHashFunction(unsigned int (*hash_function)(const std::string&), int amount, int length)
{
	std::vector<std::string> random_strings;
	FillWithRandomStrings(random_strings, amount, length);

	std::vector<unsigned int> control_sums;
	std::unordered_set<unsigned int> unique_sums;

	std::for_each(random_strings.begin(), random_strings.end(), [&](std::string& str) { control_sums.push_back(hash_function(str)); });
	std::for_each(control_sums.begin(),   control_sums.end(),   [&](unsigned int sum) { unique_sums.insert(sum); });
	
	int collisions = control_sums.size() - unique_sums.size();
	std::cout << collisions << " collisions\n";

	if (collisions > 0) {
		std::unordered_map<int, int> duplicates;
		for (int a = 0; a < (int)control_sums.size(); ++a) {
			auto it = duplicates.find(control_sums[a]);
			if (it == duplicates.end()) {
				duplicates[control_sums[a]] = a;
			}
			else {
				std::cout << "Example collision:\n";
				std::cout << '<' << random_strings[it->second] << '>' << '\n';
				std::cout << '<' << random_strings[a] << '>' << '\n';
				std::cout << '<' << control_sums[a] << '>' << '\n';
				return;
			}
		}
	}
}