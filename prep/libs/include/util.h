
#include <iostream>
#include <filesystem>
#include <sstream>
#include <string>
#include <vector>
#include <regex>

#define String std::string
#define Vector std::vector
void replace(String& i, String p, String r){std::regex c(p);i = std::regex_replace(i,c,r);}
String int2charptr(float num){std::stringstream strs;strs << num;std::string temp_str = strs.str();return temp_str;}
template<typename T>
struct vec2{T x,y; vec2(T xa, T ya) { x = xa; y = ya;}vec2(){}};
#define vec2i vec2<int>
#define vec2f vec2<float>
#define vec2d vec2<double>
