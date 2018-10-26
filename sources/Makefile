BEEINGS=existance.o vivent.o specied.o animal.o individualanimal.o vegetable.o individualvegetable.o
LIB=${BEEINGS} \
gender.o \
time.o \
container.o \
subsystemcontainer.o \
ecosystem.o \
controller.o \
like.o \
speciesinfo.o \
speciescontroller.o \
step.o \
step_log.o	\
draw_evolv.o

CC=g++ 
ROOT_FLAGS=`root-config --cflags`
ROOT_LIBS=`root-config --libs`
OPT_COMPL=-Wall -g -pedantic -D COMPLETE -D RUN_DBG -D VERBOSE_DBG -D SAFE -D VERBOSE -D VERBOSE_DBG
ROOT_OPT=-Wall -g -D COMPLETE -D RUN_DBG -D VERBOSE_DBG -D SAFE -D VERBOSE -D VERBOSE_DBG
OPT_RUN=-Wall -g -pedantic -D COMPLETE -D VERBOSE -D SAFE 
OPT_RUN_NOP=-g -pedantic -D COMPLETE -D VERBOSE 
OPT=${OPT_RUN} 

MAIN=fill_random_result
DEMO1=fill_random_result

${MAIN}:	${LIB} ${MAIN}
	${CC} ${OPT} ${LIB}  ${MAIN}.cpp  -o ${MAIN}.x ${ROOT_LIBS}

${DEMO1}:	${LIB} ${DEMO1}
	${CC} ${OPT} ${LIB}  ${DEMO1}.cpp  -o ${DEMO1}.x ${ROOT_LIBS}

%.o: %.cpp  
	${CC} ${OPT} -c $<	${ROOT_FLAGS}

run:
	./main.x 
clean:
	rm -rf *.o comandi*
