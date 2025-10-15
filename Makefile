all: 
	gcc -g -fsanitize=address test_for_approx_upper_limit.c  \
		minimal_sub_graph.c -o test_for_approx_upper_limit -lpthread -lm

phony: all