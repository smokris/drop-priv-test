all : test-parent test-child
test-parent : test-parent.c
	$(CC) -lcap -o test-parent test-parent.c
test-child : test-child.c
	$(CC) -lcap -o test-child test-child.c
