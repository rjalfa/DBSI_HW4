#include <iostream>
#include <map>
#include <vector>
using namespace std;
enum POLICY {LRU, MRU};

POLICY replacement_policy;
unsigned int num_page_faults = 0;
unsigned int moment = 0;
unsigned int PAGE_TABLE_SIZE = 0;
//Memlocation class
class memlocation 
{
	public:
		unsigned int pageid;
		unsigned int last_accessed;
		memlocation() : pageid(-1), last_accessed(0) {}
		memlocation(const memlocation& m) : pageid(m.pageid), last_accessed(m.last_accessed) {}
		memlocation(unsigned int pageid_p, unsigned int last_accessed_p) : pageid(pageid_p), last_accessed(last_accessed_p) {}
		bool operator<(const memlocation& a)
		{	
			return last_accessed < a.last_accessed;
		}
};

//All the pages in disk
vector<memlocation> disk;

//Initialize Disk Pages
void initialize_disk(int numpages)
{
	for(int i = 0; i < numpages; i ++) disk.push_back(memlocation(i + 1, 0));
}

//Gets a page from the disk
memlocation access_page_from_disk(int addr)
{
	//1-based indexing
	addr --;
	if(addr < 0 || addr >= static_cast<int>(disk.size())) throw 0;
	num_page_faults ++;
	return disk[addr];  
}

//The Page table
map<unsigned int,memlocation> pagetable;

//Gets page from page-table
memlocation get_page(unsigned int addr)
{
	if(pagetable.find(addr) != pagetable.end()) {
		//Set last accessed to current time
		pagetable[addr].last_accessed = moment;
		return pagetable[addr];
	}
	//Page not found, need to add

	//If pagetable full, need to evict
	if(pagetable.size() >= PAGE_TABLE_SIZE) // Being paranoid
	{
		decltype(pagetable)::iterator it = pagetable.begin();
		unsigned int temp = (it->second).last_accessed;
		
		//Loop over the pagetable
		for(auto itr = pagetable.begin(); itr != pagetable.end(); itr ++ )
		{
			if(replacement_policy == LRU)
			{
				if((itr->second).last_accessed < temp) {
					temp = (itr->second).last_accessed;
					it = itr;
				}
			}
			else if(replacement_policy == MRU)
			{
				if((itr->second).last_accessed > temp) {
					temp = (itr->second).last_accessed;
					it = itr;
				}
			}
		}
		// cout << "[INFO] Removing " << it->first
		//Remove page from table
		pagetable.erase(it);
	}

	//Get requested page from disk
	pagetable[addr] = access_page_from_disk(addr);
	pagetable[addr].last_accessed = moment;
	return pagetable[addr];
}

int main(int argc, char* argv[])
{
	if(argc < 3) {
		cerr << "Usage: " << argv[0] << " <<POLICY>> <<TABLE_SIZE>>" << endl;
		cerr << "POLICY=> LRU->0, MRU->1" << endl;
		exit(1);
	}
	if(!(argv[1][0] - '0' == 0 || argv[1][0] - '0' == 1))
	{
		cerr << "[ERROR] Invalid Policy detected" << endl;
		exit(2);
	}
	PAGE_TABLE_SIZE = atoi(argv[2]);
	replacement_policy = (POLICY)(argv[1][0] - '0');

	unsigned int N, W, x;
	cin >> N >> W;

	//Initialize disk to W pages
	initialize_disk(W);

	//Loop over accesses
	for(unsigned int i = 0; i < N; i ++)
	{
		cin >> x;
		moment ++ ;
		get_page(x);
	}

	//Print Page faults
	cout << num_page_faults << endl;
}