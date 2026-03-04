#include <cctype>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>

constexpr int LINE_LEN = 69;
constexpr int COL_CHECKSUM_POS = 68;


// Checksum Utilities

void fixCheckSum(std::string &line) {
  int sum = 0;

  for (int i = 0; i < COL_CHECKSUM_POS; ++i) {
    char ch = line[i];
    if (std::isdigit(ch))
      sum += ch - '0';
    else if (ch == '-')
      ++sum;
  }

  line[COL_CHECKSUM_POS] = static_cast<char>((sum % 10) + '0');
}

std::string corruptField(const std::string &line, int start, int length,
                         const std::string &replacement) {
  std::string modified = line;
  modified.replace(start, length, replacement);
  fixCheckSum(modified); // recalc checksum
  return modified;
}

// Fault Injection Functions

std::string makeBadChecksum(const std::string &line) {
  std::string modified = line;
  // Flip the last digit to something different
  modified[COL_CHECKSUM_POS] = (line[COL_CHECKSUM_POS] == '0') ? '1' : '0';
  return modified;
}

std::string makeBadLength(const std::string &line, int delta) {
  std::string modified = line;
  if (delta > 0)
    modified.append(delta, ' '); // make it longer
  else if (delta < 0)
    modified.erase(modified.size() + delta, -delta); // make it shorter
  return modified;
}

std::string makeBadInclination(const std::string &line2) {
  // Inclination > 180 to trigger out_of_range
  return corruptField(line2, 8, 8, "181.0000");
}

std::string makeBadEccentricity(const std::string &line2) {
  // Eccentricity >=1 to trigger out_of_range
  return corruptField(line2, 26, 7, "9999999");
}

std::string makeBadMeanMotion(const std::string &line2) {
  // Mean motion <=0 to trigger out_of_range
  return corruptField(line2, 52, 11, "00000000000");
}

// Validation Functions

void validateTLELength(const std::string &line) {
  if (line.length() != LINE_LEN)
    throw std::invalid_argument(
        "TLE line length error: expected 69 characters, got " +
        std::to_string(line.length()));
}

void validateTLECatalogNumbers(const std::string &line1,
                               const std::string &line2) {
  int catnum1 = std::stoi(line1.substr(2, 5));
  int catnum2 = std::stoi(line2.substr(2, 5));

  if (catnum1 != catnum2)
    throw std::invalid_argument("Catalog numbers do not match");
}

void validateTLEChecksum(const std::string &line) {
  validateTLELength(line);

  int expected = line[COL_CHECKSUM_POS] - '0';
  int sum = 0;

  for (int i = 0; i < COL_CHECKSUM_POS; ++i) {
    char ch = line[i];
    if (std::isdigit(ch))
      sum += ch - '0';
    else if (ch == '-')
      ++sum;
  }

  if ((sum % 10) != expected)
    throw std::runtime_error("Checksum mismatch (expected " +
                             std::to_string(expected) + ", computed " +
                             std::to_string(sum % 10) + ")");
}

void validateTLEInclination(const std::string &line2) {
  double inclination = std::stod(line2.substr(8, 8));
  if (inclination < 0.0 || inclination > 180.0)
    throw std::out_of_range("Inclination must be between 0 and 180 degrees");
}

void validateTLEEccentricity(const std::string &line2) {
  double ecc = std::stod("0." + line2.substr(26, 7));
  if (ecc < 0.0 || ecc >= 1.0)
    throw std::out_of_range("Eccentricity must be between 0 and 1");
}

void validateTLEMeanMotion(const std::string &line2) {
  double meanMotion = std::stod(line2.substr(52, 11));
  if (meanMotion <= 0.0)
    throw std::out_of_range("Mean Motion must be greater than 0");
  if (meanMotion > 20.0)
    throw std::out_of_range("Mean Motion is unrealistic");
}

void validateTLEOrbit(const std::string &line1, const std::string &line2) {
  validateTLELength(line1);
  validateTLELength(line2);

  validateTLECatalogNumbers(line1, line2);

  validateTLEChecksum(line1);
  validateTLEChecksum(line2);

  validateTLEMeanMotion(line2);
  validateTLEInclination(line2);
  validateTLEEccentricity(line2);
}

// Interface

void set(const char *const t1, const char *const t2) {
  if (!t1 || !t2)
    throw std::invalid_argument("TLE lines cannot be null");

  std::string sv1(t1, std::strlen(t1));
  std::string sv2(t2, std::strlen(t2));

  validateTLEOrbit(sv1, sv2);
}

// Main / Testing

int main() {
  std::string l1 = "1 37869U 11064C   24303.75000000  .00000002  "
                   "00000-0  49963+1 0 00005";

  std::string l2 = "2 37869 064.9632 093.1153 0023706 247.5214 "
                   "095.2124 02.13101781101065";

  // --- Create test cases ---
  std::string test_bad_checksum = makeBadChecksum(l1);
  std::string test_bad_length = makeBadLength(l1, -1);
  std::string test_bad_incl = makeBadInclination(l2);
  std::string test_bad_ecc = makeBadEccentricity(l2);
  std::string test_bad_mean = makeBadMeanMotion(l2);

  std::string test_bad_catalog_numbers = corruptField(l1, 2, 5, "99999");
  std::string test_bad_catalog_numbers2 = corruptField(l2, 2, 5, "12345");

  // --- Test each case ---
  try {
    set(test_bad_checksum.c_str(), l2.c_str());
  } catch (const std::exception &e) {
    std::cerr << "Bad Checksum: " << e.what() << "\n";
  }

  try {
    set(test_bad_length.c_str(), l2.c_str());
  } catch (const std::exception &e) {
    std::cerr << "Bad Length: " << e.what() << "\n";
  }

  try {
    set(l1.c_str(), test_bad_incl.c_str());
  } catch (const std::exception &e) {
    std::cerr << "Bad Inclination: " << e.what() << "\n";
  }

  try {
    set(l1.c_str(), test_bad_ecc.c_str());
  } catch (const std::exception &e) {
    std::cerr << "Bad Eccentricity: " << e.what() << "\n";
  }

  try {
    set(l1.c_str(), test_bad_mean.c_str());
  } catch (const std::exception &e) {
    std::cerr << "Bad Mean Motion: " << e.what() << "\n";
  }

  try {
    set(test_bad_catalog_numbers.c_str(), test_bad_catalog_numbers2.c_str());
  } catch (const std::exception &e) {
    std::cerr << "Bad Catalog Numbers: " << e.what() << "\n";
  }
    return 0;
  }
