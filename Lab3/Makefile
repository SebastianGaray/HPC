CC=nvcc

all: suma2d suma2dshm

suma2d: 
	$(CC) $(CFLAGS) suma2d.cu -o suma2d
suma2dshm: 
	$(CC) $(CFLAGS) suma2dshm.cu -o suma2dshm

clean:
	rm suma2d
	rm suma2dshm
	