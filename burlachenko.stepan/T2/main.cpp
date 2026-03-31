#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <iomanip>
#include <cctype>

// var 14

struct DataStruct
{
    double key1;
    unsigned long long key2;
    std::string key3;
};

std::string extractValue(const std::string& line, const std::string& key)
{
    std::string search = ":" + key + " ";
    size_t pos = line.find(search);

    if (pos == std::string::npos)
    {
        return "";
    }

    pos += search.length();
    size_t end = line.find(':', pos);

    if (end == std::string::npos)
    {
        return "";
    }
    return line.substr(pos, end - pos);
}

std::istream& operator>>(std::istream& is, DataStruct& ds) 
{
    std::string line;
    if(!std::getline(is, line)) 
    {
        return is;
    }

    std::string v1 = extractValue(line, "key1");
    std::string v2 = extractValue(line, "key2");
    std::string v3 = extractValue(line, "key3");

    if(v1.empty() || v2.empty() || v3.empty()) 
    {
        is.setstate(std::ios::failbit);
        return is;
    }

    if(!v1.empty() && (v1.back() == 'd' || v1.back() == 'D'))
    {
        v1.pop_back();
    }

    std::istringstream ss1(v1);
    double d;
    ss1 >> d;
    if(ss1.fail()) 
    {
        is.setstate(std::ios::failbit);
        return is;
    }

    ds.key1 = d;

    size_t u_pos = v2.find('u');
    if (u_pos == std::string::npos) 
    {
        is.setstate(std::ios::failbit);
        return is;
    }

    std::string num_part = v2.substr(0, u_pos);
    std::istringstream ss2(num_part);

    unsigned long long ull;
    ss2 >> ull;
    if(ss2.fail()) 
    {
        is.setstate(std::ios::failbit);
        return is;
    }
    ds.key2 = ull;

    
    if(v3.size() < 2 || v3.front() != '"' || v3.back() != '"') 
    {
        is.setstate(std::ios::failbit);
        return is;
    }
    ds.key3 = v3.substr(1, v3.size() - 2);  

    return is;
}

std::ostream& operator<<(std::ostream& os, const DataStruct& ds) {
    os << "(:key1 " << std::fixed << std::setprecision(1) << ds.key1 << "d:";
    os << "key2 " << ds.key2 << "u11:";
    os << "key3 \"" << ds.key3 << "\":)";
    return os;
}




int main()
{


    return 0;
}
