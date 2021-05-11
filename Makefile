.PHONY:clean
CC=g++
CFLAGS=-O0
INCLUDES=-I${include}

target=a.out
srcs=${wildcard ${src}/*.cpp}
objs=${patsubst ${src}/%.cpp,${obj}/%.o,${srcs}}

include=./include
src=./src
obj=./obj

${target}:${objs}
	${CC} $? -o $@
${obj}/%.o:${src}/%.cpp
	${CC} -c $< -o $@ ${INCLUDES} ${CFLAGS}

clean:
	rm -f ${obj}/*.o ./${target} ./*.txt
