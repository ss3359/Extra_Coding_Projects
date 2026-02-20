#include <string>
#include <iostream>
#include <exception>
#include <regex>
using std::cout;
using std::endl;
using std::isspace, std::remove_if;
using std::string, std::invalid_argument;


int CalculateCheckSum(const string &Line){

    int sum = 0; 

    for (int i=0; i<68; i++){
        if(isdigit(Line[i])){
            sum+=Line[i] - '0';
        }
        else if(Line[i]=='-'){
            sum+=1;
        }
    }
    return sum % 10;
}


void ValidateTLE(string LineOne, string LineTwo)
{

    try
    {
        // std::regex LineOnePattern("^1 (\\d{5})([A-Z]) (\\d{2})(\\d{3})([A-Z]{0,3})   (\\d{2})(\\d{3}\\.\\d{8})  ([ +-]\\.\\d{8})  ([ 0-9]{5}[+-]\\d)  ([ 0-9]{5}[+-]\\d) (\\d)  (\\d{4})$");
        // std::regex LineTwoPattern("^2 (\\d{5})  (\\d{1,3}\\.\\d{4}) (\\d{1,3}\\.\\d{4}) (\\d{7}) (\\d{1,3}\\.\\d{4}) (\\d{1,3}\\.\\d{4}) (\\d{1,2}\\.\\d{8})(\\d)$");

        // if (!(std::regex_match(LineOne, LineOnePattern)))
        // {
        //     throw 404;
        // }
        // if (!(std::regex_match(LineTwo, LineTwoPattern)))
        // {
        //     throw 404;
        // }
       

        //Make Sure The Catalog Numbers In The Two Lines Are The Same 
        string cat1 = LineOne.substr(2,5), cat2=LineTwo.substr(2,5);
        if(cat1 != cat2){
            throw invalid_argument("The Catalog Numbers Are Not Equal"); 
        }

        // Make Sure The Angle is between 0 and 180 degrees
        double IncAngle = stod(LineTwo.substr(8, 7));
        if (IncAngle < 0.0 || IncAngle > 180.0)
        {
            throw invalid_argument("This is not a valid inclination angle! ");
        }

        // Make Sure the Eccentricity is between 0 and 1
        string ecc_str = LineTwo.substr(26, 7);
        double e = stod("0." + ecc_str);
        if (e >= 1)
        {
            throw invalid_argument("This is not a valid (ellipse) eccentricity!");
        }

        // Make Sure The Checksum is a number between 0 and 9 (mod 10)
        int expected = LineTwo[68] - '0';
        int calculated=CalculateCheckSum(LineTwo);

        if (calculated != expected)
        {
            throw invalid_argument("The checksum is invalid");
        }
    }
    catch (int e)
    {
        cout << "Error Code: " << e << endl;
    }
    catch (invalid_argument &e)
    {
        cout << "Error: " << e.what() << endl;
    }
}

int main()
{

    string TLE_Name = "ISS (ZARYA)";
    string TLE_Line_One = "1 25544U 98067A   08264.51782528 -.00002182  00000-0 -11606-4 0  2927";
    string TLE_Line_Two = "2 25544  51.6416 247.4627 0006703 130.5360 325.0288 15.72125391563537";

    ValidateTLE(TLE_Line_One, TLE_Line_Two);

    // Line 1: invalid catalog number, bad spacing
string BadTLE_Line_One = "1 2554XU 98067A   08264.51782528 -.00002182  00000-0 -11606-4 0  292";  

// Line 2: eccentricity >=1, inclination > 180°, checksum wrong
string BadTLE_Line_Two = "2 25544  190.6416 247.4627 1234567 130.5360 325.0288 15.72125391563538";

    ValidateTLE(BadTLE_Line_One,BadTLE_Line_Two);
    return 0;
}
