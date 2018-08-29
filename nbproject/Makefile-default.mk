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
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/GlobalCircuit.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/GlobalCircuit.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=main.c I2C.c transmit.c MAG3310.c MS5607.c GPS.c GPIODebug.c Timing.c Resets.c PICADC.c Stubs.c Watchdog.c RockBlock.c ADC.c SPI.c Ballast.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/main.o ${OBJECTDIR}/I2C.o ${OBJECTDIR}/transmit.o ${OBJECTDIR}/MAG3310.o ${OBJECTDIR}/MS5607.o ${OBJECTDIR}/GPS.o ${OBJECTDIR}/GPIODebug.o ${OBJECTDIR}/Timing.o ${OBJECTDIR}/Resets.o ${OBJECTDIR}/PICADC.o ${OBJECTDIR}/Stubs.o ${OBJECTDIR}/Watchdog.o ${OBJECTDIR}/RockBlock.o ${OBJECTDIR}/ADC.o ${OBJECTDIR}/SPI.o ${OBJECTDIR}/Ballast.o
POSSIBLE_DEPFILES=${OBJECTDIR}/main.o.d ${OBJECTDIR}/I2C.o.d ${OBJECTDIR}/transmit.o.d ${OBJECTDIR}/MAG3310.o.d ${OBJECTDIR}/MS5607.o.d ${OBJECTDIR}/GPS.o.d ${OBJECTDIR}/GPIODebug.o.d ${OBJECTDIR}/Timing.o.d ${OBJECTDIR}/Resets.o.d ${OBJECTDIR}/PICADC.o.d ${OBJECTDIR}/Stubs.o.d ${OBJECTDIR}/Watchdog.o.d ${OBJECTDIR}/RockBlock.o.d ${OBJECTDIR}/ADC.o.d ${OBJECTDIR}/SPI.o.d ${OBJECTDIR}/Ballast.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/main.o ${OBJECTDIR}/I2C.o ${OBJECTDIR}/transmit.o ${OBJECTDIR}/MAG3310.o ${OBJECTDIR}/MS5607.o ${OBJECTDIR}/GPS.o ${OBJECTDIR}/GPIODebug.o ${OBJECTDIR}/Timing.o ${OBJECTDIR}/Resets.o ${OBJECTDIR}/PICADC.o ${OBJECTDIR}/Stubs.o ${OBJECTDIR}/Watchdog.o ${OBJECTDIR}/RockBlock.o ${OBJECTDIR}/ADC.o ${OBJECTDIR}/SPI.o ${OBJECTDIR}/Ballast.o

