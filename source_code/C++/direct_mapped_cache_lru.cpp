#include <iostream>
#include <stdio.h>
#include <math.h>
#include <climits>
#include <string>

using namespace std;

struct cache_content
{
	bool v;				//verify bit
	unsigned int tag;
	unsigned int time;	//time stamp for last use
};

const int K = 1024;

double log2(double n)
{
	// log(n) / log(2) is log2.	log with base 2
	return log(n) / log(double(2));
}


double simulate(int cache_size, int block_size, int set_size,const char *filename)
{
	unsigned int tag, index, set, x;
	unsigned int count = 0, miss = 0;


	int offset_bit = (int)log2(block_size);		//2^(offset_bit) bytes in one block
	int index_bit = (int)log2((cache_size / block_size) / set_size);
	int line = cache_size >> (offset_bit);		// cache/2^(offset_bit) is "number of block"
	int set_n = line / set_size;				//how many sets in cache

	cache_content **cache = new cache_content*[set_n];
	for (int i = 0; i < set_n; i++){
		cache[i] = new cache_content[set_size];
	}


	for (int i = 0; i < set_n; i++)				//instart, every line is empty
	for (int j = 0; j < set_size; j++)
		cache[i][j].v = false;

	FILE *fp = fopen(filename, "r");			// read file

	while (fscanf(fp, "%x", &x) != EOF)
	{
		count++;								//add time stamp

		index = (x >> offset_bit) & (set_n - 1);		//filter the index bits
		tag = x >> (index_bit + offset_bit);			//filter the tag bits
		set = index;

		bool hit_flag = false, empty = false;
		for (int i = 0; i < set_size; i++){
			if (cache[set][i].v && cache[set][i].tag == tag){
				hit_flag = true;				//hit
				cache[set][i].time = count;
			}
		}

		if (hit_flag == false){					//miss
			miss++;
			for (int i = 0; i < set_size; i++){
				if (cache[set][i].v == false){
					empty = true;				//some idle space for new data
					cache[set][i].v = true;
					cache[set][i].tag = tag;
					cache[set][i].time = count;
				}
			}

			if (empty == false){				//there are no idle space
				int earliest = INT_MAX, LRU = -1;
				for (int i = 0; i<set_size; i++){				//find LRU block
					if (cache[set][i].time <= earliest){
						earliest = cache[set][i].time;
						LRU = i;
					}
				}

				cache[set][LRU].tag = tag;
				cache[set][LRU].time = count;
			}
		}

	}
	fclose(fp);

        printf("size = %5d bits, ",(32 - index_bit - offset_bit+1)*set_size*set_n);
	for (int i = 0; i < set_n; i++)
		delete[] cache[i];

	delete[] cache;
	return (double)miss / (double)count;
}
void printfile(const char *filename)
{
    FILE *fp2 = fopen("output.txt", "a+");  //write?
    printf("File %s processing...\n",filename);
    fprintf(fp2,"File %s processing...\n",filename);
    for (unsigned int i = 1; i <= 32; i = i << 1){
		//cout << i << "-way:" << endl;
        printf("cache size: %2dK\n",i);
        for (unsigned int n = 1; n <= 8; n = n << 1){
            printf("%2d-way, ",n);
			//cout << "cache size: " << i << " ";
			float result = simulate(i * K, 64, n,filename) * 100;
			printf("miss rate: %2.6f%%\n", result);
			fprintf(fp2,"%d %f\n",n, result);
			//cout << "miss rate" << simulate(i * K, 64, n) << endl;
		}
		cout << endl;
	}
}

int main()
{
    printfile("LU.txt");
    printfile("RADIX.txt");
	return 0;
}
