#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
using namespace std; 

int n_p_mos_counter = 0;


void generate(string valid_expression)
{
	ofstream NetList; 
	NetList.open("netlist.txt", ios::out);
	int location = 0; 
	location = valid_expression.find('=');
	
	NetList << valid_expression << '\n';

	// should detect the precednce 
	for(int i = location + 1; i<valid_expression.length(); i++)
	{
		if(valid_expression[i+1] == '`')
		{
			if(isalpha(valid_expression[i]))
			{
				//nmos first
				//id 
				NetList << "M" << n_p_mos_counter<< " "; 
				//drain
				NetList << valid_expression[0] << " "; 
				//gate
				NetList << valid_expression[i]<< " "; 
				//source and body
				NetList << "0" << " 0";
				//type 
				NetList << " NMOS" << '\n';

				n_p_mos_counter++;
			}
		}
	}
	
	NetList.close();	
	return;
}

bool valid(string expression)
{
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


void handler()
{
	string boolean_expression;
	cout<<"\nPlease, enter your boolean expression: ";
	getline(cin, boolean_expression);
	
	if( valid(boolean_expression) )
	{
		cout << boolean_expression<<endl; 
		generate(boolean_expression);
		return; 
	}
	
	else 
	{
		cout<<"\ninvalid input."<<endl;
		handler();
	}
}



int main()
{
	
	cout<<"\nHello, this program generates NetSpice code for your boolean function."<<endl;
	
 	handler();
	
	
	return 0;
}

