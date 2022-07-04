#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/the4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/the4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../serial_example/common.c ../serial_example/int.c ../serial_example/main.c ../serial_example/taskdesc.c ../serial_example/tsk_task0.c ../serial_example/tsk_task1.c ../lcd_sample/LCD.c ../lcd_sample/tsk_task0_lcd.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/274217394/common.o ${OBJECTDIR}/_ext/274217394/int.o ${OBJECTDIR}/_ext/274217394/main.o ${OBJECTDIR}/_ext/274217394/taskdesc.o ${OBJECTDIR}/_ext/274217394/tsk_task0.o ${OBJECTDIR}/_ext/274217394/tsk_task1.o ${OBJECTDIR}/_ext/132951029/LCD.o ${OBJECTDIR}/_ext/132951029/tsk_task0_lcd.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/274217394/common.o.d ${OBJECTDIR}/_ext/274217394/int.o.d ${OBJECTDIR}/_ext/274217394/main.o.d ${OBJECTDIR}/_ext/274217394/taskdesc.o.d ${OBJECTDIR}/_ext/274217394/tsk_task0.o.d ${OBJECTDIR}/_ext/274217394/tsk_task1.o.d ${OBJECTDIR}/_ext/132951029/LCD.o.d ${OBJECTDIR}/_ext/132951029/tsk_task0_lcd.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/274217394/common.o ${OBJECTDIR}/_ext/274217394/int.o ${OBJECTDIR}/_ext/274217394/main.o ${OBJECTDIR}/_ext/274217394/taskdesc.o ${OBJECTDIR}/_ext/274217394/tsk_task0.o ${OBJECTDIR}/_ext/274217394/tsk_task1.o ${OBJECTDIR}/_ext/132951029/LCD.o ${OBJECTDIR}/_ext/132951029/tsk_task0_lcd.o

# Source Files
SOURCEFILES=../serial_example/common.c ../serial_example/int.c ../serial_example/main.c ../serial_example/taskdesc.c ../serial_example/tsk_task0.c ../serial_example/tsk_task1.c ../lcd_sample/LCD.c ../lcd_sample/tsk_task0_lcd.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/the4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F8722
MP_PROCESSOR_OPTION_LD=18f8722
MP_LINKER_DEBUG_OPTION= -u_DEBUGCODESTART=0x1fd30 -u_DEBUGCODELEN=0x2d0 -u_DEBUGDATASTART=0xef4 -u_DEBUGDATALEN=0xb
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/274217394/common.o: ../serial_example/common.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/274217394" 
	@${RM} ${OBJECTDIR}/_ext/274217394/common.o.d 
	@${RM} ${OBJECTDIR}/_ext/274217394/common.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../../../picos/PICos18_v2_10/Include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/274217394/common.o   ../serial_example/common.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/274217394/common.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/274217394/common.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/274217394/int.o: ../serial_example/int.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/274217394" 
	@${RM} ${OBJECTDIR}/_ext/274217394/int.o.d 
	@${RM} ${OBJECTDIR}/_ext/274217394/int.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../../../picos/PICos18_v2_10/Include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/274217394/int.o   ../serial_example/int.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/274217394/int.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/274217394/int.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/274217394/main.o: ../serial_example/main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/274217394" 
	@${RM} ${OBJECTDIR}/_ext/274217394/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/274217394/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../../../picos/PICos18_v2_10/Include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/274217394/main.o   ../serial_example/main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/274217394/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/274217394/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/274217394/taskdesc.o: ../serial_example/taskdesc.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/274217394" 
	@${RM} ${OBJECTDIR}/_ext/274217394/taskdesc.o.d 
	@${RM} ${OBJECTDIR}/_ext/274217394/taskdesc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../../../picos/PICos18_v2_10/Include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/274217394/taskdesc.o   ../serial_example/taskdesc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/274217394/taskdesc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/274217394/taskdesc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/274217394/tsk_task0.o: ../serial_example/tsk_task0.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/274217394" 
	@${RM} ${OBJECTDIR}/_ext/274217394/tsk_task0.o.d 
	@${RM} ${OBJECTDIR}/_ext/274217394/tsk_task0.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../../../picos/PICos18_v2_10/Include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/274217394/tsk_task0.o   ../serial_example/tsk_task0.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/274217394/tsk_task0.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/274217394/tsk_task0.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/274217394/tsk_task1.o: ../serial_example/tsk_task1.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/274217394" 
	@${RM} ${OBJECTDIR}/_ext/274217394/tsk_task1.o.d 
	@${RM} ${OBJECTDIR}/_ext/274217394/tsk_task1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../../../picos/PICos18_v2_10/Include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/274217394/tsk_task1.o   ../serial_example/tsk_task1.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/274217394/tsk_task1.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/274217394/tsk_task1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/132951029/LCD.o: ../lcd_sample/LCD.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/132951029" 
	@${RM} ${OBJECTDIR}/_ext/132951029/LCD.o.d 
	@${RM} ${OBJECTDIR}/_ext/132951029/LCD.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../../../picos/PICos18_v2_10/Include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/132951029/LCD.o   ../lcd_sample/LCD.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/132951029/LCD.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/132951029/LCD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/132951029/tsk_task0_lcd.o: ../lcd_sample/tsk_task0_lcd.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/132951029" 
	@${RM} ${OBJECTDIR}/_ext/132951029/tsk_task0_lcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/132951029/tsk_task0_lcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../../../picos/PICos18_v2_10/Include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/132951029/tsk_task0_lcd.o   ../lcd_sample/tsk_task0_lcd.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/132951029/tsk_task0_lcd.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/132951029/tsk_task0_lcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
