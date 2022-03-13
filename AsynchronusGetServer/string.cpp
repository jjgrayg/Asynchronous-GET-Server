//Function definitions for the class String
//Jarod Graygo

#include <iostream>
#include "string.hpp" 

//////////////////////////////////
// CONSTRUCTORS
//////////////////////////////////

//Default constructor for String
//Initializes to an empty string
String::String()
{
	stringSize = 1;
	str = new char[stringSize];
	str[0] = 0;
}

//Initializes to an empty string with capacity of x
String::String(int x)
{
	stringSize = x+1;
	str = new char[stringSize];
	for (int i = 0; i < stringSize; i++) str[i] = '\0';
}

//Char constructor for String
String::String(char x) : String(1)
{
	if (x == 0)
	{
		resetCapacity(0);
	}
	else
	{
		str[0] = x;
	}
}

//Char array constructor for String
String::String(const char x[])
{
	int counter = 0;
	while (x[counter] != 0) counter++;
	stringSize = counter + 1;
	str = new char[stringSize];
	str[stringSize-1] = 0;

	for (int i = 0; i < counter; i++)
	{
		if (x[i] == 0) break;
		str[i] = x[i];
	}
}

//Initializes to a string containing char array x and
//a capacity of N
String::String(int N, const char x[]) : String(N)
{
	int counter = 0;
	while (x[counter] != 0) counter++;
	for (int i = 0; i < (stringSize - 1); i++)
	{
		str[i] = x[i];
	}
}

//Copy constructor for string
String::String(const String& x)
{
	stringSize = x.stringSize;
	str = new char[stringSize];
	for (int i = 0; i < stringSize; i++)
	{
		str[i] = x.str[i];
	}
}

//Destructor for string
String::~String()
{
	delete[] str;
}

//////////////////////////////////
// OPERATOR OVERLOADS
//////////////////////////////////

//Input operator overload
std::istream& operator>>(std::istream& in, String& rhs)
{

	char temp[500];

	if (!in.eof())
	{
		in >> temp;
		rhs = String(temp);
		return in;
	}
	else
	{
		return in;
	}
}

//Output operator overload
std::ostream& operator<<(std::ostream& out, const String& rhs)
{
	for (int i = 0; i < rhs.stringSize; i++)
	{
		if (rhs.str[i] == 0) break;
		out << rhs.str[i];
	}

	return out;
}

//Assignment operator for String
String& String::operator=(const String rhs)
{
	if (str != rhs.str)
	{
		delete[] str;
		stringSize = rhs.stringSize;
		str = new char[stringSize];
		for (int i = 0; i < stringSize; i++)
		{
			str[i] = rhs.str[i];
		}
	}
	return *this;
}

//Accessor/Modifier subscript overload
char& String::operator[](int i) { return str[i]; }

//Simple accessor subscript overload
char String::operator[](int i) const { return str[i]; }

//Concatenation operators
String String::operator+(const String& rhs) const
{
	int offset = 0;
	String result((stringSize + rhs.stringSize - 2));
	for (int i = 0; i < stringSize; i++)
	{
		result.str[i] = str[i];
		offset = i;
	}
	for (int i = 0; i < rhs.stringSize; i++) { result.str[offset + i] = rhs.str[i]; }

	return result;
}

String operator+(const char lhs[], const String& rhs) { return String(lhs) + rhs; }
String operator+(char lhs, const String& rhs) { return String(lhs) + rhs; }

//Concatenation and assignment operator for String
String& String::operator+=(const String& rhs) 
{ 
	*this = *this + rhs;
	return *this;
}

//Equals operator overloads
bool String::operator==(const String& rhs) const
{
	bool equal = true;
	int index = 0;

	while (equal && (index < stringSize && index < rhs.stringSize)) {
		if (rhs.str[index] == str[index])
		{
			equal = true;
		}
		else if (rhs.str[index] != str[index])
		{
			equal = false;
		}
		index++;
	}
	return equal;
}

bool operator==(const char lhs[], const String& rhs) { return rhs == lhs; }
bool operator==(char lhs, const String& rhs) { return rhs == lhs; }

//Less than operator overloads
bool String::operator<(const String& rhs) const
{
	int index = 0;

	do {
		if (str[index] > rhs[index])
		{
			break;
		}
		else if (str[index] < rhs[index])
		{
			return true;
		}
		else if (str[index] == rhs[index])
		{
			index++;
		}
	} while (index < stringSize);

	return false;
}

