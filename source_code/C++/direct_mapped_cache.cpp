#include <iostream>
#include <stdio.h>
#include <math.h>

using namespace std;

struct cache_content
{
	bool v;		//verify bit
	unsigned int tag;
    // unsigned int	data[16];
};

const int K = 1024;

double log2(double n)
{  
    // log(n) / log(2) is log2.	log with base 2
    return log(n) / log(double(2));
}


void simulate(int cache_size, int block_size)
{
	unsigned int tag, index, x;

	int offset_bit = (int)log2(block_size);		//2^(offset_bit) bytes in one block
	int index_bit = (int)log2(cache_size / block_size);
	int line = cache_size >> (offset_bit);	// cache/2^(offset_bit) is "number of block"

	cache_content *cache = new cache_content[line];
	
    cout << "cache line: " << line << endl;

	for(int j = 0; j < line; j++)	//instart, every line is empty
		cache[j].v = false;	
	
    FILE *fp = fopen("test.txt", "r");  // read file
	
	while(fscanf(fp, "%x", &x) != EOF)
    {
		cout << hex << x << " ";	//hexadecimal base
		index = (x >> offset_bit) & (line - 1);	//filter the index bits
		tag = x >> (index_bit + offset_bit);	//filter the tag bits
		if(cache[index].v && cache[index].tag == tag)
			cache[index].v = true;    // hit
		else
        {
			cache[index].v = true;  // miss, update block
			cache[index].tag = tag;
		}
	}
	fclose(fp);

	delete [] cache;
}
	
int main()
{
	// Let us simulate 4KB cache with 16B blocks
	simulate(4 * K, 16);
}
