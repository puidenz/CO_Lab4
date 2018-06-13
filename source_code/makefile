#test
main: lru.out cache.out execute
lru.out:direct_mapped_cache_lru.cpp
	g++ -std=c++11 direct_mapped_cache_lru.cpp -o lru.out
cache.out:direct_mapped_cache.cpp
	g++ -std=c++11 direct_mapped_cache.cpp -o cache.out
clean:
	@-rm *.out
	@-rm output.txt
	@-rm result.txt
execute:
	@./cache.out > result.txt
	@./lru.out >> result.txt
	
