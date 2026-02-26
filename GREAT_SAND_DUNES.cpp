#include <string>
#include <cctype>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <cmath>

constexpr double MU_EARTH = 398600.4418; // km³/s²
constexpr double R_EARTH = 6378.137;     // km

constexpr std::size_t LINE_LEN = 69; // Every TLE line must be exactly 69 chars

enum : std::size_t
{
    COL_CATALOG_START = 2,
    COL_CATALOG_LEN = 5,
    COL_INCL_START = 8,
    COL_INCL_LEN = 8,
    COL_ECC_START = 26,
    COL_ECC_LEN = 7,
    COL_MEANMO_START = 52,
    COL_MEANMO_LEN = 11,
    COL_CHECKSUM_POS = 68
};

// --- Strip CR/LF only ---
std::string stripLineEndings(const std::string& line) {
    std::string result;
    for (char c : line)
        if (c != '\r' && c != '\n')
            result += c;
    return result;
}

// --- Checksum validation with debug ---
bool validateTLEChecksum(const std::string& raw) {
    std::string line = stripLineEndings(raw);
    std::cout << "Debug: validating line: [" << line << "]\n";
    std::cout << "Debug: line length = " << line.length() << "\n";

    if (line.length() != LINE_LEN) {
        std::cout << "Debug: Invalid line length!\n";
        return false;
    }

    int sum = 0;
    for (std::size_t i = 0; i < COL_CHECKSUM_POS; ++i) {
        char c = line[i];
        if (std::isdigit(c))
            sum += c - '0';
        else if (c == '-')
            sum += 1;
        // everything else adds 0
    }

    int expected = line[COL_CHECKSUM_POS] - '0';
    int computed = sum % 10;

    std::cout << "Debug: expected checksum = " << expected
              << ", computed checksum = " << computed << "\n";

    if (computed != expected) {
        std::cout << "Debug: checksum mismatch!\n";
        return false;
    }

    return true;
}

// --- Parse numeric fields ---
double parseInclination(const std::string& line2) {
    std::string str = line2.substr(COL_INCL_START, COL_INCL_LEN);
    std::cout << "Debug: Inclination string = [" << str << "]\n";
    return std::stod(str);
}

double parseEccentricity(const std::string& line2) {
    std::string eccStr = line2.substr(COL_ECC_START, COL_ECC_LEN);
    std::cout << "Debug: Eccentricity string = [" << eccStr << "]\n";

    uint64_t val = 0;
    for (char c : eccStr) {
        if (!std::isdigit(c)) {
            std::cout << "Debug: Eccentricity has non-digit character!\n";
            throw std::invalid_argument("Eccentricity contains non-digit characters");
        }
        val = val * 10 + (c - '0');
    }

    double e = static_cast<double>(val) / 1'000'000.0;
    std::cout << "Debug: Eccentricity value = " << e << "\n";
    return e;
}

double parseMeanMotion(const std::string& line2) {
    std::string str = line2.substr(COL_MEANMO_START, COL_MEANMO_LEN);
    std::cout << "Debug: Mean motion string = [" << str << "]\n";
    return std::stod(str);
}

// --- Validate TLE orbit ---
bool validateTLEOrbit(const std::string& line1, const std::string& line2) {

    std::cout << "Debug: Line1 = [" << line1 << "]\n";
    std::cout << "Debug: Line2 = [" << line2 << "]\n";

    if (!validateTLEChecksum(line1)) {
        std::cout << "Debug: Line1 checksum failed!\n";
        return false;
    }
    if (!validateTLEChecksum(line2)) {
        std::cout << "Debug: Line2 checksum failed!\n";
        return false;
    }

    try {
        double incl = parseInclination(line2);
        std::cout << "Debug: Inclination = " << incl << "\n";
        if (incl < 0.0 || incl > 180.0) {
            std::cout << "Debug: Inclination out of range!\n";
            return false;
        }

        double ecc = parseEccentricity(line2);
        if (ecc < 0.0 || ecc >= 1.0) {
            std::cout << "Debug: Eccentricity out of range!\n";
            return false;
        }

        double mm = parseMeanMotion(line2);
        if (mm <= 0.0) {
            std::cout << "Debug: Mean motion <= 0!\n";
            return false;
        }

    } catch (const std::exception& e) {
        std::cout << "Debug: Exception while parsing numeric fields: " << e.what() << "\n";
        return false;
    }

    return true;
}

// --- Main ---
int main() {
    // Line too short
std::string line1="1 53239U 22085A 26057.06690971 .00010434 00000+0 12875-3 0 9994";
// Inclination negative
std::string faulty_line2_mm_zero = "2 53239  41.4663 278.8535 0005884 197.5112 162.5524  0.00000000000000";
    // std::cout << "Line 2 Valid: " << validateTLEChecksum(line2) << "\n";

    bool ok =validateTLEOrbit(line1, faulty_line2_mm_zero);

 if(ok)
 {
    std::cout<<"TLE VALID"<<"\n";
 }
 else
 {
    std::cout<<"TLE Invalid"<<"\n";
 }

    return 0;
}