bool operator<(const char lhs[], const String& rhs) { return rhs > lhs; }
bool operator<(char lhs, const String& rhs) { return rhs > lhs; }

//Greater than operator overload
bool operator>(const String& lhs, const String& rhs) { return rhs < lhs; }

//Less than or equal to operator overload
bool operator<=(const String& lhs, const String& rhs)
{
	if (lhs == rhs) return true;
	else if (lhs < rhs) return true;
	else return false;
}

//Greater than or equal to operator overload
bool operator>=(const String& lhs, const String& rhs)
{
	if (lhs == rhs) return true;
	else if (lhs > rhs) return true;
	else return false;
}

//Not equal operator overload
bool operator!=(const String& lhs, const String& rhs) { return !(lhs == rhs); }

//////////////////////////////////
// MEMBER FUNCTIONS
//////////////////////////////////

//Constant time swap for String
void String::swap(String& rhs)
{
	int tStrSize = rhs.stringSize;
	rhs.stringSize = stringSize;
	stringSize = tStrSize;
	char* temp = rhs.str;
	rhs.str = str;
	str = temp;
}

//Returns the capacity of String
int String::capacity() const { return (stringSize - 1); }

//Resets the capacity of a String while keeping it in tact
void String::resetCapacity(int x)
{
	*this = String(x, str);
}

//Returns the number of characters in the string
int String::length() const
{
	int counter = 0;
	while (str[counter] != 0) counter++;
	return counter;
}

//Returns the substring from the first arg to the second arg inclusive
String String::substr(int start, int end) const
{
	int index = 0;
	if (start > end) return String('\0');
	if (end >= length()) end = (length() - 1);
	if (start >= length()) return String('\0'); 
	String result((end - start + 1));
	for (int i = start; i <= end; i++)
	{
		result.str[index] = str[i];
		index++;
	}
	return result;
}

//Finds the position of a char at or after the position in the argument. Returns -1 if the char is not present in the string
int String::findch(int pos, char ch) const
{
	if (pos < 0) pos = 0;
	for (int i = pos; i < stringSize; i++)
	{
		if (str[i] == ch) return i;
	}

	return -1;

}

//Finds the position of a string at or after the position in the argument and returns the position where it starts
//Returns -1 if the string is not present
int String::findstr(int pos, const String& string) const
{

	bool isPresent = false;

	for (int i = pos; i < stringSize; i++)
	{
		if (str[i] == string[0])
		{
			for (int j = 0; j < string.length(); j++)
			{
				if ((i + j) >= stringSize) break;
				if (str[i + j] != string[j]) isPresent = false;
				else isPresent = true;
				if (isPresent && j == (string.length() - 1)) return i;
			}
		}
	}

	return -1;

}

//Splits the String into multiple Strings based on the input character and
//places them into a vector
std::vector<String> String::split(char det) const
{
	String temp;
	bool working = true;
	std::vector<String> result;
	int begin = -1;
	int end = findch(begin + 1, det);
	while (working)
	{
		if (end == (begin + 1)) end = end + 1;
		temp = substr(begin + 1, end - 1);
		if (begin == -1 && end == -1) temp = str;
		if (temp == det) temp = '\0';
		result.push_back(temp);
		begin = findch(end - 1, det);
		if (begin == -1) break;
		end = findch(begin + 1, det);
		if (end == -1)
		{
			end = capacity();
		}
	}

	return result;
}

//Splits the String into multiple Strings based on the input String and
//places them into a vector
std::vector<String> String::split(String det) const
{
	String temp;
	bool working = true;
	std::vector<String> result;
	int begin = -1;
	int end = findstr(begin + 1, det);
	while (working)
	{
		if (end == (begin + 1)) end = end + 1;
		temp = substr(begin + 1, end - 1);
		if (begin == -1 && end == -1) temp = str;
		if (temp == det) temp = '\0';
		result.push_back(temp);
		begin = findstr(end - 1, det);
		if (begin == -1) break;
		end = findstr(begin + 1, det);
		if (end == -1)
		{
			end = capacity();
		}
	}

	return result;
}

char* String::getArr() const{
	return str;
}