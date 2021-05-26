.PHONY:clean

#OUT=Serialize
#OUT=Structured

ifeq (${OUT},Serialize) 
libstemp=libCSerializer.a libCStoreByTypeAndReg.a libCStoreFileStrategy.a 
else
libstemp=libCStructuredJson.a libCStoreByTypeAndReg.a libCStoreFileStrategy.a 
endif
objstemp=${patsubst lib%.a,%.o,${libstemp}}
target=test.out

OBJDIR=./obj
LIBDIR=./lib
LLIBEX=-ljsoncpp
OBJS=${patsubst %,${OBJDIR}/%,${objstemp}}
LIBS=${patsubst %,${LIBDIR}/%,${libstemp}}
LLIBS=${patsubst lib%.a,-l%,${libstemp}}
ifeq (${OUT},Serialize) 
TESTSRC=./test/src/test1.cpp ./test/src/CA.cpp ./test/src/CB.cpp
INCLUDEDIR=-I./test/include -I./common/interface -I./common/Serializer/include
else
TESTSRC=./test/src/test2.cpp ./test/src/CC.cpp ./test/src/CD.cpp
INCLUDEDIR=-I./test/include -I./common/interface -I./common/Structured/include
endif


${target}:${LIBS}
	g++ -o $@ ${INCLUDEDIR} ${TESTSRC} -L${LIBDIR} ${LLIBS} ${LLIBEX}
${LIBS}:./${LIBDIR}/lib%.a:${OBJDIR}/%.o
	ar -rc $@ $<
ifeq (${OUT},Serialize) 
${OBJDIR}/%.o:./common/Serializer/src/%.cpp
else
${OBJDIR}/%.o:./common/Structured/src/%.cpp
endif
	g++ -c ${INCLUDEDIR} $< -o $@ 

clean:
	rm -f ./obj/*.o ./*.out ./*.txt ./*.json
