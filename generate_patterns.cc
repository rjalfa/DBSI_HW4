#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

vector<unsigned int> accesses;

//Function to access block
void access_block(unsigned int block_addr)
{
	//Log block access in global array
	accesses.push_back(block_addr);
}

//Generic Table
class Table
{
	unsigned int starting_block;
	unsigned int numblocks;
	unsigned int records_per_block;
	unsigned int numrecords;
	public:
		Table(unsigned int starting_block_p, unsigned int numblocks_p, unsigned int number_of_records)
			: starting_block(starting_block_p), numblocks(numblocks_p), records_per_block((unsigned int)(ceil(number_of_records*1.0 / numblocks_p))), numrecords(number_of_records) {};
		unsigned int number_of_records() { return records_per_block*numblocks; }
		unsigned int get_starting_block() { return starting_block; }
		unsigned int get_numblocks() { return numblocks; }
		unsigned int get_records_per_block() { return records_per_block; }
};

void nested_loop_join(Table& inner_table, Table& outer_table)
{
	//Simulating Nested Loop Join
	unsigned int o_block = outer_table.get_starting_block();
	//Outer loop
	while(o_block < outer_table.get_starting_block() + outer_table.get_numblocks())
	{
		//Access block
		access_block(o_block);
		unsigned int i_block = inner_table.get_starting_block();
		while(i_block < inner_table.get_starting_block() + inner_table.get_numblocks())
		{
			//Access block
			access_block(i_block);

			for(unsigned int i = 0; i < outer_table.get_records_per_block(); i ++)
			{
				for(unsigned int j = 0; j < inner_table.get_records_per_block(); j ++)
				{
					//Join Tuples
					//Assume done
				}
			}
			i_block ++;
		}
		o_block ++;
	}
}

int main(int argc, char* argv[])
{
	//Table Objects
	Table employee(1,1000,10000);
	Table department(1001,100,1000);
	Table project(1101,500,5000);
	if(argc < 3) {
		cerr << "Usage: " << argv[0] << " <<inner_table>> <<outer_table>>" << endl;
		cerr << "tables are specified as employee -> 0 , department -> 1, project -> 2" << endl;
		exit(1);
	}
	int it = argv[1][0] - '0';
	int ot = argv[2][0] - '0';
	if(it == ot)
	{
		cerr << "[ERROR] Trying to join a table with itself" << endl;
		exit(2);
	}

	Table* inner_table = nullptr;
	Table* outer_table = nullptr;
	switch(it)
	{
		case 0:inner_table = &employee;break;
		case 1:inner_table = &department;break;
		case 2:inner_table = &project;break;
		default:
			cerr << "[ERROR] Invalid code for Inner Table" << endl;
			exit(3);
	}
	switch(ot)
	{
		case 0:outer_table = &employee;break;
		case 1:outer_table = &department;break;
		case 2:outer_table = &project;break;
		default:
			cerr << "[ERROR] Invalid code for Outer Table" << endl;
			exit(3);
	}
	
	//Clear Accesses
	accesses.clear();
	
	//run nested-loop
	nested_loop_join(*inner_table, *outer_table);

	//print accesses
	cout << accesses.size() << " " << 1500 << endl;
	for(unsigned int i = 0; i < accesses.size(); i ++) cout << accesses[i] << " ";
	cout << endl;
	return 0;
}