else
${OBJECTDIR}/_ext/274217394/common.o: ../serial_example/common.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/274217394" 
	@${RM} ${OBJECTDIR}/_ext/274217394/common.o.d 
	@${RM} ${OBJECTDIR}/_ext/274217394/common.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../../../picos/PICos18_v2_10/Include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/274217394/common.o   ../serial_example/common.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/274217394/common.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/274217394/common.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/274217394/int.o: ../serial_example/int.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/274217394" 
	@${RM} ${OBJECTDIR}/_ext/274217394/int.o.d 
	@${RM} ${OBJECTDIR}/_ext/274217394/int.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../../../picos/PICos18_v2_10/Include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/274217394/int.o   ../serial_example/int.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/274217394/int.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/274217394/int.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/274217394/main.o: ../serial_example/main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/274217394" 
	@${RM} ${OBJECTDIR}/_ext/274217394/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/274217394/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../../../picos/PICos18_v2_10/Include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/274217394/main.o   ../serial_example/main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/274217394/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/274217394/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/274217394/taskdesc.o: ../serial_example/taskdesc.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/274217394" 
	@${RM} ${OBJECTDIR}/_ext/274217394/taskdesc.o.d 
	@${RM} ${OBJECTDIR}/_ext/274217394/taskdesc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../../../picos/PICos18_v2_10/Include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/274217394/taskdesc.o   ../serial_example/taskdesc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/274217394/taskdesc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/274217394/taskdesc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/274217394/tsk_task0.o: ../serial_example/tsk_task0.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/274217394" 
	@${RM} ${OBJECTDIR}/_ext/274217394/tsk_task0.o.d 
	@${RM} ${OBJECTDIR}/_ext/274217394/tsk_task0.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../../../picos/PICos18_v2_10/Include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/274217394/tsk_task0.o   ../serial_example/tsk_task0.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/274217394/tsk_task0.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/274217394/tsk_task0.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/274217394/tsk_task1.o: ../serial_example/tsk_task1.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/274217394" 
	@${RM} ${OBJECTDIR}/_ext/274217394/tsk_task1.o.d 
	@${RM} ${OBJECTDIR}/_ext/274217394/tsk_task1.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../../../picos/PICos18_v2_10/Include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/274217394/tsk_task1.o   ../serial_example/tsk_task1.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/274217394/tsk_task1.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/274217394/tsk_task1.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/132951029/LCD.o: ../lcd_sample/LCD.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/132951029" 
	@${RM} ${OBJECTDIR}/_ext/132951029/LCD.o.d 
	@${RM} ${OBJECTDIR}/_ext/132951029/LCD.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../../../picos/PICos18_v2_10/Include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/132951029/LCD.o   ../lcd_sample/LCD.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/132951029/LCD.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/132951029/LCD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/132951029/tsk_task0_lcd.o: ../lcd_sample/tsk_task0_lcd.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/132951029" 
	@${RM} ${OBJECTDIR}/_ext/132951029/tsk_task0_lcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/132951029/tsk_task0_lcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../../../picos/PICos18_v2_10/Include" -ms -oa-  -I ${MP_CC_DIR}/../h  -fo ${OBJECTDIR}/_ext/132951029/tsk_task0_lcd.o   ../lcd_sample/tsk_task0_lcd.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/132951029/tsk_task0_lcd.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/132951029/tsk_task0_lcd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/the4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../../../picos/PICos18_v2_10/Kernel/picos18.lib  ../../../picos/PICos18_v2_10/Linker/18f8722.lkr
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) "../../../picos/PICos18_v2_10/Linker/18f8722.lkr"  -p$(MP_PROCESSOR_OPTION_LD)  -w -x -u_DEBUG -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map" -l"../../../picos/PICos18_v2_10/Kernel"  -z__MPLAB_BUILD=1  -u_CRUNTIME -z__MPLAB_DEBUG=1 -z__MPLAB_DEBUGGER_PK3=1 $(MP_LINKER_DEBUG_OPTION) -l ${MP_CC_DIR}/../lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/the4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}  ../../../picos/PICos18_v2_10/Kernel/picos18.lib 
else
dist/${CND_CONF}/${IMAGE_TYPE}/the4.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../../../picos/PICos18_v2_10/Kernel/picos18.lib ../../../picos/PICos18_v2_10/Linker/18f8722.lkr
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) "../../../picos/PICos18_v2_10/Linker/18f8722.lkr"  -p$(MP_PROCESSOR_OPTION_LD)  -w  -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map" -l"../../../picos/PICos18_v2_10/Kernel"  -z__MPLAB_BUILD=1  -u_CRUNTIME -l ${MP_CC_DIR}/../lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/the4.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}  ../../../picos/PICos18_v2_10/Kernel/picos18.lib 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
