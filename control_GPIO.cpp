// This is a non-object oriented C++ program to write the first input's corresponding GPIO pin number to high or low while simultaneously reading the current digital logic value on the
// third input's corresponding GPIO pin number.
// Example invocation of program: ./control_GPIO 60 hi 46
//
// ****** ENABLE PULL UP/DOWN RESISTOR ON INPUT SWITCH BEFORE USING******
// Example: 	config-pin -a p8.16 in+  

#include<string.h>
#include<fstream>
#include<iostream>
#include<sstream>
using std::string;
using std::ostringstream;
using std::stringstream;
using std::ofstream;
using std::ifstream;
using std::cout;
using std::endl;

#define GPIO_PATH "/sys/class/gpio/"

int write(string path, string filename, string value){
   ofstream fs;
   fs.open((path + filename).c_str());
   if (!fs.is_open()){
           perror("GPIO: write failed to open file ");
           return -1;
   }
   fs << value;
   fs.close();
   return 0;
}

string read(string path, string filename = "/value"){
   ifstream fs;
   fs.open((path + filename).c_str());
   if (!fs.is_open()){
           perror("GPIO: read failed to open file ");
    }
   string input;
   getline(fs,input);
   fs.close();
   return input;
}

void setDirection(string filename, string direction){
   write(GPIO_PATH, (filename + "/direction"), direction);	
}

int main(int argc, char* argv[]){
	if(argc != 4){
                cout << "Invalid arguments: ./control_GPIO <write_gpio_number> <status> <read_gpio_number>" << endl;
                cout << "Example usage is: " << endl << "./control_GPIO 60 hi 46" << endl;
		return -1;
        }

	stringstream ws(argv[1]);	
	int write_pin;
	ws >> write_pin;

	const char *hi_or_low = argv[2];

	stringstream rs(argv[3]);
	int read_pin;
	rs >> read_pin;

	ostringstream s;
	s << "gpio" << write_pin;
	string gpio_wfile = string(s.str());
	
	setDirection(gpio_wfile, "out");	
	
	if(!strcmp(hi_or_low, "hi")){ 
		write(GPIO_PATH, (gpio_wfile + "/value"), "1"); 
		cout << "Writing GPIO " << write_pin << " " << hi_or_low << endl;
	}
	else if(!strcmp(hi_or_low, "low")){ 
		write(GPIO_PATH, (gpio_wfile + "/value"), "0");
		cout << "Writing GPIO " << write_pin << " " << hi_or_low << endl;
	}
	
	ostringstream r;
	r << "gpio" << read_pin;
	string gpio_rfile = string(r.str());
	setDirection(gpio_rfile, "in");
	string out = read(GPIO_PATH, (gpio_rfile + "/value"));	
	cout << "GPIO " << read_pin << " has a value of : " << out << endl; 

	return 0;
}
