#include <gtest/gtest.h>
#include <string>
#include <stdexcept>
#include <cctype>
#include <cstring>

// ------------------------------------------------------------
// Constants
// ------------------------------------------------------------

constexpr int LINE_LEN = 69;
constexpr int COL_CHECKSUM_POS = 68;

// ------------------------------------------------------------
// Checksum Utilities
// ------------------------------------------------------------

void fixCheckSum(std::string& line)
{
    int sum = 0;

    for (int i = 0; i < COL_CHECKSUM_POS; ++i)
    {
        char ch = line[i];

        if (std::isdigit(ch))
            sum += ch - '0';
        else if (ch == '-')
            ++sum;
    }

    line[COL_CHECKSUM_POS] = static_cast<char>((sum % 10) + '0');
}

std::string corruptField(const std::string& line,
                         int start,
                         int length,
                         const std::string& replacement)
{
    std::string modified = line;
    modified.replace(start, length, replacement);
    fixCheckSum(modified);
    return modified;
}

// ------------------------------------------------------------
// Fault Injection Helpers
// ------------------------------------------------------------

std::string makeBadChecksum(const std::string& line)
{
    std::string modified = line;
    modified[COL_CHECKSUM_POS] =
        (line[COL_CHECKSUM_POS] == '0') ? '1' : '0';
    return modified;
}

std::string makeBadLength(const std::string& line)
{
    return line.substr(0, line.length() - 1); // remove 1 char
}

std::string makeBadInclination(const std::string& line2)
{
    return corruptField(line2, 8, 8, "181.0000");
}

std::string makeBadEccentricity(const std::string& line2)
{
    return corruptField(line2, 26, 7, "9999999");
}

std::string makeBadMeanMotion(const std::string& line2)
{
    return corruptField(line2, 52, 11, "00000000000");
}

// ------------------------------------------------------------
// Validation Functions
// ------------------------------------------------------------

void validateTLELength(const std::string& line)
{
    if (line.length() != LINE_LEN)
        throw std::invalid_argument("Invalid TLE length");
}

void validateTLECatalogNumbers(const std::string& line1,
                               const std::string& line2)
{
    int cat1 = std::stoi(line1.substr(2, 5));
    int cat2 = std::stoi(line2.substr(2, 5));

    if (cat1 != cat2)
        throw std::invalid_argument("Catalog numbers mismatch");
}

void validateTLEChecksum(const std::string& line)
{
    validateTLELength(line);

    int expected = line[COL_CHECKSUM_POS] - '0';
    int sum = 0;

    for (int i = 0; i < COL_CHECKSUM_POS; ++i)
    {
        char ch = line[i];

        if (std::isdigit(ch))
            sum += ch - '0';
        else if (ch == '-')
            ++sum;
    }

    if ((sum % 10) != expected)
        throw std::runtime_error("Checksum mismatch");
}

void validateTLEInclination(const std::string& line2)
{
    double inc = std::stod(line2.substr(8, 8));
    if (inc < 0.0 || inc > 180.0)
        throw std::out_of_range("Invalid inclination");
}

void validateTLEEccentricity(const std::string& line2)
{
    double ecc = std::stod("0." + line2.substr(26, 7));
    if (ecc < 0.0 || ecc >= 1.0)
        throw std::out_of_range("Invalid eccentricity");
}

void validateTLEMeanMotion(const std::string& line2)
{
    double mm = std::stod(line2.substr(52, 11));
    if (mm <= 0.0)
        throw std::out_of_range("Invalid mean motion");
}

void validateTLEOrbit(const std::string& l1,
                      const std::string& l2)
{
    validateTLELength(l1);
    validateTLELength(l2);

    validateTLECatalogNumbers(l1, l2);

    validateTLEChecksum(l1);
    validateTLEChecksum(l2);

    validateTLEInclination(l2);
    validateTLEEccentricity(l2);
    validateTLEMeanMotion(l2);
}

void set(const char* t1, const char* t2)
{
    if (!t1 || !t2)
        throw std::invalid_argument("Null TLE input");

    std::string l1(t1);
    std::string l2(t2);

    validateTLEOrbit(l1, l2);
}

// ------------------------------------------------------------
// Base Valid TLE
// ------------------------------------------------------------

std::string valid_l1 =
    "1 37869U 11064C   24303.75000000  .00000002  "
    "00000-0  49963+1 0 00005";

std::string valid_l2 =
    "2 37869 064.9632 093.1153 0023706 247.5214 "
    "095.2124 02.13101781101065";

// ------------------------------------------------------------
// Google Tests
// ------------------------------------------------------------

TEST(TLEValidation, ValidTLE)
{
    EXPECT_NO_THROW(set(valid_l1.c_str(), valid_l2.c_str()));
}

TEST(TLEValidation, BadChecksum)
{
    auto bad = makeBadChecksum(valid_l1);
    EXPECT_THROW(set(bad.c_str(), valid_l2.c_str()), std::runtime_error);
}

TEST(TLEValidation, BadLength)
{
    auto bad = makeBadLength(valid_l1);
    EXPECT_THROW(set(bad.c_str(), valid_l2.c_str()), std::invalid_argument);
}

TEST(TLEValidation, BadInclination)
{
    auto bad = makeBadInclination(valid_l2);
    EXPECT_THROW(set(valid_l1.c_str(), bad.c_str()), std::out_of_range);
}

TEST(TLEValidation, BadEccentricity)
{
    auto bad = makeBadEccentricity(valid_l2);
    EXPECT_THROW(set(valid_l1.c_str(), bad.c_str()), std::out_of_range);
}

TEST(TLEValidation, BadMeanMotion)
{
    auto bad = makeBadMeanMotion(valid_l2);
    EXPECT_THROW(set(valid_l1.c_str(), bad.c_str()), std::out_of_range);
}

TEST(TLEValidation, NominalInclinationZero)
{
    auto modified = corruptField(valid_l2, 8, 8, "000.0000");
    EXPECT_NO_THROW(set(valid_l1.c_str(), modified.c_str()));
}

TEST(TLEValidation, NominalInclination180)
{
    auto modified = corruptField(valid_l2, 8, 8, "180.0000");
    EXPECT_NO_THROW(set(valid_l1.c_str(), modified.c_str()));
}

TEST(TLEValidation, NominalMeanMotionLEO)
{
    auto modified = corruptField(valid_l2, 52, 11, "15.00000000");
    EXPECT_NO_THROW(set(valid_l1.c_str(), modified.c_str()));
}