# Source Files
SOURCEFILES=main.c I2C.c transmit.c MAG3310.c MS5607.c GPS.c GPIODebug.c Timing.c Resets.c PICADC.c Stubs.c Watchdog.c RockBlock.c ADC.c SPI.c Ballast.c


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
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/GlobalCircuit.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX360F512L
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/I2C.o: I2C.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/I2C.o.d 
	@${RM} ${OBJECTDIR}/I2C.o 
	@${FIXDEPS} "${OBJECTDIR}/I2C.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/I2C.o.d" -o ${OBJECTDIR}/I2C.o I2C.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/transmit.o: transmit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/transmit.o.d 
	@${RM} ${OBJECTDIR}/transmit.o 
	@${FIXDEPS} "${OBJECTDIR}/transmit.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/transmit.o.d" -o ${OBJECTDIR}/transmit.o transmit.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/MAG3310.o: MAG3310.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/MAG3310.o.d 
	@${RM} ${OBJECTDIR}/MAG3310.o 
	@${FIXDEPS} "${OBJECTDIR}/MAG3310.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/MAG3310.o.d" -o ${OBJECTDIR}/MAG3310.o MAG3310.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/MS5607.o: MS5607.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/MS5607.o.d 
	@${RM} ${OBJECTDIR}/MS5607.o 
	@${FIXDEPS} "${OBJECTDIR}/MS5607.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/MS5607.o.d" -o ${OBJECTDIR}/MS5607.o MS5607.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/GPS.o: GPS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/GPS.o.d 
	@${RM} ${OBJECTDIR}/GPS.o 
	@${FIXDEPS} "${OBJECTDIR}/GPS.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/GPS.o.d" -o ${OBJECTDIR}/GPS.o GPS.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/GPIODebug.o: GPIODebug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/GPIODebug.o.d 
	@${RM} ${OBJECTDIR}/GPIODebug.o 
	@${FIXDEPS} "${OBJECTDIR}/GPIODebug.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/GPIODebug.o.d" -o ${OBJECTDIR}/GPIODebug.o GPIODebug.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/Timing.o: Timing.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Timing.o.d 
	@${RM} ${OBJECTDIR}/Timing.o 
	@${FIXDEPS} "${OBJECTDIR}/Timing.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Timing.o.d" -o ${OBJECTDIR}/Timing.o Timing.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/Resets.o: Resets.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Resets.o.d 
	@${RM} ${OBJECTDIR}/Resets.o 
	@${FIXDEPS} "${OBJECTDIR}/Resets.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Resets.o.d" -o ${OBJECTDIR}/Resets.o Resets.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/PICADC.o: PICADC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/PICADC.o.d 
	@${RM} ${OBJECTDIR}/PICADC.o 
	@${FIXDEPS} "${OBJECTDIR}/PICADC.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/PICADC.o.d" -o ${OBJECTDIR}/PICADC.o PICADC.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/Stubs.o: Stubs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Stubs.o.d 
	@${RM} ${OBJECTDIR}/Stubs.o 
	@${FIXDEPS} "${OBJECTDIR}/Stubs.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Stubs.o.d" -o ${OBJECTDIR}/Stubs.o Stubs.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/Watchdog.o: Watchdog.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Watchdog.o.d 
	@${RM} ${OBJECTDIR}/Watchdog.o 
	@${FIXDEPS} "${OBJECTDIR}/Watchdog.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Watchdog.o.d" -o ${OBJECTDIR}/Watchdog.o Watchdog.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/RockBlock.o: RockBlock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/RockBlock.o.d 
	@${RM} ${OBJECTDIR}/RockBlock.o 
	@${FIXDEPS} "${OBJECTDIR}/RockBlock.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/RockBlock.o.d" -o ${OBJECTDIR}/RockBlock.o RockBlock.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/ADC.o: ADC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/ADC.o.d 
	@${RM} ${OBJECTDIR}/ADC.o 
	@${FIXDEPS} "${OBJECTDIR}/ADC.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/ADC.o.d" -o ${OBJECTDIR}/ADC.o ADC.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/SPI.o: SPI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/SPI.o.d 
	@${RM} ${OBJECTDIR}/SPI.o 
	@${FIXDEPS} "${OBJECTDIR}/SPI.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SPI.o.d" -o ${OBJECTDIR}/SPI.o SPI.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/Ballast.o: Ballast.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Ballast.o.d 
	@${RM} ${OBJECTDIR}/Ballast.o 
	@${FIXDEPS} "${OBJECTDIR}/Ballast.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Ballast.o.d" -o ${OBJECTDIR}/Ballast.o Ballast.c    -Wno-error=implicit-function-declaration
	
