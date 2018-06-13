#include <iostream>
#include <stdio.h>
#include <math.h>

using namespace std;

struct cache_content
{
	bool v;
	unsigned int tag;
    // unsigned int	data[16];
};

const int K = 1024;

double log2(double n)
{
    // log(n) / log(2) is log2.
    return log(n) / log(double(2));
}


void simulate(int cache_size, int block_size,const char *filename)
{
	unsigned int tag, index, x;
	int offset_bit = (int)log2(block_size);
	int index_bit = (int)log2(cache_size / block_size);
	int line = cache_size >> (offset_bit);
    float count_all=0,count_miss=0,count_hit=0;
	cache_content *cache = new cache_content[line];

    //cout << "cache size: " << cache_size/K <<"  block size"<< block_size<< endl;
    printf("%3dK,%4dbyte\n",cache_size/K,block_size);
	for(int j = 0; j < line; j++)
		cache[j].v = false;

    FILE *fp = fopen(filename, "r");  // read file
    FILE *fp2 = fopen("output.txt", "a+");  //write?
	while(fscanf(fp, "%x", &x) != EOF)
    {
		//cout << hex << x << " ";
		index = (x >> offset_bit) & (line - 1);
		tag = x >> (index_bit + offset_bit);
		if(cache[index].v && cache[index].tag == tag)
        {
            count_hit++;
			cache[index].v = true;    // hit
        }
		else
        {
			cache[index].v = true;  // miss
			cache[index].tag = tag;
			count_miss++;
		}
        count_all++;
	}
	//cout << count_all << "<-all:miss_>" << count_miss<<endl;
	//cout << (count_miss/line)*100 << "%\n"<< endl;
	printf("miss rate = %f%%\n",(count_miss/count_all)*100);
	//printf("%f%%\n",(count_miss/(count_hit + count_miss))*100);
	fprintf(fp2,"%d %f\n",block_size,(count_miss/count_all)*100);

	fclose(fp);
	fclose(fp2);


	delete [] cache;
}

void printfile(const char *filename)
{
    printf("file: %s\n",filename);
      for(int j = 4;j <= 256;j *= 4){
        for(int i = 16;i <= 256;i *= 2)
            simulate(j * K ,i,filename);
        printf("\n");
      }
}

int main()
{
	// Let us simulate 4KB cache with 16B blocks
    printfile("DCACHE.txt");
    printfile("ICACHE.txt");
}
