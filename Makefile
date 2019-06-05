#Note: The MKL build may need you to load gcc/6.3.0 instead of the default 4.x.x


CFLAGS = -lpthread -lm -ldl
MEX_FLAGS = -R2018a

sparseConv : sparseConv.c
	mex ${MEX_FLAGS} $^ ${CFLAGS}
