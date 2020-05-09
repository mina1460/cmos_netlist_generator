#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
using namespace std;

//******************************************************************************************//

int n_p_mos_counter = 1;
int wires_counter = 1;
string temp_wire; 

//*******************************************************************************************//

void handler();
bool valid(string expression);
void generate (string valid_expression);
string NOT(string valid_expression, int location, string input_wire, string& output_wire);

int main()
{
    
    cout<<"\nHello, this program generates NetSpice code for your boolean function."<<endl;

    handler();
   
    return 0;
}

void handler(){
    string boolean_expression;
    cout<<"\nPlease, enter your boolean expression: ";
    getline(cin, boolean_expression);
   
    if( valid(boolean_expression) )
    {
        cout <<"You entred: " << boolean_expression << endl;
        generate(boolean_expression);
        return;
    }
   
    else
    {
        cout<<"\ninvalid input."<<endl;
        handler();
    }
}

bool valid(string expression){
    
    char out = expression[0];
    int location = 0;
    location = expression.find('=');
 
    for(int i = location+1; i<expression.length(); i++)
    {
        if(expression[i] == out)
        {
            return false;
        }
       
        if(expression[i] != '|' && expression[i] != '&' && expression[i] != '`' && expression[i]!=' ')
        {
            if(!isalpha(expression[i]))
            {      
                return false;
            }
        }
    }
   
    return true;
}

string NOT(string valid_expression, int location, string input_wire, string & output_wire)
{
    string not_implementation; 

    not_implementation = "M"; // + n_p_mos_counter + " ";
    not_implementation += to_string(n_p_mos_counter); 
    not_implementation += " ";
    //drain
    not_implementation += "W";
    not_implementation += to_string (wires_counter);
    not_implementation += " ";
    //gate
    not_implementation += input_wire;
    not_implementation += " ";
    //source and body
    not_implementation += "vdd vdd";
    //type
    not_implementation +=" PMOS \n";
 
    n_p_mos_counter++;
 
    not_implementation += "M"; 
    not_implementation += to_string(n_p_mos_counter);
    not_implementation += " ";
    //drain
    not_implementation += "W";
    not_implementation += to_string (wires_counter);
    output_wire = "W";
    output_wire += to_string(wires_counter);

    wires_counter++;

   
    
    not_implementation += " ";
    //gate
    not_implementation += input_wire;
    not_implementation += " ";
    //source and body
    not_implementation += " 0   0 ";
    //type
    not_implementation += " NMOS \n";
 
    n_p_mos_counter++;

    return not_implementation; 
}

void generate(string valid_expression)
{
    ofstream NetList;
    NetList.open("netlist.txt", ios::out);
    int location = 0;
    location = valid_expression.find('=');
   
    NetList << valid_expression << '\n';
    string output_wire_name;

    //then we want to generate the not gate first. therefore, I loop over the string, if I found a not operation, we perform it 
    int expression_length = valid_expression.length();

    for (int i = location + 1; i < expression_length; i++)
    {
         if(valid_expression[i+1] == '`')       // if the next character is a not operation, we extract the current one we are wokring with
        {
            if(isalpha(valid_expression[i]))
            {
                //call NOT function
                string a;
                a = valid_expression[i];
                
                output_wire_name.clear();

                NetList << NOT(valid_expression, location, a, output_wire_name);

            }
        }
    }


    NetList << "where " << valid_expression[0] << " = " << output_wire_name << endl; 
}
