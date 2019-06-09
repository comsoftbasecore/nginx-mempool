memtest : main.o mem_alloc.o mem_pool_palloc.o  
					cc -o memtest main.o mem_alloc.o mem_pool_palloc.o 


    main.o : main.c mem_core.h 
		cc -c main.c
    mem_alloc.o : mem_alloc.c mem_core.h 
		cc -c mem_alloc.c
    mem_pool_palloc.o : mem_pool_palloc.c mem_core.h 
		cc -c mem_pool_palloc.c
    clean :
		rm -fr  memtest main.o  mem_alloc.o mem_pool_palloc.o
