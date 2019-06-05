
CFLAGS = -lpthread -lm -ldl
MEX_FLAGS = -R2018a

sparseConv : sparseConv.c
	mex ${MEX_FLAGS} $^ ${CFLAGS}