else
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/I2C.o: I2C.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/I2C.o.d 
	@${RM} ${OBJECTDIR}/I2C.o 
	@${FIXDEPS} "${OBJECTDIR}/I2C.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/I2C.o.d" -o ${OBJECTDIR}/I2C.o I2C.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/transmit.o: transmit.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/transmit.o.d 
	@${RM} ${OBJECTDIR}/transmit.o 
	@${FIXDEPS} "${OBJECTDIR}/transmit.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/transmit.o.d" -o ${OBJECTDIR}/transmit.o transmit.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/MAG3310.o: MAG3310.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/MAG3310.o.d 
	@${RM} ${OBJECTDIR}/MAG3310.o 
	@${FIXDEPS} "${OBJECTDIR}/MAG3310.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/MAG3310.o.d" -o ${OBJECTDIR}/MAG3310.o MAG3310.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/MS5607.o: MS5607.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/MS5607.o.d 
	@${RM} ${OBJECTDIR}/MS5607.o 
	@${FIXDEPS} "${OBJECTDIR}/MS5607.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/MS5607.o.d" -o ${OBJECTDIR}/MS5607.o MS5607.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/GPS.o: GPS.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/GPS.o.d 
	@${RM} ${OBJECTDIR}/GPS.o 
	@${FIXDEPS} "${OBJECTDIR}/GPS.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/GPS.o.d" -o ${OBJECTDIR}/GPS.o GPS.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/GPIODebug.o: GPIODebug.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/GPIODebug.o.d 
	@${RM} ${OBJECTDIR}/GPIODebug.o 
	@${FIXDEPS} "${OBJECTDIR}/GPIODebug.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/GPIODebug.o.d" -o ${OBJECTDIR}/GPIODebug.o GPIODebug.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/Timing.o: Timing.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Timing.o.d 
	@${RM} ${OBJECTDIR}/Timing.o 
	@${FIXDEPS} "${OBJECTDIR}/Timing.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Timing.o.d" -o ${OBJECTDIR}/Timing.o Timing.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/Resets.o: Resets.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Resets.o.d 
	@${RM} ${OBJECTDIR}/Resets.o 
	@${FIXDEPS} "${OBJECTDIR}/Resets.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Resets.o.d" -o ${OBJECTDIR}/Resets.o Resets.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/PICADC.o: PICADC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/PICADC.o.d 
	@${RM} ${OBJECTDIR}/PICADC.o 
	@${FIXDEPS} "${OBJECTDIR}/PICADC.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/PICADC.o.d" -o ${OBJECTDIR}/PICADC.o PICADC.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/Stubs.o: Stubs.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Stubs.o.d 
	@${RM} ${OBJECTDIR}/Stubs.o 
	@${FIXDEPS} "${OBJECTDIR}/Stubs.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Stubs.o.d" -o ${OBJECTDIR}/Stubs.o Stubs.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/Watchdog.o: Watchdog.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Watchdog.o.d 
	@${RM} ${OBJECTDIR}/Watchdog.o 
	@${FIXDEPS} "${OBJECTDIR}/Watchdog.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Watchdog.o.d" -o ${OBJECTDIR}/Watchdog.o Watchdog.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/RockBlock.o: RockBlock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/RockBlock.o.d 
	@${RM} ${OBJECTDIR}/RockBlock.o 
	@${FIXDEPS} "${OBJECTDIR}/RockBlock.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/RockBlock.o.d" -o ${OBJECTDIR}/RockBlock.o RockBlock.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/ADC.o: ADC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/ADC.o.d 
	@${RM} ${OBJECTDIR}/ADC.o 
	@${FIXDEPS} "${OBJECTDIR}/ADC.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/ADC.o.d" -o ${OBJECTDIR}/ADC.o ADC.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/SPI.o: SPI.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/SPI.o.d 
	@${RM} ${OBJECTDIR}/SPI.o 
	@${FIXDEPS} "${OBJECTDIR}/SPI.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/SPI.o.d" -o ${OBJECTDIR}/SPI.o SPI.c    -Wno-error=implicit-function-declaration
	
${OBJECTDIR}/Ballast.o: Ballast.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/Ballast.o.d 
	@${RM} ${OBJECTDIR}/Ballast.o 
	@${FIXDEPS} "${OBJECTDIR}/Ballast.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/Ballast.o.d" -o ${OBJECTDIR}/Ballast.o Ballast.c    -Wno-error=implicit-function-declaration
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/GlobalCircuit.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_ICD3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/GlobalCircuit.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}           -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC024FF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/GlobalCircuit.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/GlobalCircuit.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/GlobalCircuit.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
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

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
