void validateTLELength(const std::string& line)
{
    if (line.length() != LINE_LEN)
    {
        throw std::invalid_argument("TLE line length error: expected 69 characters, got " +
                                    std::to_string(line.length()));
    }
}

void validateTLEChecksum(const std::string& line)
{
    validateTLELength(line);  // make sure the line is the proper size first

    int expected = line[COL_CHECKSUM_POS] - '0';
    int sum = 0;

    for (int i = 0; i < COL_CHECKSUM_POS; ++i)
    {
        char ch = line[i];
        if (std::isdigit(ch))
        {
            sum += ch - '0';
        }
        else if (ch == '-')
        {
            ++sum;  // minus sign counts as 1 per the spec
        }
    }

    if ((sum % 10) != expected)
    {
        throw std::runtime_error("Checksum mismatch (expected " + std::to_string(expected) + ", computed " +
                                 std::to_string(sum % 10) + ")");
    }
}

void set(const char* const t1, const char* const t2)
{
    if (!t1 || !t2)
    {
        throw std::invalid_argument("TLE lines cannot be null");
    }

    //  Build string_views that know their length without scanning for '\\0'
    std::string sv1(t1, std::strlen(t1));
    std::string sv2(t2, std::strlen(t2));

    // Length Check
    validateTLELength(sv1);
    validateTLELength(sv2);

    // Checksum Check
    validateTLEChecksum(sv1);
    validateTLEChecksum(sv2);
}

int main()
{
    std::string l1 = "1 37869U 11064C   24303.75000000  .00000002  00000-0  49963+1 0 00005";
    std::string l2 = "2 37869 064.9632 093.1153 0023706 247.5214 095.2124 02.13101781101065";

    try
    {
        set(l1.c_str(), l2.c_str());
        std::cout << "TLE lines are valid." << std::endl;
    } catch (const std::exception& ex)
    {
        std::cerr << "Validation error: " << ex.what() << std::endl;
    }

    return 0;
}

